#!/bin/bash

PATH_MINISHELL=./
RUN_MINISHELL=./minishell
RUN_BASH=/bin/bash

MINISH_OUTPUT=minishell_output.txt
BASH_OUTPUT=bash_output.txt

echo "Compiling Minishell..."
make -C "$PATH_MINISHELL"
if [ $? -ne 0 ]; then
	echo "Compilation failed"
	exit 1
fi 
echo "Compilation succesfull."

run_test() {
	local input="$1"
	local test_name="$2"

	echo -e "$input" | $RUN_MINISHELL >> $MINISH_OUTPUT 2>&1
	# echo -e "\n---------------------------------------\n" | $RUN_MINISHELL >> $MINISH_OUTPUT 2>&1
	echo -e "$input" | $RUN_BASH >> $BASH_OUTPUT 2>&1
	# echo -e "\n---------------------------------------\n" | $RUN_BASH >> $BASH_OUTPUT 2>&1

	if diff -q $MINISH_OUTPUT $BASH_OUTPUT > /dev/null; then
		echo "[PASS] $test_name"
	else
		echo "[FAIL] $test_name"
		echo "Your Minishell output:"
		cat $MINISH_OUTPUT
		echo "Expected bash output:"
		cat $BASH_OUTPUT
	fi
}

echo "Test builtin commands"
run_test "echo HELLO WORLD" "Echo Test"
run_test "pwd" "Print working directory Test"
run_test "env" "Environment Test"
run_test "cd" "Single cd Test"
run_test "cd /Users" "cd with path Test"
run_test "export TEST_VAR=hello" "Export variable test"
run_test "env" "Environment Test after export"
run_test "unset TEST_VAR" "Unset Test"
run_test "env" "Environment Test after unset"
run_test "exit" "Exit Test"

echo "Test external commands"
run_test "ls -l" "List Files Test"
run_test "whoami" "Whoami Test"

echo "Cleaning up..."
make -C "$PATH_MINISHELL" fclean

# rm -f $MINISH_OUTPUT $BASH_OUTPUT
