dnl @synopsis MDL_HAVE_OPENGL
dnl
dnl Search for OpenGL.  We search first for Mesa (a GPL'ed version of
dnl Mesa) before a vendor's version of OpenGL, unless we were
dnl specifically asked not to with `--with-Mesa=no' or `--without-Mesa'.
dnl
dnl The four "standard" OpenGL libraries are searched for: "-lGL",
dnl "-lGLU", "-lGLX" (or "-lMesaGL", "-lMesaGLU" as the case may be) and
dnl "-lglut".
dnl
dnl All of the libraries that are found (since "-lglut" or "-lGLX" might
dnl be missing) are added to the shell output variable "GL_LIBS", along
dnl with any other libraries that are necessary to successfully link an
dnl OpenGL application (e.g. the X11 libraries).  Care has been taken to
dnl make sure that all of the libraries in "GL_LIBS" are listed in the
dnl proper order.
dnl
dnl Additionally, the shell output variable "GL_CFLAGS" is set to any
dnl flags (e.g. "-I" flags) that are necessary to successfully compile
dnl an OpenGL application.
dnl
dnl The following shell variable (which are not output variables) are
dnl also set to either "yes" or "no" (depending on which libraries were
dnl found) to help you determine exactly what was found.
dnl
dnl   have_GL
dnl   have_GLU
dnl   have_GLX
dnl   have_GLUT
dnl
dnl A complete little toy "Automake `make distcheck'" package of how to
dnl use this macro is available at:
dnl
dnl   ftp://ftp.slac.stanford.edu/users/langston/autoconf/ac_opengl-0.01.tar.gz
dnl
dnl Please note that as the ac_opengl macro and the toy example evolves,
dnl the version number increases, so you may have to adjust the above
dnl URL accordingly.
dnl
dnl @author Matthew D. Langston <langston@SLAC.Stanford.EDU>
dnl Substantial revisions by David Pritchard <drpritch@alumni.uwaterloo.ca>

AC_DEFUN([MDL_HAVE_OPENGL],
[
  AC_REQUIRE([AC_PROG_CC])
  AC_REQUIRE([AC_PATH_X])
  AC_REQUIRE([AC_PATH_XTRA])

  dnl Check for Mesa first, unless we were asked not to.
  AC_ARG_ENABLE( Mesa, [Prefer Mesa over native], use_Mesa=$enableval, use_Mesa=yes)

  if test "x$no_x" == "xyes" ; then
    GL_CFLAGS=""
    GL_LIBS="-L/usr/lib/w32api"
    if test x"$use_Mesa" = xyes; then
       GL_search_list="MesaGL   opengl32"
      GLU_search_list="MesaGLU glu32"
      GLX_search_list="MesaGLX"
      GLUT_search_list="glut32"
    else
       GL_search_list="opengl32 MesaGL"
      GLU_search_list="glu32 MesaGLU"
      GLX_search_list="MesaGLX"
      GLUT_search_list="glut32"
    fi
  else
    GL_CFLAGS="$X_CFLAGS"
    GL_X_LIBS="$X_PRE_LIBS $X_LIBS -lX11 -lXext -lXmu -lXt -lXi $X_EXTRA_LIBS"
    GL_LIBS="$GL_X_LIBS"
    if test x"$use_Mesa" = xyes; then
       GL_search_list="MesaGL   GL"
      GLU_search_list="MesaGLU GLU"
      GLX_search_list="MesaGLX GLX"
      GLUT_search_list="glut"
    else
       GL_search_list="GL  MesaGL"
      GLU_search_list="GLU MesaGLU"
      GLX_search_list="GLX MesaGLX"
      GLUT_search_list="glut"
    fi
  fi

  AC_LANG_SAVE
  AC_LANG_C

  GL_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$GL_CFLAGS"

  GL_save_LIBS="$LIBS"
  LIBS="$GL_LIBS"

  AC_MSG_CHECKING([for OpenGL])

  # Save the "AC_MSG_RESULT file descriptor" to FD 8.
  exec 8>&AC_FD_MSG

  # Temporarily turn off AC_MSG_RESULT so that the user gets pretty
  # messages.
  exec AC_FD_MSG>/dev/null

  AC_SEARCH_LIBS(glAccum,          $GL_search_list, have_GL=yes,   have_GL=no)

  # Restore pretty messages.
  exec AC_FD_MSG>&8

  AC_MSG_RESULT($have_GL)

  AC_MSG_CHECKING([for GLU])
  exec 8>&AC_FD_MSG
  exec AC_FD_MSG>/dev/null
  AC_SEARCH_LIBS(gluBeginCurve,   $GLU_search_list, have_GLU=yes,  have_GLU=no)
  exec AC_FD_MSG>&8
  AC_MSG_RESULT($have_GLU)

  AC_MSG_CHECKING([for GLX])
  exec 8>&AC_FD_MSG
  exec AC_FD_MSG>/dev/null
  if test "x$no_x" == "xyes" ; then
      have_GLX=no
  else
      AC_SEARCH_LIBS(glXChooseVisual, $GLX_search_list, have_GLX=yes,  have_GLX=no)
  fi
  exec AC_FD_MSG>&8
  AC_MSG_RESULT($have_GLX)

  AC_MSG_CHECKING([for GLUT])
  exec 8>&AC_FD_MSG
  exec AC_FD_MSG>/dev/null
  AC_SEARCH_LIBS(glutInit,        $GLUT_search_list, have_GLUT=yes, have_GLUT=no)
  exec AC_FD_MSG>&8
  AC_MSG_RESULT($have_GLUT)

  GL_LIBS="$LIBS"

  LIBS="$GL_save_LIBS"
  CPPFLAGS="$GL_save_CPPFLAGS"

  AC_LANG_RESTORE
  GL_X_LIBS=

  AC_SUBST(have_GL)
  AC_SUBST(have_GLU)
  AC_SUBST(have_GLX)
  AC_SUBST(have_GLUT)
  AC_SUBST(GL_CFLAGS)
  AC_SUBST(GL_LIBS)
])
