/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 93bef6f7976810fd512979b354240e833d084214 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_DuckDB_DuckDB___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, db_file, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_DuckDB_DuckDB_query, 0, 1, DuckDB\\Result, 1)
	ZEND_ARG_TYPE_INFO(0, sql, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_DuckDB_DuckDB_prepare, 0, 1, DuckDB\\Statement, 1)
	ZEND_ARG_TYPE_INFO(0, sql, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_DuckDB_DuckDB_registerFunction, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, func, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_DuckDB_Result_fetchAll, 0, 0, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, assoc, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_DuckDB_Result_fetchOne, 0, 0, MAY_BE_OBJECT|MAY_BE_ARRAY|MAY_BE_NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, assoc, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_DuckDB_Result_iterate, 0, 0, DuckDB\\ResultIterator, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, assoc, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_DuckDB_ResultIterator_rewind, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_DuckDB_ResultIterator_current, 0, 0, MAY_BE_ARRAY|MAY_BE_OBJECT|MAY_BE_NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_DuckDB_ResultIterator_key, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_DuckDB_ResultIterator_next arginfo_class_DuckDB_ResultIterator_rewind

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_DuckDB_ResultIterator_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_DuckDB_Statement_execute, 0, 1, DuckDB\\Result, 1)
	ZEND_ARG_TYPE_INFO(0, values, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(DuckDB_DuckDB, __construct);
ZEND_METHOD(DuckDB_DuckDB, query);
ZEND_METHOD(DuckDB_DuckDB, prepare);
ZEND_METHOD(DuckDB_DuckDB, registerFunction);
ZEND_METHOD(DuckDB_Result, fetchAll);
ZEND_METHOD(DuckDB_Result, fetchOne);
ZEND_METHOD(DuckDB_Result, iterate);
ZEND_METHOD(DuckDB_ResultIterator, rewind);
ZEND_METHOD(DuckDB_ResultIterator, current);
ZEND_METHOD(DuckDB_ResultIterator, key);
ZEND_METHOD(DuckDB_ResultIterator, next);
ZEND_METHOD(DuckDB_ResultIterator, valid);
ZEND_METHOD(DuckDB_Statement, execute);


static const zend_function_entry class_DuckDB_DuckDB_methods[] = {
	ZEND_ME(DuckDB_DuckDB, __construct, arginfo_class_DuckDB_DuckDB___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_DuckDB, query, arginfo_class_DuckDB_DuckDB_query, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_DuckDB, prepare, arginfo_class_DuckDB_DuckDB_prepare, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_DuckDB, registerFunction, arginfo_class_DuckDB_DuckDB_registerFunction, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_DuckDB_Result_methods[] = {
	ZEND_ME(DuckDB_Result, fetchAll, arginfo_class_DuckDB_Result_fetchAll, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_Result, fetchOne, arginfo_class_DuckDB_Result_fetchOne, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_Result, iterate, arginfo_class_DuckDB_Result_iterate, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_DuckDB_ResultIterator_methods[] = {
	ZEND_ME(DuckDB_ResultIterator, rewind, arginfo_class_DuckDB_ResultIterator_rewind, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_ResultIterator, current, arginfo_class_DuckDB_ResultIterator_current, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_ResultIterator, key, arginfo_class_DuckDB_ResultIterator_key, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_ResultIterator, next, arginfo_class_DuckDB_ResultIterator_next, ZEND_ACC_PUBLIC)
	ZEND_ME(DuckDB_ResultIterator, valid, arginfo_class_DuckDB_ResultIterator_valid, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_DuckDB_Statement_methods[] = {
	ZEND_ME(DuckDB_Statement, execute, arginfo_class_DuckDB_Statement_execute, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_DuckDB_Exception_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_DuckDB_DuckDB(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "DuckDB", "DuckDB", class_DuckDB_DuckDB_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

static zend_class_entry *register_class_DuckDB_Result(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "DuckDB", "Result", class_DuckDB_Result_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

static zend_class_entry *register_class_DuckDB_ResultIterator(zend_class_entry *class_entry_Iterator)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "DuckDB", "ResultIterator", class_DuckDB_ResultIterator_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 1, class_entry_Iterator);

	return class_entry;
}

static zend_class_entry *register_class_DuckDB_Statement(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "DuckDB", "Statement", class_DuckDB_Statement_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

static zend_class_entry *register_class_DuckDB_Exception(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "DuckDB", "Exception", class_DuckDB_Exception_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Exception);

	return class_entry;
}
