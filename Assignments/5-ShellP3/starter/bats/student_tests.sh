#!/usr/bin/env bats

# File: student_tests.sh
# 
# Custom test suite for dsh

#########################################
#  Basic command execution tests
#########################################
@test "Check if ls runs without errors" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Check if pwd prints a valid directory" {
    run ./dsh <<EOF
pwd
EOF
    [ "$status" -eq 0 ]
    
    result=$(echo "$output" | grep -E '^/' | head -n1)
    echo "Extracted result: $result"  # Debugging info
    [[ "$result" =~ ^/ ]]
}

@test "Check if whoami runs correctly" {
    run ./dsh <<EOF
whoami
EOF
    [ "$status" -eq 0 ]
}

@test "Check if ls | grep dshlib.c returns correct output" {
    run ./dsh <<EOF
ls | grep dshlib.c
EOF
    [ "$status" -eq 0 ]
    
    result=$(echo "$output" | grep "dshlib.c" | tail -n1)
    echo "Extracted result: $result"  # Debugging info
    [[ "$result" =~ "dshlib.c" ]]
}

@test "Check if echo hello | wc -w outputs 1" {
    run ./dsh <<EOF
echo hello | wc -w
EOF
    [ "$status" -eq 0 ]
    
    result=$(echo "$output" | awk '/^[0-9]+$/{print $1}' | tail -n1)
    echo "Extracted result: $result"  # Debugging info
    [ "$result" -eq 1 ]
}

@test "Check if ls | grep .c | wc -l works with multiple pipes" {
    run ./dsh <<EOF
ls | grep .c | wc -l
EOF
    [ "$status" -eq 0 ]
    [[ "$(echo "$output" | tail -n1 | awk '{print $NF}')" =~ ^[0-9]+$ ]]
}

@test "Check if invalid command returns an error" {
    run ./dsh <<EOF
invalidcommand
EOF
    [[ "$output" =~ "execvp failed" ]]  # Ensure the error message appears
}

@test "Check if cd changes directory successfully" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF
    [ "$status" -eq 0 ]
    
    result=$(echo "$output" | grep -E '^/' | tail -n1)
    echo "Extracted result: $result"  # Debugging info
    [[ "$result" == "/tmp" ]]
}

@test "Check if exit terminates the shell correctly" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}
