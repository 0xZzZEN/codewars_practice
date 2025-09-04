#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <criterion/criterion.h>
#include <inttypes.h>
#include <ctype.h>

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

bool is_valid_ip(const char *addr)
{
    char *token; // create a pointer to the token_buffer

    char str[200]; // create an array for string evaluation
    // copy address to string so that it can be evaluate later
    strcpy(str, addr);
    
    char *endptr; // to keep track of invalid character when converting from token;
    
    int16_t numbers[4] = {0}; // array of presumably octets
    
    for (uint8_t i = 0; i < 4; i++) // loop through string address, split string into tokens with delim '.'
    {
        if (i == 0)
        {
            token = strtok(str, "."); // split addr string into token with delim "."
        }
        else
        {
            token = strtok(NULL, "."); // split the same addr string into token with delim "."
        }

        if ( (token == NULL))
        {
            return false; // if strtok returns token = NULL, the delimiter isn't found => IP invalid
        }

        // if token's length is more than 1 and it has leading zero or it has invalid char as first => invalid IP
        if ( (strlen(token) > 1) && ((token[0] == '0') || (token[0] == '+')) )
        {
            return false;
        }

        if (isspace(token[0]))
        {
            return false;
        }

        // first char of token contains space => invalid IP

        //strtok returns valid pointer

        numbers[i] = strtoumax(token, &endptr, 10); // convert token into numeric value
        
        // check for strtoul errors
        if ((token == endptr) || (*endptr != '\0'))
        {
            return false; // no digits were found OR further characters after number (endptr != '\0') OR the number doesn't match the octet range
        }

        // check the range of number => should be 0-255
        if ((numbers[i] > 255) || (numbers[i] < 0))
        {
            return false;
        }
        
        // i-th convertion was successfull, keep going
        
        printf("strtoul() run %d returns value %d\n", i, numbers[i]); //debug

    }

    // call strtok one more time after loop and if there's one more delim, the IP is invalid (no more than 4 octets)
    char *extra_token = strtok(NULL, ".");
    if ((extra_token != NULL))
    {
        return false;
    }

    return true;
}


// gdb par if using criterion lib
// set follow-fork-mode child