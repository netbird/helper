/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: fuyuan <yufuyuan2005@126.com>                                                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_helper.h"

/* If you declare any globals in php_helper.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(helper)
*/

/* True global resources - no need for thread safety here */

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("helper.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_helper_globals, helper_globals)
    STD_PHP_INI_ENTRY("helper.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_helper_globals, helper_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

/**
 * {{{ php_function array_get(array, key, default) }}}
 * 
 **/
PHP_FUNCTION(array_get)
{
	zval *arr; // array
	zend_string* strkey; // key
	zval *defaultval = NULL; // default value
	zval *retval;
	HashTable *arrHashTable;
	zval *dest_entry;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "zS|z", &arr, 
		&strkey, &defaultval) == FAILURE) {
		return;
	}
	if ((retval = zend_hash_find(Z_ARRVAL_P(arr), strkey)) != NULL){
		RETURN_ZVAL(retval, 1, 0);
	} 
	// foreach
	if (zend_memrchr(ZSTR_VAL(strkey), '.', ZSTR_LEN(strkey))) {
		char *entry, *ptr, *seg;
		HashTable *target = Z_ARRVAL_P(arr);
		entry = estrndup(ZSTR_VAL(strkey), ZSTR_LEN(strkey));
		if ((seg = php_strtok_r(entry, ".", &ptr))) {
			do {
				if (target == NULL || (retval = zend_symtable_str_find(target, seg, strlen(seg))) == NULL) {
					break;
				}

				if (Z_TYPE_P(retval) == IS_ARRAY) {
					target = Z_ARRVAL_P(retval);
				} else {
					target = NULL;
				}
			} while ((seg = php_strtok_r(NULL, ".", &ptr)));
		}
		efree(entry);
		if (retval) {
			RETURN_ZVAL(retval, 1, 0);
		}
	}
	// end foreach
	if (defaultval) {
		RETURN_ZVAL(defaultval, 1, 0);
	} else {
		RETURN_NULL();
	}
}

/**{{{  php_function self_concats }}}**/
PHP_FUNCTION(self_concats)
{
	char *str = NULL;
	size_t str_len;
	zend_long n;
	zend_long i;
	char *result; /* point to result*/
	char *ptr;
	size_t result_len;
	/** params */
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sl", &str, &str_len, &n) == FAILURE) {
		return;
	}

	result_len = str_len * n;
	result = (char *)emalloc(result_len + 1);
	ptr = result;

    while(n--) {
		memcpy(ptr, str, str_len);
		ptr += str_len;
	}
	// end flag
	*ptr = '\0';
	RETURN_STRINGL(result, result_len);

}

/* }}} */

/* {{{ php_helper_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_helper_init_globals(zend_helper_globals *helper_globals)
{
	helper_globals->global_value = 0;
	helper_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(helper)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(helper)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(helper)
{
#if defined(COMPILE_DL_HELPER) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(helper)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(helper)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "helper support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ helper_functions[]
 *
 * Every user visible function must have an entry in helper_functions[].
 */
const zend_function_entry helper_functions[] = {
	PHP_FE(self_concats,	NULL)
	PHP_FE(array_get,	NULL)
	PHP_FE_END	/* Must be the last line in helper_functions[] */
};
/* }}} */

/* {{{ helper_module_entry
 */
zend_module_entry helper_module_entry = {
	STANDARD_MODULE_HEADER,
	"helper",
	helper_functions,
	PHP_MINIT(helper),
	PHP_MSHUTDOWN(helper),
	PHP_RINIT(helper),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(helper),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(helper),
	PHP_HELPER_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HELPER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(helper)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
