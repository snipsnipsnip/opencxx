# ------------------------------------------------------------------------
#@beginlicenses@
#@license{contributors}{2001}@
#@endlicenses@
# ------------------------------------------------------------------------
#

dnl OCC_PROG_BASH
dnl Test for Bourne Again Shell (BASH)
dnl
AC_DEFUN([OCC_PROG_BASH],
[
AC_MSG_CHECKING([for bash])
if bash --version >/dev/null 2>/dev/null; then
    BASH_SH=`bash -ec 'echo $BASH'`
    AC_MSG_RESULT($BASH_SH)
else
    AC_MSG_RESULT("not found")
    BASH_SH=""
fi
])

