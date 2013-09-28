#include "php_easywsclient.h"
#include "easywsclient.hpp"

#define PHP_EASYWSCLIENT_RES_NAME "WebSocket Connection"

zend_object_handlers easywsclient_object_handlers;
int le_easywsclient;

struct easywsclient_object {
  zend_object std;
  easywsclient::WebSocket* Easywsclient;
};

zend_class_entry *easywsclient_ce;

void easywsclient_free_storage(void* object TSRMLS_DC) {
    easywsclient_object* obj = (easywsclient_object*)object;
    delete obj->Easywsclient;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value easywsclient_create_handler(zend_class_entry* type TSRMLS_DC) {
    zval* tmp;
    zend_object_value retval;

    easywsclient_object* obj = (easywsclient_object*)emalloc(sizeof(easywsclient_object));
    memset(obj, 0, sizeof(easywsclient_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

#if PHP_VERSION_ID < 50399	
	zend_hash_copy(obj->std.properties, &(type->default_properties), (copy_ctor_func_t)zval_add_ref, (void *)(&tmp), sizeof(zval *));
#else
	object_properties_init(&(obj->std), type);
#endif

    retval.handle = zend_objects_store_put(obj, NULL,
        easywsclient_free_storage, NULL TSRMLS_CC);
    retval.handlers = &easywsclient_object_handlers;

    return retval;
}

// this only happens after PHP 5.4... remember to change zend_function_entry to function_entry or use #if
zend_function_entry easywsclient_methods[] = {
    PHP_ME(easywsclient,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(easywsclient,  create_dummy,    NULL, ZEND_ACC_PUBLIC)
    PHP_ME(easywsclient,  poll,            NULL, ZEND_ACC_PUBLIC)
    PHP_ME(easywsclient,  send,            NULL, ZEND_ACC_PUBLIC)
    PHP_ME(easywsclient,  dispatch,        NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// The actual methods...
PHP_METHOD(easywsclient, __construct) {
	// return easywsclient::WebSocket::from_url(std::string url);
	char *url;
    int url_len;
    zval *res;
	zval *object = getThis();
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &url, &url_len) == FAILURE) {
        RETURN_NULL();
    }
    
    // Casting char -> string
    easywsclient::WebSocket::pointer ws = easywsclient::WebSocket::from_url((std::string)url);
    ZEND_REGISTER_RESOURCE(res, ws, le_easywsclient);
        
    // Build the object
    object_init(return_value);
    zend_update_property_stringl(easywsclient_ce, getThis(), "url", strlen("url"), url, url_len TSRMLS_CC);
    // Add resource
    //zend_update_property_zval(easywsclient_ce, getThis(), "ws", strlen("ws"), ws TSRMLS_CC);
    zval_copy_ctor(return_value);
}
PHP_METHOD(easywsclient, create_dummy) {
}
PHP_METHOD(easywsclient, poll) {
}
PHP_METHOD(easywsclient, send) {
}
PHP_METHOD(easywsclient, dispatch) {
    zval *callback;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &callback) == FAILURE) {
        RETURN_NULL();
    }
    *return_value = *callback;
    zval_copy_ctor(return_value);
}

PHP_MINIT_FUNCTION(easywsclient)
{	
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "easywsclient", easywsclient_methods);
    easywsclient_ce = zend_register_internal_class(&ce TSRMLS_CC);
    
    // Build the basic class
    zend_declare_property_string(easywsclient_ce, "url", strlen("url"), "", ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_class_constant_string(easywsclient_ce, "CLOSED", strlen("CLOSED"), "Yep." TSRMLS_CC);

	// Resource stuff
	le_easywsclient = zend_register_list_destructors_ex(NULL, NULL, PHP_EASYWSCLIENT_RES_NAME, module_number);
	
    return SUCCESS;
}

zend_module_entry easywsclient_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_EASYWSCLIENT_EXTNAME,
    NULL,        /* Functions */
    PHP_MINIT(easywsclient),        /* MINIT */
    NULL,        /* MSHUTDOWN */
    NULL,        /* RINIT */
    NULL,        /* RSHUTDOWN */
    NULL,        /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_EASYWSCLIENT_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_EASYWSCLIENT
extern "C" {
ZEND_GET_MODULE(easywsclient)
}
#endif
