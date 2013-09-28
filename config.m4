PHP_ARG_ENABLE(easywsclient,
    [Whether to enable the "easywsclient" extension],
    [  --enable-easywsclient       Enable easywsclient.])

if test $PHP_EASYWSCLIENT != "no"; then
    PHP_REQUIRE_CXX()
    PHP_SUBST(EASYWSCLIENT_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, VEHICLES_SHARED_LIBADD)
    PHP_NEW_EXTENSION(easywsclient, easywsclient.cpp php_easywsclient.cpp, $ext_shared)
fi
