#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // -- convert argv from string to an int, using recommended atoi
    int k = atoi(argv[1]);

    // -- get input from the user
    string plain = get_string("plaintext: ");
    printf("ciphertext: ");

    // -- use j for second loop
    // -- printing out letters one at a time as they are encoded
    // -- remove ascii encoding first, do the calculation, then add ascii back
    for (int j = 0; j < strlen(plain); j++)
    {
        // -- calculate uppercase, print one char at a time
        if (isupper(plain[j]))
        {
            printf("%c", (plain[j] - 65 + k) % 26 + 65);
        }
        // -- calculate lowercase, print one char at a time
        else if (islower(plain[j]))
        {
            printf("%c", (plain[j] - 97 + k) % 26 + 97);
        }
        // -- ignore calculating anything not upper or lower
        else
        {
            printf("%c", plain[j]);
        }
    }
    printf("\n");
}
