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
[  --with-snappy-includedir=DIR  snappy header files], no, no)

if test "$PHP_SNAPPY" != "no"; then

  AC_MSG_CHECKING([searching for libsnappy])

  if test "$PHP_SNAPPY_INCLUDEDIR" != "no"; then
    for i in $PHP_SNAPPY_INCLUDEDIR /usr/local /usr; do
      if test -r $i/include/snappy-c.h; then
        LIBSNAPPY_CFLAGS="-I$i/include"
        LIBSNAPPY_LIBDIR="$i/$PHP_LIBDIR"
        AC_MSG_RESULT(found in $i)
        break
      fi
    done
    if test -z "$LIBSNAPPY_LIBDIR"; then
      AC_MSG_RESULT(not found)
      AC_MSG_ERROR(Please reinstall the snappy library distribution)
    fi
    PHP_CHECK_LIBRARY(snappy, snappy_compress,
    [
      PHP_ADD_LIBRARY_WITH_PATH(snappy, $LIBSNAPPY_LIBDIR, SNAPPY_SHARED_LIBADD)
      AC_DEFINE(HAVE_LIBSNAPPY,1,[ ])
    ], [
      AC_MSG_ERROR(could not find usable libsnappy)
    ], [
      -L$LIBSNAPPY_LIBDIR
    ])

    PHP_SUBST(SNAPPY_SHARED_LIBADD)
    PHP_NEW_EXTENSION(snappy, snappy.c, $ext_shared,, $LIBSNAPPY_CFLAGS)
  else
    AC_MSG_RESULT(use bundled version)

  dnl compiler C++:
  PHP_REQUIRE_CXX()

  dnl snappy
  SNAPPY_MAJOR="1"
  SNAPPY_MINOR="1"
  SNAPPY_PATCHLEVEL="7"

  AC_PROG_CXX
  AC_LANG([C++])
  AC_C_BIGENDIAN([AC_DEFINE([SNAPPY_IS_BIG_ENDIAN], [1], [snappy is big endian])])
  AC_CHECK_HEADERS([stdint.h stddef.h sys/mman.h sys/resource.h windows.h byteswap.h sys/byteswap.h sys/endian.h sys/time.h])

  AC_CHECK_FUNC([mmap])

  AC_MSG_CHECKING([if the compiler supports __builtin_expect])
  AC_TRY_COMPILE(, [
    return __builtin_expect(1, 1) ? 1 : 0
  ], [
    snappy_have_builtin_expect=yes
    AC_MSG_RESULT([yes])
  ], [
    snappy_have_builtin_expect=no
    AC_MSG_RESULT([no])
  ])
  if test x$snappy_have_builtin_expect = xyes ; then
    AC_DEFINE([HAVE_BUILTIN_EXPECT], [1], [Define to 1 if the compiler supports __builtin_expect.])
  fi

  AC_MSG_CHECKING([if the compiler supports __builtin_ctzll])
  AC_TRY_COMPILE(, [
    return (__builtin_ctzll(0x100000000LL) == 32) ? 1 : 0
  ], [
    snappy_have_builtin_ctz=yes
    AC_MSG_RESULT([yes])
  ], [
    snappy_have_builtin_ctz=no
    AC_MSG_RESULT([no])
  ])
  if test x$snappy_have_builtin_ctz = xyes ; then
    AC_DEFINE([HAVE_BUILTIN_CTZ], [1], [Define to 1 if the compiler supports __builtin_ctz and friends.])
  fi

  if test "$ac_cv_header_stdint_h" = "yes"; then
    AC_SUBST([ac_cv_have_stdint_h], [1])
    AC_SUBST([HAVE_STDINT_H_01], [1])
  else
    AC_SUBST([ac_cv_have_stdint_h], [0])
    AC_SUBST([HAVE_STDINT_H_01], [0])
  fi
  if test "$ac_cv_header_stddef_h" = "yes"; then
    AC_SUBST([ac_cv_have_stddef_h], [1])
    AC_SUBST([HAVE_STDDEF_H_01], [1])
  else
    AC_SUBST([ac_cv_have_stddef_h], [0])
    AC_SUBST([HAVE_STDDEF_H_01], [0])
  fi
  if test "$ac_cv_header_sys_uio_h" = "yes"; then
    AC_SUBST([ac_cv_have_sys_uio_h], [1])
    AC_SUBST([HAVE_SYS_UIO_H_01], [1])
  else
    AC_SUBST([ac_cv_have_sys_uio_h], [0])
    AC_SUBST([HAVE_SYS_UIO_H_01], [0])
  fi

  AC_SUBST([SNAPPY_MAJOR])
  AC_SUBST([SNAPPY_MINOR])
  AC_SUBST([SNAPPY_PATCHLEVEL])

  dnl Check for stdc++
  LIBNAME=stdc++
  AC_MSG_CHECKING([for stdc++])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE(
  [
    #include <string>
    using namespace std;
  ],[
    string dummy;
  ],[
    AC_MSG_RESULT(yes)
    PHP_ADD_LIBRARY($LIBNAME, , SNAPPY_SHARED_LIBADD)
  ],[
    AC_MSG_ERROR([wrong stdc++ library not found])
  ])
  AC_LANG_RESTORE

  PHP_SUBST(SNAPPY_SHARED_LIBADD)

  dnl Sources
  SNAPPY_SOURCES="snappy/snappy-c.cc snappy/snappy.cc snappy/snappy-stubs-internal.cc snappy/snappy-sinksource.cc"

  PHP_NEW_EXTENSION(snappy, snappy.c $SNAPPY_SOURCES, $ext_shared)

  if test -f "$ext_srcdir/snappy/snappy-stubs-public.h.in"; then
    if test "$SNAPPY_PATCHLEVEL" -ge 7; then
      mv $ext_srcdir/snappy/snappy-stubs-public.h.in \
      $ext_srcdir/snappy/snappy-stubs-public.h.in.orig

      sed 's/${\(HAVE_[[A-Z\_]]*_H_01\)}/@\1@/' \
      $ext_srcdir/snappy/snappy-stubs-public.h.in.orig > \
      $ext_srcdir/snappy/snappy-stubs-public.h.in
    fi
  fi
  AC_CONFIG_FILES([$ext_srcdir/snappy/snappy-stubs-public.h])
  AC_OUTPUT
  PHP_ADD_BUILD_DIR($ext_builddir/snappy, 1)
  PHP_ADD_INCLUDE([$ext_srcdir/snappy])
  fi
fi
