#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// instructions advise to use functions like this,
// but i could not get it working with multiple functions for letters, words, and sentences
// so this is a mix of what i could get working
int count_letters(string text);
// int count_words(string text);
// int count_sentences(string text);


int main(void)
{
    // index = 0.0588 * L - 0.296 * S - 15.8
    // where L is the average number of letters per 100 words in the text,
    // and S is the average number of sentences per 100 words in the text.
    string text1 = get_string("Text: ");

    int letters = count_letters(text1);
    int words = 1;
    int sentences = 0;

for (int i = 0; i < strlen(text1); i++)
    {
        if (isspace(text1[i]))
        {
            words = words + 1;
        }
        else if (ispunct(text1[i]))
        {
            sentences = sentences + 1;
        }
    }

    printf("%i letters\n", letters);
    printf("%i words\n", words);
    printf("%i sentences\n", sentences);

}

int count_letters(string text)
{
    // count all the letters up to the end of the string
    int lettercount = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        // use isalpha from the ctype library to ignore non-letters, as hinted in the instructions
        if (isalpha(text[i]))
        {
            lettercount = lettercount + 1;
        }
    }
    return lettercount;
}
