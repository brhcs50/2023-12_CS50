#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// -- instructions advise to use these named functions and declare prototypes here
int count_letters(string text);
int count_words(string textor);
int count_sentences(string textmex);

int main(void)
{
    // -- get input from the user
    string text1 = get_string("Text: ");

    // -- declare variables used in functions
    int letters = count_letters(text1);
    int words = count_words(text1);
    int sentences = count_sentences(text1);

    // -- from the instructions:
    // index = 0.0588 * L - 0.296 * S - 15.8
    // where L is the average number of letters per 100 words in the text,
    // and S is the average number of sentences per 100 words in the text.
    float L = letters / (float) words * 100;
    float S = sentences / (float) words * 100;

    // -- using round function from math library, or the grade is 1 too low
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

    // -- commenting out printed results during development
    // printf("%i letters\n", letters);
    // printf("%i words\n", words);
    // printf("%i sentences\n", sentences);
    // printf("L %f\n", L);
    // printf("S %f\n", S);
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

int count_words(string textor)
{
    // count all the letters up to the end of the string
    int wordcount = 1;

    for (int i = 0; i < strlen(textor); i++)
    {
        // use isalpha from the ctype library to ignore non-letters, as hinted in the instructions
        if (isspace(textor[i]))
        {
            wordcount = wordcount + 1;
        }
    }
    return wordcount;
}

int count_sentences(string textmex)
{
    // count all the letters up to the end of the string
    int sentcount = 0;

    for (int i = 0; i < strlen(textmex); i++)
    {
        // use isalpha from the ctype library to ignore non-letters, as hinted in the instructions
        if (textmex[i] == '.' || textmex[i] == '!' || textmex[i] == '?')
        {
            sentcount = sentcount + 1;
        }
    }
    return sentcount;
}
