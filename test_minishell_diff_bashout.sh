#!/bin/bash

PATH_MINISHELL=./
RUN_MINISHELL=./minishell
RUN_BASH=/bin/bash

MINISH_TEMP=minish_temp.txt
BASH_TEMP=bash_temp.txt
MINISH_OUTPUT=minishell_output.txt
BASH_OUTPUT=bash_output.txt
CHECK=check.txt

echo "Compiling Minishell..."
make -C "$PATH_MINISHELL"
if [ $? -ne 0 ]; then
	echo "Compilation failed"
	exit 1
fi 
echo "Compilation succesfull."

> $MINISH_OUTPUT
> $BASH_OUTPUT
> $CHECK

run_test() {
	local input="$1"
	local test_name="$2"

	# echo -e "$input" | $RUN_MINISHELL | grep -v "$input" >> $MINISH_OUTPUT 2>&1
	# echo -e "\n---------------------------------------\n" | $RUN_MINISHELL >> $MINISH_OUTPUT 2>&1
	# echo -e "$input" | $RUN_BASH >> $BASH_OUTPUT 2>&1
	# echo -e "\n---------------------------------------\n" | $RUN_BASH >> $BASH_OUTPUT 2>&1

	echo -e "$input" | $RUN_MINISHELL | grep -v "$input" > $MINISH_TEMP 2>&1
	echo -e "$input" | $RUN_BASH > $BASH_TEMP 2>&1

	if diff -q $MINISH_TEMP $BASH_TEMP > /dev/null; then
		echo "[PASS] $test_name"
		echo "[PASS] $test_name" >> $CHECK
	else
		echo "[FAIL] $test_name"
		echo "[FAIL] $test_name" >> $CHECK
		echo "Your Minishell output:"
		echo -e "$test_name\n" >> $MINISH_OUTPUT
		cat $MINISH_TEMP
		cat $MINISH_TEMP >> $MINISH_OUTPUT
		echo -e "\n--------------------------------\n" >> $MINISH_OUTPUT
		echo "Expected bash output:"
		echo -e "$test_name\n" >> $BASH_OUTPUT
		cat $BASH_TEMP
		cat $BASH_TEMP >> $BASH_OUTPUT
		echo -e "\n--------------------------------\n" >> $BASH_OUTPUT
	fi
}

echo "Test builtin commands" | tee -a $CHECK
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
# echo -e "\n"

echo -e "\nTest external commands" | tee -a $CHECK
run_test "ls -l" "List Files Test"
run_test "whoami" "Whoami Test"
# echo -e "\n"

echo -e "\nTest pipe commands" | tee -a $CHECK
run_test "ls -l | wc -l" "ls -l line count Test"
run_test "echo hello | wc -l" "echo line count Test"
# run_test "ps aux | grep zsh | awk '{print $2}'" "Print zsh PIDs Test"
# run_test "ps aux | grep \"zsh\"" "Grep with doublequotes Test"

# echo -e "\nTest pipe with redirections commands" | tee -a $CHECK
# run_test "cat < check.txt | grep FAIL" "FAIL lines in check.txt Test"
# run_test "ls | sort > sorted_list.txt" "Sort ls result into txt file Test"
# run_test "grep FAIL < check.txt | sort > output.txt" "Sort from check FAIL lines into output.txt file"

echo "Cleaning up..."
make -C "$PATH_MINISHELL" fclean

rm -f $MINISH_TEMP $BASH_TEMP
rm -f $MINISH_OUTPUT $BASH_OUTPUT $CHECK
rm -f output.txt sorted_list.txt
