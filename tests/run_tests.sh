#!/bin/bash
# Run from the PROJECT ROOT (where get_next_line.c is):
#     bash tests/run_tests.sh
#     VALGRIND=1 bash tests/run_tests.sh        # also run every build under valgrind
#     bash tests/run_tests.sh 1 42              # only these BUFFER_SIZE values
#
# Your sources are compiled exactly like the evaluators do (-Wall -Wextra -Werror),
# once per BUFFER_SIZE value, plus once WITHOUT the -D flag.

SRCS="get_next_line.c get_next_line_utils.c"
TEST="tests/gnl_test.c"
SIZES="${@:-1 2 3 5 10 42 9999 10000000 default}"
BUILD="$(mktemp -d /tmp/gnl_build_XXXXXX)"
status=0

for f in $SRCS $TEST get_next_line.h; do
	if [ ! -f "$f" ]; then
		echo "missing file: $f (run this script from the project root)"
		exit 2
	fi
done

# ---- sanity checks on your files (the usual causes of build errors) ----
pre=0
for f in $SRCS get_next_line.h; do
	if grep -nE '^(int|void)[[:space:]]+main[[:space:]]*\(' "$f" >/dev/null; then
		echo "[KO] $f contains a main() function:"
		grep -nE '^(int|void)[[:space:]]+main[[:space:]]*\(' "$f" | sed 's/^/       line /'
		echo "     -> remove it (and any debug code): the evaluators bring their own main()"
		pre=1
	fi
done
if ! grep -qE 'get_next_line[[:space:]]*\([[:space:]]*int' get_next_line.h; then
	echo "[KO] get_next_line.h does not declare:  char *get_next_line(int fd);"
	echo "     -> add the prototype inside the include guard (and outside the BUFFER_SIZE #ifndef block)"
	pre=1
fi
if [ "$pre" -ne 0 ]; then
	echo
	echo "Fix the problems above, then run the tests again."
	exit 2
fi

for bs in $SIZES; do
	flag=""
	[ "$bs" != "default" ] && flag="-D BUFFER_SIZE=$bs"
	objs=""
	echo "----- BUFFER_SIZE=$bs : compiling"
	for src in $SRCS; do
		obj="$BUILD/${src%.c}_$bs.o"
		if ! cc -Wall -Wextra -Werror $flag -I. -c "$src" -o "$obj"; then
			echo "[KO] $src does not compile with the project flags"
			status=1
			continue 2
		fi
		objs="$objs $obj"
	done
	if ! cc -Wall -Wextra $flag -I. -c "$TEST" -o "$BUILD/test_$bs.o" ||
	   ! cc $objs "$BUILD/test_$bs.o" -o "$BUILD/gnl_test_$bs"; then
		echo "[KO] could not build the test program"
		status=1
		continue
	fi
	if [ -n "$VALGRIND" ]; then
		run="valgrind --leak-check=full --error-exitcode=42 -q"
	else
		run=""
	fi
	if command -v timeout >/dev/null; then
		run="timeout 120 $run"
	fi
	$run "$BUILD/gnl_test_$bs" | grep -E "^\[KO\]|^     ->|RESULT"
	rc=${PIPESTATUS[0]}
	if [ "$rc" -eq 124 ]; then
		echo "[KO] BUFFER_SIZE=$bs : timeout (infinite loop or far too slow?)"
		status=1
	elif [ "$rc" -eq 139 ] || [ "$rc" -eq 134 ] || [ "$rc" -eq 135 ]; then
		echo "[KO] BUFFER_SIZE=$bs : the program CRASHED (exit code $rc)"
		status=1
	elif [ "$rc" -eq 42 ]; then
		echo "[KO] BUFFER_SIZE=$bs : valgrind found errors or leaks"
		status=1
	elif [ "$rc" -ne 0 ]; then
		status=1
	fi
done

rm -rf "$BUILD"
echo
if [ "$status" -eq 0 ]; then
	echo "ALL BUILDS PASSED"
else
	echo "SOME TESTS FAILED (see [KO] lines above; run ./gnl_test for the full list)"
fi
exit $status