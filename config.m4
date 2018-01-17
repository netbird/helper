dnl $Id$
dnl config.m4 for extension helper

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(helper, for helper support,
Make sure that the comment is aligned:
[  --with-helper             Include helper support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(helper, whether to enable helper support,
dnl Make sure that the comment is aligned:
dnl [  --enable-helper           Enable helper support])

if test "$PHP_HELPER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-helper -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/helper.h"  # you most likely want to change this
  dnl if test -r $PHP_HELPER/$SEARCH_FOR; then # path given as parameter
  dnl   HELPER_DIR=$PHP_HELPER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for helper files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       HELPER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$HELPER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the helper distribution])
  dnl fi

  dnl # --with-helper -> add include path
  dnl PHP_ADD_INCLUDE($HELPER_DIR/include)

  dnl # --with-helper -> check for lib and symbol presence
  dnl LIBNAME=helper # you may want to change this
  dnl LIBSYMBOL=helper # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HELPER_DIR/$PHP_LIBDIR, HELPER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_HELPERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong helper lib version or lib not found])
  dnl ],[
  dnl   -L$HELPER_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(HELPER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(helper, helper.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
