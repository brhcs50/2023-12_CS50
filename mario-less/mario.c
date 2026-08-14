#include <cs50.h>
#include <stdio.h>

int main(void)
{

    // loop to get input "height" from user, between 1-8
    int height = 0;
    do
    {
        height = get_int("How tall do you want the steps, from 1-8? ");
    }
    while (height < 1 || height > 8);

    // rows: for 1 to height, print horizontal lines
    for (int i = 0; i < height; i++)
    {
        // columns: for 1 to height, this is how many characters on each horizontal
        for (int j = 0; j < height; j++)
        {
            if (i + j < height - 1) // calculate the number of spaces to right-align, then print them
            {
                printf(" ");
            }
            else // print the actual # of the pyramid
            {
                printf("#");
            }
        }
        printf("\n"); // next row
    }
}