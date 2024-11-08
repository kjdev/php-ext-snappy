
#ifndef PHP_SNAPPY_H
#define PHP_SNAPPY_H

#define SNAPPY_EXT_VERSION "0.2.2"
#define SNAPPY_LIB_VERSION "1.2.1"

extern zend_module_entry snappy_module_entry;
#define phpext_snappy_ptr &snappy_module_entry

/* Support PHP < 5.3.7 */
#ifndef ZEND_FE_END
#define ZEND_FE_END {NULL, NULL, NULL}
#endif

#ifdef PHP_WIN32
#   define PHP_SNAPPY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define PHP_SNAPPY_API __attribute__ ((visibility("default")))
#else
#   define PHP_SNAPPY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define SNAPPY_G(v) TSRMG(snappy_globals_id, zend_snappy_globals *, v)
#else
#define SNAPPY_G(v) (snappy_globals.v)
#endif

#if ZEND_MODULE_API_NO >= 20190128
#ifndef TSRMLS_CC
#define TSRMLS_CC
#endif
#ifndef TSRMLS_DC
#define TSRMLS_DC
#endif
#endif

#endif  /* PHP_SNAPPY_H */
