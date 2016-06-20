# Configure paths for the GLUI library
# David Pritchard 2002-05-23, based on gtk.m4 by Owen Taylor
#
# Creates shell variables have_GLUI, GLUI_LIBS and GLUI_CFLAGS

AC_DEFUN([DRP_CHECK_GLUI],
[
  AC_REQUIRE([MDL_HAVE_OPENGL])
  GLUI_CFLAGS="$GL_CFLAGS"
  GLUI_LIBS="$GL_LIBS -lglui"

  AC_LANG_SAVE
  GLUI_save_LDFLAGS="$LDFLAGS"
  GLUI_save_CFLAGS="$CFLAGS"
  GLUI_save_LIBS="$LIBS"

  #
  # Checks for GLUI
  # 

  AC_LANG_CPLUSPLUS
  CFLAGS="$GLUI_CFLAGS"
  LIBS="$GLUI_LIBS"

  AC_MSG_CHECKING([for GLUI])

  # Save the "AC_MSG_RESULT file descriptor" to FD 8.
  exec 8>&AC_FD_MSG

  # Temporarily turn off AC_MSG_RESULT so that the user gets pretty
  # messages.
  exec AC_FD_MSG>/dev/null

  AC_CHECK_HEADER(glui.h, have_GLUI=yes, have_GLUI=no)

  if test "$have_GLUI" = "yes" ; then
    AC_CACHE_VAL(ac_cv_link_GLUI,[
      AC_TRY_LINK([#include <glui.h>],[
        float version = GLUI_Master.get_version();
      ],ac_cv_link_GLUI=yes,ac_cv_link_GLUI=no)
    ])
    have_GLUI=$ac_cv_link_GLUI
  fi

  # Restore pretty messages.
  exec AC_FD_MSG>&8

  AC_MSG_RESULT($have_GLUI)

  CFLAGS="$GLUI_save_CFLAGS"
  LIBS="$GLUI_save_LIBS"
  AC_LANG_RESTORE

  AC_SUBST(GLUI_LIBS)
  AC_SUBST(GLUI_CFLAGS)
  AC_SUBST(have_GLUI)
])
