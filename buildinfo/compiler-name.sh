cmdname=`echo $0 | sed 's:.*/::'`
if test "x$1" = x; then
    echo "$cmdname: no compiler given" >&2
    exit 1
fi

if test "x$1" = x--help; then
    echo ""
    echo "    USAGE: $cmdname PATH_TO_COMPILER_EXECUTABLE"
    echo ""
    echo "    Recognizes and prints compiler name and version."
    echo ""
    exit
fi

compiler_path=$1
compiler_basename=`echo $compiler_path | sed 's:.*/::'`

case $compiler_basename in
  *g++*|*gcc*) 
      name=gcc
      if $compiler_path --version >/dev/null; then
          version=`$compiler_path --version \
              | sed 's/$/ /g' \
              | awk 'BEGIN{RS=" "}($0 ~ /^[0-9]+[.]/) {print;exit;} {next}'`
      else
          echo "$cmdname: cannot run \`$compiler_path --version'" >&2
          exit 1
      fi
      ;;
  *)
      name=unknown
      version=0.0
      ;;
esac

echo $name-$version
