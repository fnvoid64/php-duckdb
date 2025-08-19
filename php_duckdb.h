/* duckdb extension for PHP */

#ifndef PHP_DUCKDB_H
# define PHP_DUCKDB_H

extern zend_module_entry duckdb_module_entry;
# define phpext_duckdb_ptr &duckdb_module_entry

# define PHP_DUCKDB_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_DUCKDB)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_DUCKDB_H */
