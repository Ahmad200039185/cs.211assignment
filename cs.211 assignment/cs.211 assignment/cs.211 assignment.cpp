#include <iostream>  // Library for input/output
#include <cctype>    // Library for character classification (isdigit, isalpha, etc.)
#include <sstream>   // Library for handling input as a stream
#include <cstring>   // Library for handling strings

using namespace std;

// Global Variables
int charClass;  // Stores the character class (LETTER, DIGIT, UNKNOWN)
char lexeme[100];  // Stores the lexeme being analyzed
char nextChar;  // Stores the current character being processed
int lexLen;  // Length of the lexeme
int nextToken;  // Stores the token type
istringstream inputStream;  // Simulates file input

// Function Declarations
void addChar();
void getChar();
void getNonBlank();
int lex();

// Character Classes
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Token Definitions
#define INT_LIT 10      // Integer literal
#define IDENT 11        // Identifier
#define ASSIGN_OP 20    // Assignment operator (=)
#define ADD_OP 21       // Addition operator (+)
#define SUB_OP 22       // Subtraction operator (-)
#define MULT_OP 23      // Multiplication operator (*)
#define DIV_OP 24       // Division operator (/)
#define LEFT_PAREN 25   // Left parenthesis '('
#define RIGHT_PAREN 26  // Right parenthesis ')'
#define EOF_TOKEN -1    // End of file token

int main() {
    // Define the input string (instead of reading from a file)
    string input = "(sum + 47) / total";
    inputStream.str(input);  // Load input string into stream

    getChar();  // Read the first character

    // Process the input until we reach EOF
    do {
        lex();
    } while (nextToken != EOF_TOKEN);

    return 0;
}

// Function to identify operators and parentheses
int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    default:
        addChar();
        nextToken = EOF_TOKEN;
        break;
    }
    return nextToken;
}

// Function to add a character to the lexeme
void addChar() {
    if (lexLen < 99) {  // Ensure lexeme does not exceed buffer size
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';  // Null-terminate the lexeme
    }
    else {
        cout << "Error - lexeme is too long" << endl;
    }
}

// Function to read the next character and classify it
void getChar() {
    if (inputStream.get(nextChar)) {  // Read next character
        if (isalpha(nextChar))  // If it's a letter
            charClass = LETTER;
        else if (isdigit(nextChar))  // If it's a digit
            charClass = DIGIT;
        else  // Any other character
            charClass = UNKNOWN;
    }
    else {
        charClass = EOF_TOKEN;  // End of input
    }
}

// Function to skip whitespace characters
void getNonBlank() {
    while (isspace(nextChar))  // Keep reading while space, tab, or newline
        getChar();
}

// Lexical analyzer function
int lex() {
    lexLen = 0;  // Reset lexeme length
    getNonBlank();  // Skip spaces

    switch (charClass) {
    case LETTER:  // If the character is a letter, start an identifier
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {  // Identifiers can contain letters & digits
            addChar();
            getChar();
        }
        nextToken = IDENT;
        break;

    case DIGIT:  // If the character is a digit, start an integer literal
        addChar();
        getChar();
        while (charClass == DIGIT) {  // Keep reading if it's a number
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

    case UNKNOWN:  // If it's an operator or parenthesis
        lookup(nextChar);
        getChar();
        break;

    case EOF_TOKEN:  // End of input
        nextToken = EOF_TOKEN;
        strcpy_s(lexeme, sizeof(lexeme), "EOF");  // Store "EOF" in lexeme safely
        break;
    }

    // Print the recognized token and lexeme
    cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
    return nextToken;
}
