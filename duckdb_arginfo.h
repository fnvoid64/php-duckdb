/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a55be721db960c1694edd5943cf2e046ded71774 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Fnvoid_DuckDB_DuckDB___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, db_file, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, config, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Fnvoid_DuckDB_DuckDB_query, 0, 1, Fnvoid\\DuckDB\\Result, 0)
	ZEND_ARG_TYPE_INFO(0, sql, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Fnvoid_DuckDB_DuckDB_prepare, 0, 1, Fnvoid\\DuckDB\\Statement, 0)
	ZEND_ARG_TYPE_INFO(0, sql, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Fnvoid_DuckDB_DuckDB_registerFunction, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, func, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Fnvoid_DuckDB_DuckDB_createAppender, 0, 1, Fnvoid\\DuckDB\\Appender, 0)
	ZEND_ARG_TYPE_INFO(0, table, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Fnvoid_DuckDB_DuckDB_duckDBVersion, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Fnvoid_DuckDB_Result_fetchAll, 0, 0, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, assoc, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_Fnvoid_DuckDB_Result_fetchOne, 0, 0, MAY_BE_OBJECT|MAY_BE_ARRAY|MAY_BE_NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, assoc, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Fnvoid_DuckDB_Result_iterate, 0, 0, Fnvoid\\DuckDB\\ResultIterator, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, assoc, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Fnvoid_DuckDB_ResultIterator_rewind, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_Fnvoid_DuckDB_ResultIterator_current, 0, 0, MAY_BE_ARRAY|MAY_BE_OBJECT|MAY_BE_NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Fnvoid_DuckDB_ResultIterator_key, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Fnvoid_DuckDB_ResultIterator_next arginfo_class_Fnvoid_DuckDB_ResultIterator_rewind

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Fnvoid_DuckDB_ResultIterator_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Fnvoid_DuckDB_Statement_execute, 0, 1, Fnvoid\\DuckDB\\Result, 0)
	ZEND_ARG_TYPE_INFO(0, values, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Fnvoid_DuckDB_Appender_appendRow, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, row, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Fnvoid_DuckDB_Appender_flush arginfo_class_Fnvoid_DuckDB_ResultIterator_rewind


ZEND_METHOD(Fnvoid_DuckDB_DuckDB, __construct);
ZEND_METHOD(Fnvoid_DuckDB_DuckDB, query);
ZEND_METHOD(Fnvoid_DuckDB_DuckDB, prepare);
ZEND_METHOD(Fnvoid_DuckDB_DuckDB, registerFunction);
ZEND_METHOD(Fnvoid_DuckDB_DuckDB, createAppender);
ZEND_METHOD(Fnvoid_DuckDB_DuckDB, duckDBVersion);
ZEND_METHOD(Fnvoid_DuckDB_Result, fetchAll);
ZEND_METHOD(Fnvoid_DuckDB_Result, fetchOne);
ZEND_METHOD(Fnvoid_DuckDB_Result, iterate);
ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, rewind);
ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, current);
ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, key);
ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, next);
ZEND_METHOD(Fnvoid_DuckDB_ResultIterator, valid);
ZEND_METHOD(Fnvoid_DuckDB_Statement, execute);
ZEND_METHOD(Fnvoid_DuckDB_Appender, appendRow);
ZEND_METHOD(Fnvoid_DuckDB_Appender, flush);


static const zend_function_entry class_Fnvoid_DuckDB_DuckDB_methods[] = {
	ZEND_ME(Fnvoid_DuckDB_DuckDB, __construct, arginfo_class_Fnvoid_DuckDB_DuckDB___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_DuckDB, query, arginfo_class_Fnvoid_DuckDB_DuckDB_query, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_DuckDB, prepare, arginfo_class_Fnvoid_DuckDB_DuckDB_prepare, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_DuckDB, registerFunction, arginfo_class_Fnvoid_DuckDB_DuckDB_registerFunction, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_DuckDB, createAppender, arginfo_class_Fnvoid_DuckDB_DuckDB_createAppender, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_DuckDB, duckDBVersion, arginfo_class_Fnvoid_DuckDB_DuckDB_duckDBVersion, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_Fnvoid_DuckDB_Result_methods[] = {
	ZEND_ME(Fnvoid_DuckDB_Result, fetchAll, arginfo_class_Fnvoid_DuckDB_Result_fetchAll, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_Result, fetchOne, arginfo_class_Fnvoid_DuckDB_Result_fetchOne, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_Result, iterate, arginfo_class_Fnvoid_DuckDB_Result_iterate, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_Fnvoid_DuckDB_ResultIterator_methods[] = {
	ZEND_ME(Fnvoid_DuckDB_ResultIterator, rewind, arginfo_class_Fnvoid_DuckDB_ResultIterator_rewind, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_ResultIterator, current, arginfo_class_Fnvoid_DuckDB_ResultIterator_current, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_ResultIterator, key, arginfo_class_Fnvoid_DuckDB_ResultIterator_key, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_ResultIterator, next, arginfo_class_Fnvoid_DuckDB_ResultIterator_next, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_ResultIterator, valid, arginfo_class_Fnvoid_DuckDB_ResultIterator_valid, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_Fnvoid_DuckDB_Statement_methods[] = {
	ZEND_ME(Fnvoid_DuckDB_Statement, execute, arginfo_class_Fnvoid_DuckDB_Statement_execute, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_Fnvoid_DuckDB_Appender_methods[] = {
	ZEND_ME(Fnvoid_DuckDB_Appender, appendRow, arginfo_class_Fnvoid_DuckDB_Appender_appendRow, ZEND_ACC_PUBLIC)
	ZEND_ME(Fnvoid_DuckDB_Appender, flush, arginfo_class_Fnvoid_DuckDB_Appender_flush, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_Fnvoid_DuckDB_Exception_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_Fnvoid_DuckDB_DuckDB(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Fnvoid\\DuckDB", "DuckDB", class_Fnvoid_DuckDB_DuckDB_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

static zend_class_entry *register_class_Fnvoid_DuckDB_Result(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Fnvoid\\DuckDB", "Result", class_Fnvoid_DuckDB_Result_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

static zend_class_entry *register_class_Fnvoid_DuckDB_ResultIterator(zend_class_entry *class_entry_Iterator)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Fnvoid\\DuckDB", "ResultIterator", class_Fnvoid_DuckDB_ResultIterator_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 1, class_entry_Iterator);

	return class_entry;
}

static zend_class_entry *register_class_Fnvoid_DuckDB_Statement(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Fnvoid\\DuckDB", "Statement", class_Fnvoid_DuckDB_Statement_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

static zend_class_entry *register_class_Fnvoid_DuckDB_Appender(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Fnvoid\\DuckDB", "Appender", class_Fnvoid_DuckDB_Appender_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

static zend_class_entry *register_class_Fnvoid_DuckDB_Exception(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Fnvoid\\DuckDB", "Exception", class_Fnvoid_DuckDB_Exception_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Exception);

	return class_entry;
}
