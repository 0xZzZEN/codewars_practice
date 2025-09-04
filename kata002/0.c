#include <stdio.h>
#include <stdint.h>
#include <criterion/criterion.h>
#include <stdlib.h>

void two_oldest_ages(size_t n, const int ages[n], int result[2]);
void tester(size_t n, const int ages[n], const int expected[2]);

int main(int argc, char **argv)
{
    if (argc <)
    // create an array
    uint32_t N 
    const int ages[N]
    two_oldest_ages
    
}

//result is an output buffer which has to be filled with the solution
void two_oldest_ages(size_t n, const int ages[n], int result[2]) {
    s
}

Test(Example_Tests, should_pass_all_the_tests_provided)
{
  {
    const int ages[6] = {1, 5, 87, 45, 8, 8};
    const int oldest[2] = {45, 87};
    tester(6, ages, oldest);
  }
  {
    const int ages[6] = {6, 5, 83, 5, 3, 18};
    const int oldest[2] = {18, 83};
    tester(6, ages, oldest);
  }
  {
    const int ages[2] = {10, 1};
    const int oldest[2] = {1, 10};
    tester(2, ages, oldest);
  }
}
