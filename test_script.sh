#!/bin/bash

# test for simple commands
echo hello
echo
env
echo
ls -l
echo
/bin/ls
echo
nonexistent_command
echo
touch test_exec
chmod -x test_exec
./test_exec
rm -f test_exec
echo "hello | <> tamas $USER"
echo 'hello | <> tamas $USER'

# test for complicated input
echo hello | wc -l >test_file1
echo "have a nice day" >>test_file1
cat <test_file1