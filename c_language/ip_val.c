#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <criterion/criterion.h>

extern bool is_valid_ip(const char *addr);
static void do_test(const char *addr, bool expected);

Test(test_suite, sample_tests)
{
    do_test("0.0.0.0", true);
    do_test("255.255.255.255", true);
    do_test("0.11.222.11", true);
    do_test("0", false); // not enough bytes
    do_test("1.2.99999999999999999999999999999999.4", false); // integer overflow
    do_test("0.0.0", false); // not enough bytes
    do_test(" 1.2.3.4", false); // leading whitespace
    do_test("1. 2.3.4", false); // middle whitespce
    do_test("1.2.3.4 ", false); // trailing whitspace
    do_test(" ", false);
    do_test("0.0.0.0.0", false); // too many bytes
    do_test("1.2.03.4", false); // leading 0
    do_test("1.2.+3.4", false); // + sign
    do_test("256.256.256.256", false); // byte too large
    do_test("1.2.1000.4", false); // byte too large
    do_test("4.-5.-6.7", false); // negative bytes
    do_test("0.0.0.0abcdefg", false);
    do_test(".1.2.3.4", false); // leading dot
    do_test("0.11.222.11.", false); // trailing dot
    do_test("1.2..4", false); // empty bytes
    do_test("...", false);
    do_test("abcdefg0.0.0.0", false);
    do_test("a.b.c.d", false); // hex digits
}

static void do_test(const char *addr, bool expected)
{
    const char *b[2] = { "false", "true" };
    bool actual = is_valid_ip(addr);
    cr_assert_eq(actual, expected,
        "expected %s but got %s for input \"%s\"",
        b[expected], b[actual], addr);
}

int is_valid_ip(const char *addr)
{
    char *token; // create a pointer to the token_buffer

    char str[16]; // create an array for string evaluation (max is 15 symbols + NUL)
    
    // copy addr string to string so that it can be evaluate later
    strncpy(str, addr, sizeof(str));

    str[sizeof(str) - 1] = '\0'; // add NUL in case sizeof addr exceeds sizeof str

    printf("copied successfully to str: \"%s\"\n", str); // debug copied str
    
    char *endptr; // to keep track of invalid character when converting from token;
    
    uintmax_t number; // variable to check if it's the right octet

    uint8_t numbers_quantity = 0; // numbers counter after token convertion. IPv4
    
    // to find the position of trailing '.'
    uint8_t current_token_length = 0; 
    uint8_t sum_tokens_length = 0;

    if ( ((token = strtok(str, ".")) != NULL) && (str[0] != '.'))
    {
        do
        {
            // strtok returns valid pointer
            // so the strtok changes i-th delimiter to the '\0' to find next token
            // last iteration makes token pointed to the last token and there should be no delim
            // so it happens the delims must be == 3

            // calculate the current token length
            current_token_length = strlen(token);

            // if token's length is more than 1 and it has leading zero => invalid IP
            if (current_token_length > 1 && token[0] == '0')
            {
                return 0;
            }

            // if token has invalid char at the beginning => invalid IP
            if (token[0] == ' ' || token[0] == '+' || token[0] == '.')
            {
                return 0;
            }

            // calculate the sum of already sucessfully returned tokens lengths to find the position of trailing '.'
            sum_tokens_length += current_token_length + 1; // + NUL char

            number = strtoumax(token, &endptr, 10); // convert token into numeric value
            
            // check for strtoul errors
            if (token == endptr || *endptr != '\0')
            {
                return 0; // no digits were found OR further characters after number (endptr != '\0')
            }

            // number is valid, check if it fits in octet

            // check the range of number => should be 0-255 (less than zero handled by strtoumax)
            if (number > 255 || errno == ERANGE)
            {
                return 0;
            }
            
            // i-th convertion was successfull, keep going
            numbers_quantity++;

            printf("strtoul() converted %" PRIu8 " token and returned valid number %ju\n", numbers_quantity, number); //debug
        
        } 
        while ((token = strtok(NULL, ".")) != NULL); // loop through string address, split string into tokens with delim '.'
    }
    else
    {
        return 0; // if strtok returns token = NULL, the delimiter isn't found or if str[0] = '.' => IP invalid
    }

    // limit exceeded, must be 4 digits and 3 delimiters (3 delimiters should be changed to the \0 in the char array and last one is just omitted to the \0)

    // the sum (sum_tokens_length) of already sucessfully returned tokens to find trailing '.'
    if ( (numbers_quantity != 4) || (isalpha(addr[sum_tokens_length - 1])) || isspace(addr[sum_tokens_length - 1]) || ispunct(addr[sum_tokens_length - 1]) )
    {
        return 0;
    }

    // all criterias passed, ip is valid 
    return 1;
}


// gdb par if using criterion lib
// set follow-fork-mode child

/*
i = 0 '\000'
token = 0x7fffffffd701 "1"
str = ".1\000\062.3.4\000cdefg\000\000", '\071' <repeats 20 times>, "\000\064\000\000Ɗ\370\367\377\177\000\000P\327\377\377\377\177\000\000\"\000\000\000\000\000\000\000\v\000\000\000\000\000\000\000\260\303\\UUU", '\000' <repeats 18 times>, "\360E\371\367\377\177\000\000\262\303\\UUU", '\000' <repeats 27 times>, "\260\217\227I\244\067\067\000\000\000\000\000\000\000\000()\372\367\377\177\000\000\340\327\377\377\377\177\000\000\200\331\377\377\377\177\000\000()\372\367\377\177\000\000\b}UUUU\000\000@\320\377\367\377\177\000"
endptr = 0x7fffffffd705 "3.4"
numbers = {0, 0, 0, 0}
extra_token = 0x7fffffffd708 ""


Thread 3.1 "ip_val" hit Breakpoint 1, is_valid_ip (addr=0x5555555562a3 "0.11.222.11.") at ip_val.c:73
73                  token = strtok(NULL, "."); // split the same addr string into token with delim "."
(gdb) info locals
i = 3 '\003'
token = 0x7fffffffd705 "222"
str = "0\000\061\061\000\062\062\062\000\061\061.\000g\000\000", '\071' <repeats 20 times>, "\000\064\000\000Ɗ\370\367\377\177\000\000P\327\377\377\377\177\000\000\"\000\000\000\000\000\000\000\v\000\000\000\000\000\000\000\260\303\\UUU", '\000' <repeats 18 times>, "\360E\371\367\377\177\000\000\262\303\\UUU", '\000' <repeats 27 times>, "\252\006\251\347\035\344\246\000\000\000\000\000\000\000\000()\372\367\377\177\000\000\340\327\377\377\377\177\000\000\200\331\377\377\377\177\000\000()\372\367\377\177\000\000\b}UUUU\000\000@\320\377\367\377\177\000"
endptr = 0x7fffffffd708 ""
numbers = {0, 11, 222, 0}
extra_token = 0x7fffffffd708 ""
(gdb) c
Continuing.


*/

/*
debug without criterion
int main(void)
{
    int result = is_valid_ip("0.11.222.11.");
    char *res = result == 0 ? "False" : "True";
    printf("Result is %s\n", res);
    return 0;
}
*/
