#!/usr/bin/env bats

# File: student_tests.sh
# Custom unit test suite to verify dsh functionality

@test "Shell starts and exits properly" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Execute basic command - ls" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Execute external command - uname" {
    run ./dsh <<EOF
uname
EOF
    [ "$status" -eq 0 ]
}

@test "Execute echo command with multiple words" {
    run ./dsh <<EOF
echo Hello World
EOF

    stripped_output=$(echo "$output" | grep -v "dsh2>" | grep -v "cmd loop returned" | tail -n 1)
    expected_output="Hello World"
    [ "$stripped_output" = "$expected_output" ]
}

@test "Handle quoted spaces in echo" {
    run ./dsh <<EOF
echo " hello     world     "
EOF

    stripped_output=$(echo "$output" | grep -v "dsh2>" | grep -v "cmd loop returned" | tail -n 1)
    expected_output=" hello     world     "
    [ "$stripped_output" = "$expected_output" ]
}

@test "Change directory to an existing directory" {
    mkdir -p /tmp/dsh-test

    run ./dsh <<EOF
cd /tmp/dsh-test
pwd
EOF

    stripped_output=$(echo "$output" | grep -v "dsh2>" | grep -v "cmd loop returned" | tail -n 1)
    expected_output="/tmp/dsh-test"

    echo "Captured stdout: $output"
    echo "Processed Output: ${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]

    rm -rf /tmp/dsh-test
}

@test "Attempt to change to a non-existent directory" {
    run ./dsh <<EOF
cd /tmp/does_not_exist
EOF

    stripped_output=$(echo "$output" | grep "cd failed" | tail -n 1)
    expected_output="cd failed: No such file or directory"

    echo "Captured stdout: $output"
    echo "Processed Output: ${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "Execute a non-existent command" {
    run ./dsh <<EOF
fakecommand
EOF

    stripped_output=$(echo "$output" | grep "fork error" | tail -n 1)
    expected_output="fork error: No such file or directory"

    echo "Captured stdout: $output"
    echo "Processed Output: ${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "Handle unexpected characters" {
    run ./dsh <<EOF
@#%$
EOF

    stripped_output=$(echo "$output" | grep "fork error" | tail -n 1)
    expected_output="fork error: No such file or directory"

    echo "Captured stdout: $output"
    echo "Processed Output: ${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

