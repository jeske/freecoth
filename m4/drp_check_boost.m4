# Configure paths for Boost (headers only, doesn't test libraries)
# David Pritchard 2002-08-19, based on gtk.m4 by Owen Taylor

dnl DRP_CHECK_BOOST
AC_DEFUN([DRP_CHECK_BOOST],
[
  AC_REQUIRE([AC_PATH_X])

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  #
  # Checks for Boost
  # 

  AC_CHECK_HEADER(boost/ref.hpp, HAVE_BOOST=yes, HAVE_BOOST=no)

  AC_LANG_RESTORE

  AC_SUBST(HAVE_BOOST)
])
