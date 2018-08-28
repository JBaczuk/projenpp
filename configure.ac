#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([projenpp], [0.1.0], [https://github.com/JBaczuk/projenpp/issues])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Specify the directory of additional local Autoconf macros.
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX

# Require c++11 for all c++ products.
#------------------------------------------------------------------------------
AS_CASE([${CC}], [*],
    [AX_CHECK_COMPILE_FLAG([-std=c++11],
[CXXFLAGS="$CXXFLAGS -std=c++11"])])

# Checks for libraries.
AX_BOOST_BASE([1.47.0])
AX_BOOST_UNIT_TEST_FRAMEWORK
AX_BOOST_PROGRAM_OPTIONS

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])
AC_OUTPUT
