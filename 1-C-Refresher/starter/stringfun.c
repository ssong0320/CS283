#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *buff, int user_str_len);
void print_words(char *buff, int user_str_len);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *input_ptr = user_str;    //pointer to traverse input string
    char *buffer_ptr = buff;       //pointer for buffer
    int characters_copied = 0;     //track # of valid characters
    int is_whitespace = 0;         //track whitespace

    // Skip leading spaces
    while (*input_ptr == ' ' || *input_ptr == '\t') {
        input_ptr++;
    }

    while (*input_ptr != '\0') {
        // check if buffer is full
        if (characters_copied >= len) {
            return -1;
        }

        // Handle whitespace
        if (*input_ptr == ' ' || *input_ptr == '\t') {
            if (!is_whitespace) {
                *buffer_ptr = ' ';  // add single space
                buffer_ptr++;
                characters_copied++;
                is_whitespace = 1;  // mark that whitespace has been added
            }
        } else {
            *buffer_ptr = *input_ptr;  // copy non-whitespace character
            buffer_ptr++;
            characters_copied++;
            is_whitespace = 0;        // reset is_whitespace
        }

        input_ptr++;  // move to the next character in the input string
    }

    // Remove trailing space
    if (characters_copied > 0 && *(buffer_ptr - 1) == ' ') {
        buffer_ptr--;
        characters_copied--;
    }

    // fill the remaining buffer with periods
    while (characters_copied < len) {
        *buffer_ptr = '.';
        buffer_ptr++;
        characters_copied++;
    }

    return characters_copied;  // total characters written to buffer
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    char *ptr = buff;  // pointer to traverse uff
    int wc = 0;
    bool word_started = false;  //track if in word
    int i = 0;  // loop counter

    while (i < str_len) {
        // Check if the current character is part of a word
        if (*ptr != ' ' && *ptr != '.') {
            if (!word_started) {
                wc++;
                word_started = true;
            }
        } else {
            // Mark that we are outside a word
            word_started = false;
        }

        ptr++;
        i++;
    }

    return wc;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *buff, int user_str_len) {
    char *start = buff;
    char *end = buff + user_str_len - 1;
    char temp;

    // Reverse the user string in place
    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    // Print only the reversed user string
    printf("Reversed String: ");
    for (int i = 0; i < user_str_len; i++) {
        if (buff[i] != '.') {
            putchar(buff[i]);
        }
    }
    putchar('\n');
}


void print_words(char *buff, int user_str_len) {
    printf("Word Print\n");
    printf("----------\n");

    char *ptr = buff;  // Pointer to traverse the buff
    int word_index = 1;  // Track the index of each word
    int word_length = 0;  // Track the length of the current word
    bool in_word = false;  // Track if we are inside a word

    for (int i = 0; i < user_str_len; i++) {
        // Check if the current character is part of a word
        if (*ptr != ' ' && *ptr != '.') {
            if (!in_word) {
                printf("%d. ", word_index++);
                in_word = true;
            }
            putchar(*ptr);
            word_length++;
        } else {
            // If the current word ends, print its length
            if (in_word) {
                printf("(%d)\n", word_length);
                word_length = 0;
                in_word = false;
            }
        }
        ptr++;
    }

    // Print the last word if the string does not end with a space
    if (in_word) {
        printf("(%d)\n", word_length);
    }

    printf("\nNumber of words returned: %d\n", word_index-1);
}


int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //  This is safe becuase it ensures that there are at least 2 arguments so that when argv[1] is accessed,
    //  there exists a value, and won't result in undefined behavior.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //  Ensures that the input has at least 3 arguments which is necessary for all commands
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    buff = (char *)malloc(BUFFER_SZ);
    if (buff == NULL) {
        exit(3);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, user_str_len);
            break;

        case 'w':
            print_words(buff, user_str_len);
            break;

        case 'x':
            if (argc != 5) {
                usage(argv[0]);
                exit(1);

            }
            printf("Not Implemented!\n");
            exit(2);

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE

//Providing both the buffer pointer and its length ensures the functions
//operate only on the intended portion of the buffer, 
//preventing overflows and undefined behavior. It also makes the code 
//more modular and adaptable to changes in buffer size or partial operations.