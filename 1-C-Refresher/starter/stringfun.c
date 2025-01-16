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


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *input_ptr = user_str;    //pointer to traverse input string
    char *buffer_ptr = buff;       //pointer for buffer
    int characters_copied = 0;     //track # of valid characters
    int is_whitespace = 0;         //track whitespace

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

    // fill the remaining buffer with periods
    while (characters_copied < len) {
        *buffer_ptr = '.';
        buffer_ptr++;
        characters_copied++;
    }

    return characters_copied;  // total characters written to buffer
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    char *ptr = buff;
    int wc = 0;
    bool word_started = false;
    int i = 0;

    while (i < str_len) {
        if (*ptr != ' ') {
            if (!word_started) {
                wc++;
                word_started = true;
            }
        } else {
            word_started = false;
        }

        ptr++;
        i++;
    }

    return wc;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

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
        exit(99);
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
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE