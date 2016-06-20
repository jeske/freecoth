# Configure paths for Matrix Template Library (MTL)
# David Pritchard 2002-05-23, based on gtk.m4 by Owen Taylor

dnl DRP_CHECK_MTL
AC_DEFUN([DRP_CHECK_MTL],
[
  AC_REQUIRE([AC_PATH_X])

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  #
  # Checks for MTL
  # 

  AC_CHECK_HEADER(mtl/mtl.h, HAVE_MTL=yes, HAVE_MTL=no)

  AC_LANG_RESTORE

  AC_SUBST(HAVE_MTL)
])
