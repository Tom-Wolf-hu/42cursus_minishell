#!/bin/bash

GREEN='\033[1;32m'
RED='\033[1;31m'
DEFAULT='\033[0m'

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

run_stest() {
	local input="$1"
	local test_name="$2"

	# echo -e "$input" | $RUN_MINISHELL | grep -v "$input" >> $MINISH_OUTPUT 2>&1
	# echo -e "\n---------------------------------------\n" | $RUN_MINISHELL >> $MINISH_OUTPUT 2>&1
	# echo -e "$input" | $RUN_BASH >> $BASH_OUTPUT 2>&1
	# echo -e "\n---------------------------------------\n" | $RUN_BASH >> $BASH_OUTPUT 2>&1

	echo -e "$input" | $RUN_MINISHELL | grep -v ">" > $MINISH_TEMP 2>&1
	# echo -e "$input" | $RUN_MINISHELL > $MINISH_TEMP 2>&1
	echo -e "$input" | $RUN_BASH > $BASH_TEMP 2>&1

	if diff -q $MINISH_TEMP $BASH_TEMP > /dev/null; then
		echo -e "${GREEN}[PASS]$DEFAULT $test_name"
		echo -e "${GREEN}[PASS]$DEFAULT $test_name" >> $CHECK
	else
		echo -e "${RED}[FAIL]$DEFAULT $test_name"
		echo -e "${RED}[FAIL]$DEFAULT $test_name" >> $CHECK
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

run_ktest() {
	local prepare_cmd="$1"
	local check_cmd="$2"
	local test_name="$3"

	# echo -e "$prepare_cmd" | $RUN_MINISHELL | grep -v ">"
	# echo -e "$check_cmd" | $RUN_MINISHELL | grep -v ">" > $MINISH_TEMP 2>&1
	echo -e "$prepare_cmd" | $RUN_MINISHELL | grep -vE '^\s*(minishell)?\s*>'
	echo -e "$check_cmd" | $RUN_MINISHELL | grep -vE '^\s*(minishell)?\s*>' > $MINISH_TEMP 2>&1

	echo -e "$prepare_cmd" | $RUN_BASH
	echo -e "$check_cmd" | $RUN_BASH > $BASH_TEMP 2>&1

	if diff -q $MINISH_TEMP $BASH_TEMP > /dev/null; then
		echo -e "${GREEN}[PASS]$DEFAULT $test_name"
		echo -e "${GREEN}[PASS]$DEFAULT $test_name" >> $CHECK
	else
		echo "${RED}[FAIL]$DEFAULT $test_name"
		echo "${RED}[FAIL]$DEFAULT $test_name" >> $CHECK
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
run_stest "echo HELLO WORLD" "Echo Test"
run_stest "pwd" "Print working directory Test"
run_stest "pwd hello " "PWD Test with more arguments"
run_ktest "env" "env | grep USER" "Environment Test"
run_ktest "cd" "pwd" "Single cd Test"
run_ktest "cd /Users" "pwd" "cd with path Test"
run_ktest "export TEST_VAR=hello" "env | grep TEST_VAR" "Export variable test"
run_ktest "unset TEST_VAR" "env | grep TEST_VAR" "Unset Test"
run_stest "exit" "Exit Test"
echo -e "\n"

echo -e "\nTest external commands" | tee -a $CHECK
run_ktest "ls -l" "ls -l | wc -l" "List Files Test"
run_stest "whoami" "Whoami Test"
echo -e "\n"

echo -e "\nTest pipe commands" | tee -a $CHECK
run_stest "ls -l | wc -l" "ls -l line count Test"
run_stest "echo hello | wc -l" "echo line count Test"
run_stest "ls -l | grep file | sort" "3 command in pipleine Test"
run_stest "cat file3 | grep "helo1" | cut -d ':' -f 3 | sort" "4 command in pipeline Test with external command"
# run_stest 'echo $PATH | tr ":" "\n" | grep "bin" | wc -l' "4 command in pipeline Test with builtin command"
# run_stest "ps aux | grep zsh | awk '{print $5}'" "Print zsh PIDs Test"
# run_stest "ps aux | grep \"zsh\"" "Grep with doublequotes Test" --> these commands not working right way through script

echo -e "\nTest pipe with redirections commands" | tee -a $CHECK
run_stest "cat < check.txt | grep FAIL" "FAIL lines in check.txt Test"
run_ktest "ls | sort > sorted_list.txt" "cat sorted_list.txt" "Sort ls result into txt file Test"
run_ktest "grep FAIL < check.txt | sort > output.txt" "cat output.txt" "Sort from check FAIL lines into output.txt file"
run_ktest "cat file3 | grep day | awk '{print $2}' > file5" "cat file5" "Print second column of day lines from file3 to file5"
run_ktest "sort < file3 | uniq > file5" "cat file5" "Sort file3 lines without duplicate from file3 to file5"

echo -e "\nTest Commands with single quotes" | tee -a $CHECK
run_stest "echo 'hello alex' 'how are you'" "Echo Test with 2 single quoted arguments"
run_stest "echo 'This is the exit status: $?'" "Exit status Test"

echo -e "\nTest Commands with doublequotes" | tee -a $CHECK
run_stest "echo \"hello alex\" \"how are you\"" "Echo Test with 2 doublequoted arguments"
run_stest "grep \"day\" \"file3\"" "Grep Test with 2 doublequoted arguments"
run_stest "echo \"hello alex\" 123 \"how are you\"" "Echo Test with doublequoted and simple argument"
run_stest "echo \"This is the exit status: $?\"" "Exit status Test"
run_stest "\"echo\" hello" "Command in doublequotes Test"

echo -e "\nTest Commands with single and doublequotes" | tee -a $CHECK
run_stest "echo \"Hello '$USER'\"" "Expand in singlequotes Test"
run_stest "echo 'Hello \"$USER\"'" "Expand in doublequotes Test"
run_stest "echo \"hello ' tak\"" "Double and single command Test"
# run_stest "\"cat\" 'file3'" "Command in doublequotes file in singlequotes Test"

echo -e "\nTest pipe with redirections and quotes" | tee -a $CHECK
run_stest "env | grep -E '^(USER|HOME|PWD|SHELL)='" "Environment most common variable Test"
run_stest "cat < file3 | grep 'day'" "Grep with singlequotes Test"
run_stest "yes \"no\" | HEAD -n 10" "Print 10 no Test"
run_stest "cat < file3 | grep \"day\"" "Grep from file with doublequotes"

echo "Cleaning up..."
make -C "$PATH_MINISHELL" fclean

# rm -f $MINISH_TEMP $BASH_TEMP
# rm -f $MINISH_OUTPUT $BASH_OUTPUT $CHECK
rm -f output.txt sorted_list.txt file5