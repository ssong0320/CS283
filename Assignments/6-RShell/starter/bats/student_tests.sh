#!/usr/bin/env bats

# File: student_tests.sh

@test "Basic: ls command should run without errors" {
    run ./dsh <<EOF                
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Basic: Running hello world should succeed" {
    run ./dsh <<EOF
echo "Hello, world!"
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="Hello,world!localmodedsh4>dsh4>cmdloopreturned0"

    echo "Output: $output"
    echo "Exit Status: $status"
    echo "$stripped_output -> $expected_output"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Pipes: ls bats | grep student_tests.sh" {
    run ./dsh <<EOF
ls bats | grep student_tests.sh
EOF

    [[ "$output" == *"student_tests.sh"* ]]

    echo "Output: $output"
    echo "Exit Status: $status"
}

@test "Built-in: Change directory to /tmp and verify" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF

    [[ "$output" == *"/tmp"* ]]

    echo "Output: $output"
    echo "Exit Status: $status"
}

@test "Invalid: Running non-existing command should fail" {
    run ./dsh <<EOF
non_existing_command
EOF

    echo "Output: $output"
    echo "Exit Status: $status"

    [[ "$output" == *"execvp failed"* ]]
}


@test "Exit: Ensure shell exits gracefully" {
    run ./dsh <<EOF
exit
EOF

    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$status" -eq 0 ]
}

@test "Redirection: echo output to file and verify" {
    run ./dsh <<EOF
echo "hello" > testfile.txt
cat testfile.txt
EOF

    [[ "$output" == *"hello"* ]]

    echo "Output: $output"
    echo "Exit Status: $status"
}

@test "Pipes: ps aux | grep dsh" {
    run ./dsh <<EOF
ps aux | grep dsh
EOF

    [[ "$output" == *"dsh"* ]]

    echo "Output: $output"
    echo "Exit Status: $status"
}

@test "Background Process: Run sleep in background" {
    run ./dsh <<EOF
sleep 5 &
EOF

    [[ "$output" == *"&"* ]]

    echo "Output: $output"
    echo "Exit Status: $status"
}

@test "Multiple Arguments: echo with multiple words" {
    run ./dsh <<EOF
echo Hello world from dsh
EOF

    [[ "$output" == *"Hello world from dsh"* ]]

    echo "Output: $output"
    echo "Exit Status: $status"
}


@test "Background Process: Ensure it runs and doesn't block" {
    run ./dsh <<EOF
sleep 3 &
echo "Background process started"
EOF

    [[ "$output" == *"Background process started"* ]]

    echo "Output: $output"
    echo "Exit Status: $status"
}

@test "Input Redirection: Use file as input" {
    echo "input text" > input.txt

    run ./dsh <<EOF
cat < input.txt
EOF

    [[ "$output" == *"input text"* ]]

    echo "Output: $output"
    echo "Exit Status: $status"
}
