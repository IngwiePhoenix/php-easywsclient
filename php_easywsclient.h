#ifndef PHP_EASYWSCLIENT_H
#define PHP_EASYWSCLIENT_H

#define PHP_EASYWSCLIENT_EXTNAME  "easywsclient"
#define PHP_EASYWSCLIENT_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 

extern "C" {
#include "php.h"
}

extern zend_module_entry easywsclient_module_entry;
#define phpext_easywsclient_ptr &easywsclient_module_entry;

PHP_METHOD(easywsclient, __construct);
PHP_METHOD(easywsclient, create_dummy);
PHP_METHOD(easywsclient, poll);
PHP_METHOD(easywsclient, send);
PHP_METHOD(easywsclient, dispatch);


#endif /* PHP_EASYWSCLIENT_H */