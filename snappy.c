
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#if PHP_MAJOR_VERSION >= 7 && defined(HAVE_APCU_SUPPORT)
#include "ext/standard/php_var.h"
#include "ext/apcu/apc_serializer.h"
#include "zend_smart_str.h"
#endif
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

#if PHP_MAJOR_VERSION >= 7 && defined(HAVE_APCU_SUPPORT)
static int APC_SERIALIZER_NAME(snappy)(APC_SERIALIZER_ARGS);
static int APC_UNSERIALIZER_NAME(snappy)(APC_UNSERIALIZER_ARGS);
#endif

static const zend_function_entry snappy_functions[] = {
    ZEND_FE(snappy_compress, arginfo_snappy_compress)
    ZEND_FE(snappy_uncompress, arginfo_snappy_uncompress)
    ZEND_FE_END
};

PHP_MINIT_FUNCTION(snappy)
{
#if PHP_MAJOR_VERSION >= 7 && defined(HAVE_APCU_SUPPORT)
    apc_register_serializer("snappy",
                            APC_SERIALIZER_NAME(snappy),
                            APC_UNSERIALIZER_NAME(snappy),
                            NULL);
#endif
    return SUCCESS;
}

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
#if PHP_MAJOR_VERSION >= 7 && defined(HAVE_APCU_SUPPORT)
    php_info_print_table_row(2, "Snappy APCu serializer ABI",
                             APC_SERIALIZER_ABI);
#endif
    php_info_print_table_end();
}

#if PHP_MAJOR_VERSION >= 7 && defined(HAVE_APCU_SUPPORT)
static const zend_module_dep snappy_module_deps[] = {
    ZEND_MOD_OPTIONAL("apcu")
    ZEND_MOD_END
};
#endif

zend_module_entry snappy_module_entry = {
#if PHP_MAJOR_VERSION >= 7 && defined(HAVE_APCU_SUPPORT)
    STANDARD_MODULE_HEADER_EX,
    NULL,
    snappy_module_deps,
#elif ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "snappy",
    snappy_functions,
    PHP_MINIT(snappy),
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

static int php_snappy_compress(const char* in, const size_t in_len,
                               char** out, size_t* out_len)
{
    *out_len = snappy_max_compressed_length(in_len);
    *out = (char*)emalloc(*out_len);
    if (!*out) {
        zend_error(E_WARNING, "snappy_compress : memory error");
        *out_len = 0;
        return FAILURE;
    }

    if (snappy_compress(in, in_len, *out, out_len) != SNAPPY_OK) {
        zend_error(E_WARNING, "snappy_compress : data error");
        efree(*out);
        *out = NULL;
        *out_len = 0;
        return FAILURE;
    }

    return SUCCESS;
}

static int php_snappy_uncompress(const char* in, const size_t in_len,
                                 char** out, size_t* out_len)
{
    if (snappy_uncompressed_length(in, in_len, out_len) != SNAPPY_OK) {
        zend_error(E_WARNING, "snappy_uncompress : output length error");
        return FAILURE;
    }

    *out = (char*)emalloc(*out_len);
    if (!*out) {
        zend_error(E_WARNING, "snappy_uncompress : memory error");
        *out_len = 0;
        return FAILURE;
    }

    if (snappy_uncompress(in, in_len, *out, out_len) != SNAPPY_OK) {
        zend_error(E_WARNING, "snappy_uncompress : data error");
        efree(*out);
        *out = NULL;
        *out_len = 0;
        return FAILURE;
    }

    return SUCCESS;
}

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

    if (php_snappy_compress(Z_STRVAL_P(data), Z_STRLEN_P(data),
                            &output, &output_len) == FAILURE) {
        RETURN_FALSE;
    }

#if ZEND_MODULE_API_NO >= 20141001
    RETVAL_STRINGL(output, output_len);
#else
    RETVAL_STRINGL(output, output_len, 1);
#endif

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

    if (php_snappy_uncompress(Z_STRVAL_P(data), Z_STRLEN_P(data),
                              &output, &output_len) == FAILURE) {
        RETURN_FALSE;
    }

#if ZEND_MODULE_API_NO >= 20141001
    RETVAL_STRINGL(output, output_len);
#else
    RETVAL_STRINGL(output, output_len, 1);
#endif

    efree(output);
}

#if PHP_MAJOR_VERSION >= 7 && defined(HAVE_APCU_SUPPORT)
static int APC_SERIALIZER_NAME(snappy)(APC_SERIALIZER_ARGS)
{
    int result;
    php_serialize_data_t var_hash;
    smart_str var = {0};

    BG(serialize_lock)++;
    PHP_VAR_SERIALIZE_INIT(var_hash);
    php_var_serialize(&var, (zval*) value, &var_hash);
    PHP_VAR_SERIALIZE_DESTROY(var_hash);
    BG(serialize_lock)--;

    if (EG(exception)) {
        smart_str_free(&var);
        var.s = NULL;
    }

    if (var.s == NULL) {
        return 0;
    }

    if (php_snappy_compress(ZSTR_VAL(var.s), ZSTR_LEN(var.s),
                            (char **)buf, buf_len) == SUCCESS) {
        result = 1;
    } else {
        result = 0;
    }

    smart_str_free(&var);

    return result;
}

static int APC_UNSERIALIZER_NAME(snappy)(APC_UNSERIALIZER_ARGS)
{
    const unsigned char* tmp;
    int result;
    php_unserialize_data_t var_hash;
    size_t var_len;
    unsigned char* var;

    if (php_snappy_uncompress(buf, buf_len,
                              (char **)&var, &var_len) != SUCCESS) {
        ZVAL_NULL(value);
        return 0;
    }

    BG(serialize_lock)++;
    PHP_VAR_UNSERIALIZE_INIT(var_hash);
    tmp = var;
    result = php_var_unserialize(value, &tmp, var + var_len, &var_hash);
    PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
    BG(serialize_lock)--;

    if (!result) {
        php_error_docref(NULL, E_NOTICE,
                         "Error at offset %ld of %ld bytes",
                         (zend_long)(tmp - var), (zend_long)var_len);
        ZVAL_NULL(value);
        result = 0;
    } else {
        result = 1;
    }

    efree(var);

    return result;
}
#endif
