#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int startsize = 0;
    do
    {
        startsize = get_int("Enter the starting population size: ");
    }
    while (startsize < 9);
    // TODO: Prompt for end size
    int endsize = 0;
    do
    {
        endsize = get_int("Enter the ending population size: ");
    }
    while (endsize < startsize);
    // TODO: Calculate number of years until we reach threshold
    if (startsize < endsize)
    {
        int years = 0;
        do
        {
            startsize = startsize + (startsize / 3) - (startsize / 4);
            years++;
        }
        while (startsize < endsize);
        // TODO: Print number of years
        printf("Years: %i\n", years);
    }
    else if (startsize == endsize)
    {
        printf("Years: 0\n");
    }
}