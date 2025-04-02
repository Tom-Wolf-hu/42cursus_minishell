#!/bin/bash

# test for simple commands
# echo hello
# echo
# env
# echo
# ls -l
# echo
# /bin/ls
# echo
# nonexistent_command
# echo
# touch test_exec
# chmod -x test_exec
# ./test_exec
# echo "hello | <> tamas $USER"
# echo 'hello | <> tamas $USER'

# test for complicated input
echo hello | wc -l >test_file1
echo "have a nice day" >>test_file1
cat <test_file1
echo
echo "hello $USER '$PATH'"
echo 'hello $USER "$PATH"'
echo "ls -l >file1 < cat -e"
# ls -l >test_file2 < cat -e
# ls -l >test_file2 | cat -e >>test_file1
echo $ User
echo $USER
echo $uSER
echo $ USER
# | wc -l >>test_file1



# remove test files
# rm -f test_exec
# rm -f test_file1
# rm -f test_file2