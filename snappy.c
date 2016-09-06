
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_snappy.h"

/* snappy */
#include <snappy-c.h>

static ZEND_FUNCTION(snappy_compress);
static ZEND_FUNCTION(snappy_uncompress);

ZEND_BEGIN_ARG_INFO_EX(arginfo_snappy_compress, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_snappy_uncompress, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

static const zend_function_entry snappy_functions[] = {
    ZEND_FE(snappy_compress, arginfo_snappy_compress)
    ZEND_FE(snappy_uncompress, arginfo_snappy_uncompress)
    ZEND_FE_END
};

PHP_MINFO_FUNCTION(snappy)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "Snappy support", "enabled");
    php_info_print_table_row(2, "Extension Version", SNAPPY_EXT_VERSION);
#ifdef HAVE_LIBSNAPPY
    php_info_print_table_row(2, "Snappy Version", "system library");
#else
    php_info_print_table_row(2, "Snappy Version", SNAPPY_LIB_VERSION);
#endif
    php_info_print_table_end();
}

zend_module_entry snappy_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "snappy",
    snappy_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_MINFO(snappy),
#if ZEND_MODULE_API_NO >= 20010901
    SNAPPY_EXT_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SNAPPY
ZEND_GET_MODULE(snappy)
#endif

#define SNAPPY_BUFFER_SIZE 4096

static ZEND_FUNCTION(snappy_compress)
{
    zval *data;
    char *output;
    size_t output_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "z", &data) == FAILURE) {
        RETURN_FALSE;
    }

    if (Z_TYPE_P(data) != IS_STRING) {
        zend_error(E_WARNING,
                   "snappy_compress : expects parameter to be string.");
        RETURN_FALSE;
    }

    output_len = snappy_max_compressed_length(Z_STRLEN_P(data));
    output = (char *)emalloc(output_len);
    if (!output) {
        zend_error(E_WARNING, "snappy_compress : memory error");
        RETURN_FALSE;
    }


    if (snappy_compress(Z_STRVAL_P(data), Z_STRLEN_P(data),
                        output, &output_len) == SNAPPY_OK) {
#if ZEND_MODULE_API_NO >= 20141001
        RETVAL_STRINGL(output, output_len);
#else
        RETVAL_STRINGL(output, output_len, 1);
#endif
    } else {
        RETVAL_FALSE;
    }

    efree(output);
}

static ZEND_FUNCTION(snappy_uncompress)
{
    zval *data;
    char *output = NULL;
    size_t output_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "z", &data) == FAILURE) {
        RETURN_FALSE;
    }

    if (Z_TYPE_P(data) != IS_STRING) {
        zend_error(E_WARNING,
                   "snappy_uncompress : expects parameter to be string.");
        RETURN_FALSE;
    }

    if (snappy_uncompressed_length(Z_STRVAL_P(data), Z_STRLEN_P(data),
                                   &output_len) != SNAPPY_OK) {
        zend_error(E_WARNING, "snappy_uncompress : output length error");
        RETURN_FALSE;
    }

    output = (char *)emalloc(output_len);
    if (!output) {
        zend_error(E_WARNING, "snappy_uncompress : memory error");
        RETURN_FALSE;
    }

    if (snappy_uncompress(Z_STRVAL_P(data), Z_STRLEN_P(data),
                          output, &output_len) == SNAPPY_OK) {
#if ZEND_MODULE_API_NO >= 20141001
        RETVAL_STRINGL(output, output_len);
#else
        RETVAL_STRINGL(output, output_len, 1);
#endif
    } else {
        zend_error(E_WARNING, "snappy_uncompress : data error");
        RETVAL_FALSE;
    }

    efree(output);
}
