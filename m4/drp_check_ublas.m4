# Configure paths for Boost uBLAS
# David Pritchard 2002-08-19, based on gtk.m4 by Owen Taylor

dnl DRP_CHECK_UBLAS
AC_DEFUN([DRP_CHECK_UBLAS],
[
  AC_REQUIRE([AC_PATH_X])

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  #
  # Checks for uBLAS
  # 

  AC_CHECK_HEADER(boost/numeric/ublas/matrix.hpp, HAVE_UBLAS=yes, HAVE_UBLAS=no)

  AC_LANG_RESTORE

  AC_SUBST(HAVE_UBLAS)
])
