#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([1.5])
AC_INIT([fractal-spheres], [0.1], [ibaylov@gmail.com])
AC_CONFIG_SRCDIR([src/geom-decorator.hh])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])


# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_LIB( [m], [sqrt])
AC_CHECK_LIB( [GL], [glPushMatrix])
AC_CHECK_LIB( [GLU], [gluSphere])
AC_CHECK_LIB( [glut], [glutInit])
AC_CHECK_FUNCS([sqrt glPushMatrix gluSphere glutInit])

AC_OUTPUT([ Makefile src/Makefile ])
