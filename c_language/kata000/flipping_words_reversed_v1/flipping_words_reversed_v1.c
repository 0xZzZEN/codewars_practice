/*
Write a function that takes in a string of one or more words, and returns the same string, but with all words that have five or more letters reversed (Just like the name of this Kata). 
Strings passed in will consist of only letters and spaces. Spaces will be included only when more than one word is present.

Examples:

"Hey fellow warriors"  --> "Hey wollef sroirraw" 
"This is a test        --> "This is a test" 
"This is another test" --> "This is rehtona test"

*/

//  do not allocate memory for return string
//  assign the value to the pointer "result"

#include <stdio.h>
#include <stdlib.h>

#include <criterion/criterion.h>

#define MAX_WORD_LENGTH 45

void spin_words(const char *sentence, char *result);
void tester(const char *sentence, char *result);
//bool is_word_boundary(char letter);

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: ./flipping_words_reversed sentence \n");
    return 1;
  }

  char result[30 * 14 + 1];
  spin_words(argv[1], (char *)&result);
  
  printf("Original sentence: \"%s\" \n", argv[1]);
  printf("Result: \"%s\" \n", result);
  //tester(argv[1], submitted);
}

void spin_words(const char *sentence, char *result) {
  
  //  <----  hajime!

  // make a new dynamic char array to store current word
  char *reordered_word = malloc(sizeof(char) * MAX_WORD_LENGTH + 1);
  if (reordered_word == NULL)
  {
    printf("Couldn't allocate memory to make reordered_word array. Try to close other programs and try again.\n");
    return;
  }

  // 5 or more letters should be reversed
  int current_letter_position_in_sentence = 0, word_count = 0, start_position_of_the_word = 0;
  
  // do check each letter in sentence until we hit NUl char (end of the sentence):
  while (true)
  {
    // check if letter has ended:
    if ((sentence[current_letter_position_in_sentence] == ' ') || (sentence[current_letter_position_in_sentence] == '.') || (sentence[current_letter_position_in_sentence] == ',') || (sentence[current_letter_position_in_sentence] == '\0'))
    {
      // means we found space or NUL char => end of the word
      //int separator_position = current_letter_position_in_sentence;
      
      // calculate the letters of current word until we hit the SPACE or NUL
      int end_position_of_the_word = current_letter_position_in_sentence - 1; // minus one means without SPACE or NUL char
      int num_of_letters_of_current_word = end_position_of_the_word - start_position_of_the_word + 1; // because count starts from zero

      word_count += 1; // optional to debug

      // for each individual letter we reorganise the positions
      for (int i = 0; i <= num_of_letters_of_current_word; i++)
      {
        if (num_of_letters_of_current_word >= 5)
        {
          // if word has 5 or more letters, we should reverse the order and store it at the new array
          reordered_word[i] = sentence[end_position_of_the_word - i];
        }
        else
        {
          // else we should use the original word from sentence and only place the original order to the new array
          reordered_word[i] = sentence[start_position_of_the_word + i];
        }
        // after ordering steps store a word into a new sentence
        result[start_position_of_the_word + i] = reordered_word[i];
      }
      // loop finished and i = separator

      // so we add separator(ex: space) between words
      if (word_count >= 1)
      {
        result[current_letter_position_in_sentence] = ' ';
      }

      // clear reordered_word array to store next word
      memset(reordered_word, '\0', MAX_WORD_LENGTH * sizeof(char) + 1);

      // add the starting point of the new word
      start_position_of_the_word = current_letter_position_in_sentence + 1;

      // break out of loop if we hit NUL char
      if (sentence[current_letter_position_in_sentence] == '\0')
      {
        // store NUL char at the end of sentence
        result[current_letter_position_in_sentence] = '\0';
        break;
      }
      
      //result[letters_of_current_word + 1] = ' ';
    }

    // count letters in sentence to find separation between each word, starting from 0
    current_letter_position_in_sentence += 1;

  }

  // free allocated memory
  free(reordered_word);
}

/*bool is_word_boundary(char letter)
{
  return letter == ' ' || letter == '.' || letter == ','; //end of the word
}*/

/*
result = malloc(strlen(sentence) + 1);

if letters_of_current_word >= 5:
  strcopy(result, sentence)
*/

Test(Example_Tests, should_pass_all_the_tests_provided)
{
    tester("Welcome",              "emocleW"             );
    tester("spam",                 "spam"                );    
    tester("This is a test",       "This is a test"      );
    tester("This is another test", "This is rehtona test"); 
}

void tester(const char *sentence, char *expected) {
    char submitted[30 * 14 + 1];
    spin_words(sentence, (char *)&submitted);
    if(strcmp(submitted, expected)) {
        cr_assert_fail(
            "Sentence:  \"%s\"\n \nSubmitted: \"%s\"\n \nExpected:  \"%s\"\n \n",
             sentence,             submitted,            expected
        );
    }
    cr_assert(1);
}