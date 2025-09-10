/* duckdb extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_duckdb.h"
#include "duckdb_arginfo.h"
#include "zend_exceptions.h"
#include "duckdb.h"
#include "zend_interfaces.h"
#include <string.h>

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

static zend_class_entry *php_duckdb_object_ce;
static zend_class_entry *php_duckdb_exception_ce;
static zend_class_entry *php_duckdb_result_ce;
static zend_class_entry *php_duckdb_statement_ce;
static zend_class_entry *php_duckdb_result_iterator_ce;
static zend_class_entry *php_duckdb_appender_ce;

static zend_object_handlers php_duckdb_object_handlers;
static zend_object_handlers php_duckdb_result_handlers;
static zend_object_handlers php_duckdb_statement_handlers;
static zend_object_handlers php_duckdb_result_iterator_handlers;
static zend_object_handlers php_duckdb_appender_handlers;

typedef struct _php_duckdb_object
{
	duckdb_connection conn;
	duckdb_database db;
	uint32_t threads;
	HashTable udfs;
	zend_object std;
} php_duckdb_object;

typedef struct _php_duckdb_result
{
	duckdb_result res;
	zend_object std;
} php_duckdb_result;

typedef struct _php_duckdb_statement
{
	duckdb_prepared_statement stmt;
	zend_object std;
} php_duckdb_statement;

typedef struct _php_duckdb_udf
{
	zend_fcall_info fci;
	zend_fcall_info_cache fcc;
} php_duckdb_udf;

typedef struct _php_duckdb_result_iterator
{
	duckdb_result *res;
	duckdb_data_chunk chunk;
	idx_t row_count;
	idx_t col_count;
	idx_t row_idx;
	uint64_t key;
	bool valid;
	bool assoc;
	zend_object std;
} php_duckdb_result_iterator;

typedef struct _php_duckdb_appender
{
	duckdb_appender appender;
	zend_object std;
} php_duckdb_appender;

#define Z_DUCKDB_P(zv) ((php_duckdb_object *)((char *)(Z_OBJ_P(zv)) - XtOffsetOf(php_duckdb_object, std)))
#define Z_DUCKDB_RESULT_P(zv) ((php_duckdb_result *)((char *)(Z_OBJ_P(zv)) - XtOffsetOf(php_duckdb_result, std)))
#define Z_DUCKDB_STATEMENT_P(zv) ((php_duckdb_statement *)((char *)(Z_OBJ_P(zv)) - XtOffsetOf(php_duckdb_statement, std)))
#define Z_DUCKDB_RESULT_ITERATOR_P(zv) ((php_duckdb_result_iterator *)((char *)(Z_OBJ_P(zv)) - XtOffsetOf(php_duckdb_result_iterator, std)))
#define Z_DUCKDB_APPENDER_P(zv) ((php_duckdb_appender *)((char *)(Z_OBJ_P(zv)) - XtOffsetOf(php_duckdb_appender, std)))

static duckdb_logical_type php_duckdb_zval_to_logical_type(zend_uchar t)
{
	duckdb_logical_type lt;
	switch (t)
	{
		case IS_LONG:
			lt = duckdb_create_logical_type(DUCKDB_TYPE_BIGINT);
			break;

		case IS_DOUBLE:
			lt = duckdb_create_logical_type(DUCKDB_TYPE_DOUBLE);
			break;

		case IS_STRING:
			lt = duckdb_create_logical_type(DUCKDB_TYPE_VARCHAR);
			break;

		case IS_TRUE:
		case IS_FALSE:
		case _IS_BOOL:
			lt = duckdb_create_logical_type(DUCKDB_TYPE_BOOLEAN);
			break;
		
		default:
			lt = duckdb_create_logical_type(DUCKDB_TYPE_VARCHAR);
			break;
	}

	return lt;
}

static zend_uchar php_duckdb_zend_type_to_uchar(zend_type t)
{
	uint32_t mask = ZEND_TYPE_FULL_MASK(t);

	if (mask & MAY_BE_NULL)   	return IS_NULL;
    if (mask & MAY_BE_LONG)   	return IS_LONG;
    if (mask & MAY_BE_DOUBLE) 	return IS_DOUBLE;
    if (mask & MAY_BE_STRING) 	return IS_STRING;
    if (mask & MAY_BE_ARRAY)  	return IS_UNDEF;
    if (mask & MAY_BE_OBJECT) 	return IS_UNDEF;
    if (mask & MAY_BE_BOOL)   	return _IS_BOOL;
    if (mask & MAY_BE_RESOURCE) return IS_UNDEF;
    if (mask & MAY_BE_FALSE)  	return IS_FALSE;

    return IS_UNDEF;
}

static void php_duckdb_to_zval(zval *val, duckdb_type type, void *data, idx_t row)
{
	switch (type)
	{
		case DUCKDB_TYPE_BOOLEAN: {
			bool *arr = (bool*)data;
			ZVAL_BOOL(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_TINYINT: {
			int8_t *arr = (int8_t*)data;
			ZVAL_LONG(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_SMALLINT: {
			int16_t *arr = (int16_t*)data;
			ZVAL_LONG(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_INTEGER: {
			int32_t *arr = (int32_t*)data;
			ZVAL_LONG(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_BIGINT: {
			int64_t *arr = (int64_t*)data;
			if (SIZEOF_ZEND_LONG >= 8) {
				ZVAL_LONG(val, arr[row]);
			} else {
				char buf[32];
				snprintf(buf, sizeof(buf), "%" PRId64, arr[row]);
				ZVAL_STRING(val, buf);
			}
			break;
		}

		case DUCKDB_TYPE_UUID:
		case DUCKDB_TYPE_HUGEINT: {
			duckdb_hugeint *arr = (duckdb_hugeint*)data;
			duckdb_hugeint huint = arr[row];
			double value = duckdb_hugeint_to_double(huint);

			ZVAL_DOUBLE(val, value);
			break;
		}

		case DUCKDB_TYPE_UTINYINT: {
			uint8_t *arr = (uint8_t*)data;
			ZVAL_LONG(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_USMALLINT: {
			uint16_t *arr = (uint16_t*)data;
			ZVAL_LONG(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_UINTEGER: {
			uint32_t *arr = (uint32_t*)data;
			ZVAL_LONG(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_UBIGINT: {
			uint64_t *arr = (uint64_t*)data;
			if (SIZEOF_ZEND_LONG >= 8) {
				ZVAL_LONG(val, arr[row]);
			} else {
				char buf[32];
				snprintf(buf, sizeof(buf), "%" PRId64, arr[row]);
				ZVAL_STRING(val, buf);
			}
			break;
		}

		case DUCKDB_TYPE_FLOAT: {
			float *arr = (float*)data;
			ZVAL_DOUBLE(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_DOUBLE: {
			double *arr = (double*)data;
			ZVAL_DOUBLE(val, arr[row]);
			break;
		}

		case DUCKDB_TYPE_DECIMAL: {
			duckdb_decimal *arr = (duckdb_decimal*)data;
			duckdb_decimal dec = arr[row];
			double value = duckdb_decimal_to_double(dec);
			ZVAL_DOUBLE(val, value);
			break;
		}

		case DUCKDB_TYPE_DATE: {
			duckdb_date *arr = (duckdb_date*)data;
			ZVAL_LONG(val, arr[row].days);
			break;
		}

		case DUCKDB_TYPE_TIME: {
			duckdb_time *arr = (duckdb_time*)data;
			ZVAL_LONG(val, arr[row].micros);
			break;
		}

		case DUCKDB_TYPE_TIMESTAMP:
		case DUCKDB_TYPE_TIMESTAMP_S:
		case DUCKDB_TYPE_TIMESTAMP_TZ: {
			duckdb_timestamp *arr = (duckdb_timestamp*)data;
			duckdb_timestamp ts = arr[row];

			time_t sec = ts.micros / 1000000;
			int usec = ts.micros % 1000000;

			struct tm tm;
			gmtime_r(&sec, &tm);

			char buffer[32];
			strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);

			char final[40];
			snprintf(final, sizeof(final), "%s.%06d", buffer, usec);

			ZVAL_STRING(val, final);
			break;
		}

		case DUCKDB_TYPE_BLOB:
		case DUCKDB_TYPE_VARCHAR: {
			duckdb_string_t *arr = (duckdb_string_t*)data;
			duckdb_string_t s = arr[row];

			if (duckdb_string_is_inlined(s)) {
				ZVAL_STRINGL(val, s.value.inlined.inlined, s.value.inlined.length);
			} else {
				ZVAL_PSTRINGL(val, s.value.pointer.ptr, s.value.pointer.length);
			}

			break;
		}

		default:
			ZVAL_NULL(val);
			break;
	}
}

static zend_object *php_duckdb_object_new(zend_class_entry *ce)
{
	php_duckdb_object *obj = zend_object_alloc(sizeof(php_duckdb_object), ce);

	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);

	obj->std.handlers = &php_duckdb_object_handlers;

	zend_hash_init(&obj->udfs, 8, NULL, NULL, 0);

	return &obj->std;
}

static void php_duckdb_udf_free(php_duckdb_udf *udf)
{
	if (!udf)	return;

	zval_ptr_dtor(&udf->fci.function_name);

	for (uint32_t i = 0; i < udf->fci.param_count; i++) {
		zval_ptr_dtor(&udf->fci.params[i]);
		efree(udf->fci.params);
	}

	efree(udf);
}

static void php_duckdb_object_free(zend_object *object)
{
	php_duckdb_object *obj = (php_duckdb_object *)((char*)object - php_duckdb_object_handlers.offset);

	if (obj->conn) {
		duckdb_disconnect(&obj->conn);
	}

	if (obj->db) {
		duckdb_close(&obj->db);
	}

	zval *udf_tmp;
	ZEND_HASH_FOREACH_VAL(&obj->udfs, udf_tmp) {
		if (Z_TYPE_P(udf_tmp) == IS_PTR) {
			php_duckdb_udf *udf = Z_PTR_P(udf_tmp);

			if (udf) {
				php_duckdb_udf_free(udf);
			}
		}
	} ZEND_HASH_FOREACH_END();

	zend_hash_destroy(&obj->udfs);

	zend_object_std_dtor(&obj->std);
}

static zend_object *php_duckdb_result_new(zend_class_entry *ce)
{
	php_duckdb_result *obj = zend_object_alloc(sizeof(php_duckdb_result), ce);

	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);

	obj->std.handlers = &php_duckdb_result_handlers;
	return &obj->std;
}

static void php_duckdb_result_free(zend_object *object)
{
	php_duckdb_result *obj = (php_duckdb_result *)((char*)object - php_duckdb_result_handlers.offset);

	duckdb_destroy_result(&obj->res);

	zend_object_std_dtor(&obj->std);
}

static zend_object *php_duckdb_statement_new(zend_class_entry *ce)
{
	php_duckdb_statement *obj = zend_object_alloc(sizeof(php_duckdb_statement), ce);

	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);

	obj->std.handlers = &php_duckdb_statement_handlers;
	return &obj->std;
}

static void php_duckdb_statement_free(zend_object *object)
{
	php_duckdb_statement *obj = (php_duckdb_statement *)((char*)object - php_duckdb_statement_handlers.offset);

	if (obj->stmt) {
		duckdb_destroy_prepare(&obj->stmt);
	}

	zend_object_std_dtor(&obj->std);
}

static zend_object *php_duckdb_result_iterator_new(zend_class_entry *ce)
{
	php_duckdb_result_iterator *obj = zend_object_alloc(sizeof(php_duckdb_result_iterator), ce);

	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);

	obj->std.handlers = &php_duckdb_result_iterator_handlers;
	return &obj->std;
}

static void php_duckdb_result_iterator_free(zend_object *object)
{
	php_duckdb_result_iterator *obj = (php_duckdb_result_iterator *)((char*)object - php_duckdb_result_iterator_handlers.offset);

	if (obj->chunk) {
		duckdb_destroy_data_chunk(&obj->chunk);
	}

	zend_object_std_dtor(&obj->std);
}

static zend_object *php_duckdb_appender_new(zend_class_entry *ce)
{
	php_duckdb_appender *obj = zend_object_alloc(sizeof(php_duckdb_appender), ce);

	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);

	obj->std.handlers = &php_duckdb_appender_handlers;
	return &obj->std;
}

static void php_duckdb_appender_free(zend_object *object)
{
	php_duckdb_appender *obj = (php_duckdb_appender *)((char*)object - php_duckdb_appender_handlers.offset);

	if (obj->appender) {
		duckdb_appender_close(obj->appender);
		duckdb_appender_destroy(&obj->appender);
	}

	zend_object_std_dtor(&obj->std);
}

static void php_duckdb_udf_callback(duckdb_function_info info, duckdb_data_chunk input, duckdb_vector output)
{
	php_duckdb_udf *udf = (php_duckdb_udf *)duckdb_scalar_function_get_extra_info(info);

	if (!udf) {
		duckdb_scalar_function_set_error(info, "Internal function error: could not retrieve function call data.");
        return;
	}

	idx_t nrows = duckdb_data_chunk_get_size(input);

	if (nrows == 0) {
		return;
	}

	idx_t ncols = duckdb_data_chunk_get_column_count(input);

	zend_fcall_info fci = udf->fci;
	fci.param_count = ncols;
	fci.params = safe_emalloc(sizeof(zval), ncols, 0);

	for (idx_t r = 0; r < nrows; r++) {
		for (idx_t c = 0; c < ncols; c++) {
			duckdb_vector v = duckdb_data_chunk_get_vector(input, c);
			duckdb_logical_type lt = duckdb_vector_get_column_type(v);
			duckdb_type c_type = duckdb_get_type_id(lt);
			uint64_t *validity = duckdb_vector_get_validity(v);
			void *data = duckdb_vector_get_data(v);
			
			if (!duckdb_validity_row_is_valid(validity, r)) {
				ZVAL_NULL(&fci.params[c]);
			} else {
				php_duckdb_to_zval(&fci.params[c], c_type, data, r);
			}

			duckdb_destroy_logical_type(&lt);
		}

		zval retval;
		fci.retval = &retval;

		if (zend_call_function(&fci, &udf->fcc) != SUCCESS) {
			duckdb_scalar_function_set_error(info, "PHP Function call failed");
			
			for (uint32_t i = 0; i < fci.param_count; i++) {
				zval_ptr_dtor(&fci.params[i]);
			}

			if (fci.params) {
				efree(fci.params);
			}

			zval_ptr_dtor(&retval);

			return;
		}

		if (EG(exception)) {
			for (uint32_t i = 0; i < fci.param_count; i++) {
				zval_ptr_dtor(&fci.params[i]);
			}

			if (fci.params) {
				efree(fci.params);
			}

			zval_ptr_dtor(&retval);

			return;
		}

		duckdb_vector_ensure_validity_writable(output);

		void *output_data = duckdb_vector_get_data(output);
		uint64_t *output_validity = duckdb_vector_get_validity(output);
		duckdb_logical_type output_lt = duckdb_vector_get_column_type(output);
		duckdb_type output_t = duckdb_get_type_id(output_lt);
		duckdb_destroy_logical_type(&output_lt);

		if (Z_TYPE(retval) == IS_NULL) {
			duckdb_validity_set_row_invalid(output_validity, r);
		} else {
			switch (output_t)
			{
				case DUCKDB_TYPE_BOOLEAN: {
					((bool *)output_data)[r] = Z_TYPE(retval) == IS_TRUE ? true : false;
					break;
				}

				case DUCKDB_TYPE_BIGINT: {
					((int64_t *)output_data)[r] = (int64_t)Z_LVAL(retval);
					break;
				}

				case DUCKDB_TYPE_FLOAT:
				case DUCKDB_TYPE_DOUBLE: {
					((double *)output_data)[r] = (double)Z_DVAL(retval);
					break;
				}

				case DUCKDB_TYPE_VARCHAR: {
					zend_string *str = Z_STR(retval);
					duckdb_vector_assign_string_element_len(output, r, ZSTR_VAL(str), ZSTR_LEN(str));
					break;
				}

				default:
					duckdb_validity_set_row_invalid(output_validity, r);
					break;
			}
		}

		for (uint32_t i = 0; i < fci.param_count; i++) {
			zval_ptr_dtor(&fci.params[i]);
		}
		
		zval_ptr_dtor(&retval);
	}

	if (fci.params) {
		efree(fci.params);
	}
}

ZEND_METHOD(Fnvoid_DuckDB_DuckDB, __construct)
{
	php_duckdb_object *self = Z_DUCKDB_P(ZEND_THIS);
	char *db_file = NULL;
	size_t db_file_len;
	zval *param_configs = NULL;

	ZEND_PARSE_PARAMETERS_START(0,2)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING_OR_NULL(db_file, db_file_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY(param_configs)
	ZEND_PARSE_PARAMETERS_END();

	duckdb_config config;
	self->threads = 0;

	if (duckdb_create_config(&config) == DuckDBError) {
		zend_throw_exception(php_duckdb_exception_ce, "Config init failed", 0);
		duckdb_destroy_config(&config);
		RETURN_THROWS();
	}

	if (param_configs != NULL) {
		HashTable *z_configs = Z_ARRVAL_P(param_configs);
		zend_string *key;
		zval *val;
		
		if (zend_hash_num_elements(z_configs) > 0) {
			ZEND_HASH_FOREACH_STR_KEY_VAL(z_configs, key, val) {
				const char *c_name = ZSTR_VAL(key);
				zend_string *val_str = zval_get_string(val);
				const char *c_val = ZSTR_VAL(val_str);

				if (duckdb_set_config(config, c_name, c_val) == DuckDBError) {
					zend_throw_exception_ex(php_duckdb_exception_ce, 0, "Failed to set config: '%s' = '%s'", c_name, c_val);
					duckdb_destroy_config(&config);
					RETURN_THROWS();
				}

				if (c_name && strcmp(c_name, "threads") == 0) {
					self->threads = Z_LVAL_P(val);
				}
			} ZEND_HASH_FOREACH_END();
		}
	}

	char *db_open_err = NULL;
	if (duckdb_open_ext(db_file, &self->db, config, &db_open_err) == DuckDBError) {
		zend_throw_exception_ex(php_duckdb_exception_ce, 0, "DB Open Error: %s", db_open_err);
		duckdb_destroy_config(&config);
		duckdb_free(db_open_err);
		RETURN_THROWS();
	}

	if (db_open_err != NULL) {
		duckdb_free(db_open_err);
	}

	if (duckdb_connect(self->db, &self->conn) == DuckDBError) {
		duckdb_destroy_config(&config);
		duckdb_close(&self->db);
		zend_throw_exception(php_duckdb_exception_ce, "Could not connect to database", 0);
		RETURN_THROWS();
	}

	duckdb_destroy_config(&config);
}

ZEND_METHOD(Fnvoid_DuckDB_DuckDB, query)
{
	php_duckdb_object *self = Z_DUCKDB_P(ZEND_THIS);
	char *sql;
	size_t sql_len;

	ZEND_PARSE_PARAMETERS_START(1,1)
		Z_PARAM_STRING(sql, sql_len)
	ZEND_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_duckdb_result_ce);
	php_duckdb_result *res = Z_DUCKDB_RESULT_P(return_value);

	if (duckdb_query(self->conn, sql, &res->res) != DuckDBSuccess) {
        zend_throw_exception(php_duckdb_exception_ce, duckdb_result_error(&res->res), 0);
        RETURN_THROWS();
    }
}

ZEND_METHOD(Fnvoid_DuckDB_DuckDB, prepare)
{
	php_duckdb_object *self = Z_DUCKDB_P(ZEND_THIS);
	char *sql;
	size_t sql_len;

	ZEND_PARSE_PARAMETERS_START(1,1)
		Z_PARAM_STRING(sql, sql_len)
	ZEND_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_duckdb_statement_ce);
	php_duckdb_statement *stmt = Z_DUCKDB_STATEMENT_P(return_value);

	if (duckdb_prepare(self->conn, sql, &stmt->stmt) == DuckDBError) {
		zend_throw_exception(php_duckdb_exception_ce, duckdb_prepare_error(stmt->stmt), 0);
        RETURN_THROWS();
	}
}

ZEND_METHOD(Fnvoid_DuckDB_DuckDB, registerFunction)
{
	php_duckdb_object *self = Z_DUCKDB_P(ZEND_THIS);
	char *name;
	size_t name_len;
	zend_fcall_info fci;
	zend_fcall_info_cache fcc;

	ZEND_PARSE_PARAMETERS_START(2,2)
		Z_PARAM_STRING(name, name_len)
		Z_PARAM_FUNC(fci, fcc)
	ZEND_PARSE_PARAMETERS_END();

	if (self->threads != 1) {
		zend_throw_exception(php_duckdb_exception_ce, "Please set threads = 1 to use user functions", 0);
		RETURN_THROWS();
	}

	if (zend_hash_str_exists(&self->udfs, name, name_len)) {
		zend_throw_exception_ex(php_duckdb_exception_ce, 0, "Function: %s is already registered", name);
		RETURN_THROWS();
	}

	zend_uchar zt = IS_UNDEF;
	zend_function *zf = fcc.function_handler;

	if (zf->common.fn_flags & ZEND_ACC_HAS_RETURN_TYPE) {
		zend_type zrt = zf->common.arg_info[-1].type;
		zt = php_duckdb_zend_type_to_uchar(zrt);
	}

	if (zt == IS_UNDEF) {
		zend_throw_exception_ex(php_duckdb_exception_ce, 0, "Function: %s return type is invalid (Allowed: string, int, float, double, null, bool)", name);
		RETURN_THROWS();
	}

	php_duckdb_udf *udf = (php_duckdb_udf *)emalloc(sizeof(php_duckdb_udf));
	udf->fci = fci;
	udf->fcc = fcc;

	Z_TRY_ADDREF_P(&udf->fci.function_name);

	zend_hash_str_add_ptr(&self->udfs, name, name_len, udf);

	duckdb_scalar_function func = duckdb_create_scalar_function();
	duckdb_scalar_function_set_name(func, name);

	for (uint32_t i = 0; i < zf->common.num_args; i++) {
		duckdb_logical_type t = php_duckdb_zval_to_logical_type(php_duckdb_zend_type_to_uchar(zf->common.arg_info[i].type));
		duckdb_scalar_function_add_parameter(func, t);
		duckdb_destroy_logical_type(&t);
	}

	// Return Type
	duckdb_logical_type rt = php_duckdb_zval_to_logical_type(zt);
	duckdb_scalar_function_set_return_type(func, rt);
	duckdb_destroy_logical_type(&rt);

	duckdb_scalar_function_set_function(func, php_duckdb_udf_callback);
	duckdb_scalar_function_set_extra_info(func, (void *)udf, NULL);

	if (duckdb_register_scalar_function(self->conn, func) != DuckDBSuccess) {
		zend_throw_exception_ex(php_duckdb_exception_ce, 0, "Could not register function: %s", name);
		duckdb_destroy_scalar_function(&func);
		RETURN_FALSE;
	}

	duckdb_destroy_scalar_function(&func);

	RETURN_TRUE;
}

ZEND_METHOD(Fnvoid_DuckDB_DuckDB, createAppender)
{
	char *table;
	size_t table_len;

	ZEND_PARSE_PARAMETERS_START(1,1)
		Z_PARAM_STRING(table, table_len);
	ZEND_PARSE_PARAMETERS_END();

	php_duckdb_object *self = Z_DUCKDB_P(ZEND_THIS);

	object_init_ex(return_value, php_duckdb_appender_ce);
	php_duckdb_appender *appender = Z_DUCKDB_APPENDER_P(return_value);

	if (duckdb_appender_create(self->conn, NULL, table, &appender->appender) == DuckDBError) {
		zend_throw_exception_ex(php_duckdb_exception_ce, 0, "Appender Error: %s", duckdb_appender_error(appender->appender));
		RETURN_THROWS();
	}
}

ZEND_METHOD(Fnvoid_DuckDB_DuckDB, duckDBVersion)
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETVAL_STRING(duckdb_library_version());
}

ZEND_METHOD(Fnvoid_DuckDB_Result, fetchAll)
{
	php_duckdb_result *self = Z_DUCKDB_RESULT_P(ZEND_THIS);
	bool assoc = false;

	ZEND_PARSE_PARAMETERS_START(0,1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(assoc)
	ZEND_PARSE_PARAMETERS_END();

	if (self->res.internal_data == NULL) {
		zend_throw_exception(php_duckdb_exception_ce, "Result is not initialized yet", 0);
		RETURN_THROWS();
	}

	array_init(return_value);

	while (1)
	{
		duckdb_data_chunk chunk = duckdb_fetch_chunk(self->res);

		if (!chunk) {
			duckdb_destroy_data_chunk(&chunk);
			break;
		}

		idx_t nrows = duckdb_data_chunk_get_size(chunk);

		if (nrows == 0) {
			duckdb_destroy_data_chunk(&chunk);
			break;
		}

		idx_t ncols = duckdb_data_chunk_get_column_count(chunk);

		for (idx_t r = 0; r < nrows; r++) {
			zval row;

			if (assoc) {
				array_init_size(&row, 1);
			} else {
				object_init(&row);
			}

			for (idx_t c = 0; c < ncols; c++) {
				duckdb_vector v = duckdb_data_chunk_get_vector(chunk, c);
				duckdb_type c_type = duckdb_column_type(&self->res, c);
				uint64_t *validity = duckdb_vector_get_validity(v);
				void *data = duckdb_vector_get_data(v);
				zval val;

				if (!duckdb_validity_row_is_valid(validity, r)) {
					ZVAL_NULL(&val);
				} else {
					php_duckdb_to_zval(&val, c_type, data, r);
				}

				const char *c_name = duckdb_column_name(&self->res, c);
				
				if (assoc) {
					add_assoc_zval(&row, c_name, &val);
				} else {
					zend_update_property(zend_standard_class_def, Z_OBJ(row), c_name, strlen(c_name), &val);
				}
			}

			add_next_index_zval(return_value, &row);
		}

		duckdb_destroy_data_chunk(&chunk);
	}
}

ZEND_METHOD(Fnvoid_DuckDB_Result, fetchOne)
{
	php_duckdb_result *self = Z_DUCKDB_RESULT_P(ZEND_THIS);
	bool assoc = false;

	ZEND_PARSE_PARAMETERS_START(0,1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(assoc)
	ZEND_PARSE_PARAMETERS_END();

	if (self->res.internal_data == NULL) {
		zend_throw_exception(php_duckdb_exception_ce, "Result is not initialized yet", 0);
		RETURN_THROWS();
	}

	duckdb_data_chunk chunk = duckdb_fetch_chunk(self->res);

	if (!chunk) {
		duckdb_destroy_data_chunk(&chunk);
		RETURN_NULL();
	}

	idx_t nrows = duckdb_data_chunk_get_size(chunk);

	if (nrows == 0) {
		duckdb_destroy_data_chunk(&chunk);
		RETURN_NULL();
	}

	idx_t ncols = duckdb_data_chunk_get_column_count(chunk);

	if (assoc) {
		array_init_size(return_value, 1);
	} else {
		object_init(return_value);
	}

	for (idx_t c = 0; c < ncols; c++) {
		duckdb_vector v = duckdb_data_chunk_get_vector(chunk, c);
		duckdb_type c_type = duckdb_column_type(&self->res, c);
		uint64_t *validity = duckdb_vector_get_validity(v);
		void *data = duckdb_vector_get_data(v);
		zval val;

		if (!duckdb_validity_row_is_valid(validity, 0)) {
			ZVAL_NULL(&val);
		} else {
			php_duckdb_to_zval(&val, c_type, data, 0);
		}

		const char *c_name = duckdb_column_name(&self->res, c);

		if (assoc) {
			add_assoc_zval(return_value, c_name, &val);
		} else {
			zend_update_property(zend_standard_class_def, Z_OBJ_P(return_value), c_name, strlen(c_name), &val);
		}
	}

	duckdb_destroy_data_chunk(&chunk);
}

ZEND_METHOD(Fnvoid_DuckDB_Result, iterate)
{
	php_duckdb_result *self = Z_DUCKDB_RESULT_P(ZEND_THIS);
	bool assoc;

	ZEND_PARSE_PARAMETERS_START(0,1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(assoc)
	ZEND_PARSE_PARAMETERS_END();

	if (self->res.internal_data == NULL) {
		zend_throw_exception(php_duckdb_exception_ce, "Result is not initialized yet", 0);
		RETURN_THROWS();
	}

	object_init_ex(return_value, php_duckdb_result_iterator_ce);
	php_duckdb_result_iterator *it = Z_DUCKDB_RESULT_ITERATOR_P(return_value);
	it->chunk = duckdb_fetch_chunk(self->res);
	it->res = &self->res;
	it->row_idx = 0;
	it->assoc = assoc;
	it->key = 0;

	if (it->chunk) {
		it->row_count = duckdb_data_chunk_get_size(it->chunk);
		it->col_count = duckdb_data_chunk_get_column_count(it->chunk);
		it->valid = true;
	} else {
		it->valid = false;
	}
}

ZEND_METHOD(Fnvoid_DuckDB_Statement, execute)
{
	zval *values;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(values)
	ZEND_PARSE_PARAMETERS_END();

	php_duckdb_statement *self = Z_DUCKDB_STATEMENT_P(ZEND_THIS);

	if (!self->stmt) {
		zend_throw_exception(php_duckdb_exception_ce, "Statement is not initialized yet", 0);
		RETURN_THROWS();
	}

	HashTable *params = Z_ARRVAL_P(values);
	zval *val;
	idx_t param_idx = 1;

	ZEND_HASH_FOREACH_VAL(params, val) {
		switch (Z_TYPE_P(val)) {
            case IS_NULL:
                duckdb_bind_null(self->stmt, param_idx);
                break;

            case IS_LONG:
                duckdb_bind_int64(self->stmt, param_idx, Z_LVAL_P(val));
                break;

            case IS_DOUBLE:
                duckdb_bind_double(self->stmt, param_idx, Z_DVAL_P(val));
                break;

            case IS_STRING:
                duckdb_bind_varchar_length(self->stmt, param_idx, Z_STRVAL_P(val), Z_STRLEN_P(val));
                break;

            case IS_TRUE:
            case IS_FALSE:
                duckdb_bind_boolean(self->stmt, param_idx, (Z_TYPE_P(val) == IS_TRUE));
                break;

            default:
                zend_throw_exception(php_duckdb_exception_ce, "Parameter data type is not supported", 0);
                RETURN_THROWS();
        }

        param_idx++;
	} ZEND_HASH_FOREACH_END();

	object_init_ex(return_value, php_duckdb_result_ce);
	php_duckdb_result *res = Z_DUCKDB_RESULT_P(return_value);

	if (duckdb_execute_prepared(self->stmt, &res->res) == DuckDBError) {
        zend_throw_exception(php_duckdb_exception_ce, duckdb_result_error(&res->res), 0);
        RETURN_THROWS();
    }
}

ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, rewind)
{
	php_duckdb_result_iterator *self = Z_DUCKDB_RESULT_ITERATOR_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (!self->res) {
		zend_throw_exception(php_duckdb_exception_ce, "Result is not initialized yet", 0);
		RETURN_THROWS();
	}

	self->row_idx = 0;
}

ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, current)
{
	php_duckdb_result_iterator *self = Z_DUCKDB_RESULT_ITERATOR_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (!self->res) {
		zend_throw_exception(php_duckdb_exception_ce, "Result is not initialized yet", 0);
		RETURN_THROWS();
	}

	if (!self->chunk) {
		RETURN_NULL();
	}

	if (self->row_count == 0) {
		RETURN_NULL();
	}

	if (self->assoc) {
		array_init_size(return_value, 1);
	} else {
		object_init(return_value);
	}

	for (idx_t c = 0; c < self->col_count; c++) {
		duckdb_vector v = duckdb_data_chunk_get_vector(self->chunk, c);
		duckdb_type c_type = duckdb_column_type(self->res, c);
		uint64_t *validity = duckdb_vector_get_validity(v);
		void *data = duckdb_vector_get_data(v);
		zval val;

		if (!duckdb_validity_row_is_valid(validity, self->row_idx)) {
			ZVAL_NULL(&val);
		} else {
			php_duckdb_to_zval(&val, c_type, data, self->row_idx);
		}

		const char *c_name = duckdb_column_name(self->res, c);

		if (self->assoc) {
			add_assoc_zval(return_value, c_name, &val);
		} else {
			zend_update_property(zend_standard_class_def, Z_OBJ_P(return_value), c_name, strlen(c_name), &val);
		}
	}
}

ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, key)
{
	php_duckdb_result_iterator *self = Z_DUCKDB_RESULT_ITERATOR_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (!self->res) {
		zend_throw_exception(php_duckdb_exception_ce, "Result is not initialized yet", 0);
		RETURN_THROWS();
	}

	RETURN_LONG(self->key);
}

ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, next)
{
	php_duckdb_result_iterator *self = Z_DUCKDB_RESULT_ITERATOR_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (!self->res) {
		zend_throw_exception(php_duckdb_exception_ce, "Result is not initialized yet", 0);
		RETURN_THROWS();
	}

	self->row_idx++;
	self->key++;

	if (self->row_idx >= self->row_count) {
		self->chunk = duckdb_fetch_chunk(*self->res);
		self->row_idx = 0;

		if (self->chunk) {
			self->row_count = duckdb_data_chunk_get_size(self->chunk);
			self->col_count = duckdb_data_chunk_get_column_count(self->chunk);
			self->valid = true;
		} else {
			self->valid = false;
		}
	}
}

ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, valid)
{
	php_duckdb_result_iterator *self = Z_DUCKDB_RESULT_ITERATOR_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (!self->res) {
		zend_throw_exception(php_duckdb_exception_ce, "Result is not initialized yet", 0);
		RETURN_THROWS();
	}

	ZVAL_BOOL(return_value, self->valid);
}

ZEND_METHOD(Fnvoid_DuckDB_Appender, appendRow)
{
	zval *param_row;

	ZEND_PARSE_PARAMETERS_START(1,1)
		Z_PARAM_ARRAY(param_row)
	ZEND_PARSE_PARAMETERS_END();

	php_duckdb_appender *self = Z_DUCKDB_APPENDER_P(ZEND_THIS);

	if (!self->appender) {
		zend_throw_exception(php_duckdb_exception_ce, "Appender is not initialized yet", 0);
		RETURN_THROWS();
	}

	HashTable *row = Z_ARRVAL_P(param_row);
	zval *val;

	ZEND_HASH_FOREACH_VAL(row, val) {
		switch (Z_TYPE_P(val)) {
            case IS_NULL:
				duckdb_append_null(self->appender);
                break;

            case IS_LONG:
				duckdb_append_int64(self->appender, Z_LVAL_P(val));
                break;

            case IS_DOUBLE:
				duckdb_append_double(self->appender, Z_DVAL_P(val));
                break;

            case IS_STRING:
				duckdb_append_varchar_length(self->appender, Z_STRVAL_P(val), Z_STRLEN_P(val));
                break;

            case IS_TRUE:
            case IS_FALSE:
				duckdb_append_bool(self->appender, (Z_TYPE_P(val) == IS_TRUE));
                break;

            default:
                zend_throw_exception(php_duckdb_exception_ce, "Unsupported data type passed, allowed (string, int, float, double, bool, null)", 0);
                RETURN_THROWS();
        }
	} ZEND_HASH_FOREACH_END();

	if (duckdb_appender_end_row(self->appender) == DuckDBError) {
		zend_throw_exception_ex(php_duckdb_exception_ce, 0, "Appending error: %s", duckdb_appender_error(self->appender));
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

ZEND_METHOD(Fnvoid_DuckDB_Appender, flush)
{
	php_duckdb_appender *self = Z_DUCKDB_APPENDER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (duckdb_appender_flush(self->appender) == DuckDBError) {
		zend_throw_exception_ex(php_duckdb_exception_ce, 0, "Flush Error: %s", duckdb_appender_error(self->appender));
	}
}

PHP_MINIT_FUNCTION(duckdb)
{
	memcpy(&php_duckdb_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_duckdb_object_handlers.offset = XtOffsetOf(php_duckdb_object, std);
	php_duckdb_object_handlers.free_obj = php_duckdb_object_free;

	memcpy(&php_duckdb_result_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_duckdb_result_handlers.offset = XtOffsetOf(php_duckdb_result, std);
	php_duckdb_result_handlers.free_obj = php_duckdb_result_free;

	memcpy(&php_duckdb_statement_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_duckdb_statement_handlers.offset = XtOffsetOf(php_duckdb_statement, std);
	php_duckdb_statement_handlers.free_obj = php_duckdb_statement_free;

	memcpy(&php_duckdb_result_iterator_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_duckdb_result_iterator_handlers.offset = XtOffsetOf(php_duckdb_result_iterator, std);
	php_duckdb_result_iterator_handlers.free_obj = php_duckdb_result_iterator_free;

	memcpy(&php_duckdb_appender_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_duckdb_appender_handlers.offset = XtOffsetOf(php_duckdb_appender, std);
	php_duckdb_appender_handlers.free_obj = php_duckdb_appender_free;

	php_duckdb_exception_ce = register_class_Fnvoid_DuckDB_Exception(zend_ce_exception);

	php_duckdb_object_ce = register_class_Fnvoid_DuckDB_DuckDB();
	php_duckdb_object_ce->create_object = php_duckdb_object_new;

	php_duckdb_result_ce = register_class_Fnvoid_DuckDB_Result();
	php_duckdb_result_ce->create_object = php_duckdb_result_new;

	php_duckdb_statement_ce = register_class_Fnvoid_DuckDB_Statement();
	php_duckdb_statement_ce->create_object = php_duckdb_statement_new;

	php_duckdb_result_iterator_ce = register_class_Fnvoid_DuckDB_ResultIterator(zend_ce_iterator);
	php_duckdb_result_iterator_ce->create_object = php_duckdb_result_iterator_new;

	php_duckdb_appender_ce = register_class_Fnvoid_DuckDB_Appender();
	php_duckdb_appender_ce->create_object = php_duckdb_appender_new;


	return SUCCESS;
}


PHP_RINIT_FUNCTION(duckdb)
{
#if defined(ZTS) && defined(COMPILE_DL_DUCKDB)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}

PHP_MINFO_FUNCTION(duckdb)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "duckdb support", "enabled");
	php_info_print_table_end();
}

zend_module_entry duckdb_module_entry = {
	STANDARD_MODULE_HEADER,
	"duckdb",					/* Extension name */
	NULL,						/* zend_function_entry */
	PHP_MINIT(duckdb),			/* PHP_MINIT - Module initialization */
	NULL,						/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(duckdb),			/* PHP_RINIT - Request initialization */
	NULL,						/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(duckdb),			/* PHP_MINFO - Module info */
	PHP_DUCKDB_VERSION,			/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DUCKDB
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(duckdb)
#endif
