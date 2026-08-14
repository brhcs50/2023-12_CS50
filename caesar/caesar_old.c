#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool only_digits(string digstring);

int main(int argc, string argv[])
{
    //
    // argv[1] for the key
    //

    if (argc < 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else if (argc > 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        // -- get input from the user
        string text1 = get_string("plaintext: ");
        printf("You typed: %s\n", text1);
        return 0;
    }

    bool tempresult = only_digits(argv[1]);
    printf("%i\n", tempresult);
}

bool only_digits(string digstring)
{
    for (int i = 0; i < strlen(digstring); i++)
    {
        if (!isdigit(digstring[i]))
        {
        printf("Usage: ./caesar key\n");
        return 1;
        }
    }
}
