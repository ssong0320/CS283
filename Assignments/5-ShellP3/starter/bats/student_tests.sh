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
    
    result=$(echo "$output" | grep "dshlib.c" | head -n1)  # Extract only the relevant line
    echo "Extracted result: $result"
    [[ "$result" =~ "dshlib.c" ]]
}

@test "Check if echo hello | wc -w outputs 1" {
    run ./dsh <<EOF
echo hello | wc -w
EOF
    [ "$status" -eq 0 ]

    result=$(echo "$output" | awk '/^[0-9]+$/{print $1}' | head -n1)
    echo "Extracted result: $result"
    [ "$result" -eq 1 ]
}

@test "Check if ls | grep .c | wc -l works with multiple pipes" {
    run ./dsh <<EOF
ls | grep .c | wc -l
EOF
    [ "$status" -eq 0 ]

    result=$(echo "$output" | grep -Eo '^[0-9]+$' | head -n1)
    echo "Extracted result: $result"
    [[ "$result" =~ ^[0-9]+$ ]]
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

    result=$(echo "$output" | grep -E '^/' | head -n1)
    echo "Extracted result: $result"
    [[ "$result" == "/tmp" ]]
}

@test "Check if exit terminates the shell correctly" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]

    result=$(echo "$output" | grep "cmd loop returned" | head -n1)
    echo "Extracted result: $result"
    [[ "$result" =~ "cmd loop returned" ]]
}

@test "Check if echo prints the correct message" {
    run ./dsh <<EOF
echo Hello, world!
EOF
    [ "$status" -eq 0 ]

    result=$(echo "$output" | grep "Hello, world!" | head -n1)
    echo "Extracted result: $result"
    [[ "$result" == "Hello, world!" ]]
}

@test "Check if multiple commands execute in sequence" {
    run ./dsh <<EOF
echo first
echo second
EOF
    [ "$status" -eq 0 ]

    result=$(echo "$output" | grep -E "first|second" | wc -l)
    echo "Extracted result: $result"
    [ "$result" -eq 2 ]  # Expecting two lines of output
}


@test "Check if output redirection works" {
    run ./dsh <<EOF
echo "File test" > test_output.txt
cat test_output.txt
EOF
    [ "$status" -eq 0 ]

    result=$(echo "$output" | grep "File test" | head -n1)
    echo "Extracted result: $result"
    [[ "$result" == "File test" ]]
}


@test "Check if shell handles empty input" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}
