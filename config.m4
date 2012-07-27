dnl config.m4 for extension snappy

dnl Check PHP version:
AC_MSG_CHECKING(PHP version)
if test ! -z "$phpincludedir"; then
    PHP_VERSION=`grep 'PHP_VERSION ' $phpincludedir/main/php_version.h | sed -e 's/.*"\([[0-9\.]]*\)".*/\1/g' 2>/dev/null`
elif test ! -z "$PHP_CONFIG"; then
    PHP_VERSION=`$PHP_CONFIG --version 2>/dev/null`
fi

if test x"$PHP_VERSION" = "x"; then
    AC_MSG_WARN([none])
else
    PHP_MAJOR_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\1/g' 2>/dev/null`
    PHP_MINOR_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\2/g' 2>/dev/null`
    PHP_RELEASE_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\3/g' 2>/dev/null`
    AC_MSG_RESULT([$PHP_VERSION])
fi

if test $PHP_MAJOR_VERSION -lt 5; then
    AC_MSG_ERROR([need at least PHP 5 or newer])
fi

PHP_ARG_ENABLE(snappy, whether to enable snappy support,
[  --enable-snappy           Enable snappy support])

PHP_ARG_WITH(snappy-includedir, for snappy header,
[  --with-snappy-includedir=DIR  snappy header files], yes)

if test "$PHP_SNAPPY" != "no"; then
  dnl # check with-path

  if test "$PHP_SNAPPY_INCLUDEDIR" != "no" && test "$PHP_SNAPPY_INCLUDEDIR" != "yes"; then
    if test -r "$PHP_SNAPPY_INCLUDEDIR/snappy.h"; then
      SNAPPY_DIR="$PHP_SNAPPY_INCLUDEDIR"
    else
      AC_MSG_ERROR([Can't find snappy headers under "$PHP_SNAPPY_INCLUDEDIR"])
    fi
  else
    SEARCH_PATH="/usr/local /usr"
    SEARCH_FOR="/include/snappy.h"
    if test -r $PHP_SNAPPY/$SEARCH_FOR; then # path given as parameter
      SNAPPY_DIR="$PHP_SNAPPY/include"
    else # search default path list
      AC_MSG_CHECKING([for snappy files in default path])
      for i in $SEARCH_PATH ; do
        if test -r $i/$SEARCH_FOR; then
          SNAPPY_DIR="$i/include"
          AC_MSG_RESULT(found in $i)
        fi
      done
    fi
  fi

  if test -z "$SNAPPY_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Can't find snappy headers])
  fi

  LIBNAME=snappy
  AC_MSG_CHECKING([for snappy])
  AC_TRY_COMPILE(
  [
    #include "$SNAPPY_DIR/snappy-c.h"
  ],[
    snappy_max_compressed_length(1);
  ],[
    AC_MSG_RESULT(yes)
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SNAPPY_DIR/lib, SNAPPY_SHARED_LIBADD)
    AC_DEFINE(HAVE_SNAPPYLIB,1,[ ])
  ],[
    AC_MSG_RESULT([error])
    AC_MSG_ERROR([wrong snappy lib version or lib not found : $SNAPPY_DIR])
  ])

dnl  PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SNAPPY_DIR/lib, SNAPPY_SHARED_LIBADD)

  PHP_SUBST(SNAPPY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(snappy, snappy.c, $ext_shared)

  ifdef([PHP_INSTALL_HEADERS],
  [
    PHP_INSTALL_HEADERS([ext/snappy/], [php_snappy.h])
  ], [
    PHP_ADD_MAKEFILE_FRAGMENT
  ])
fi
