#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	string words[] = {"battleship", "boot", "cannon", "iron", "thimble", "top hat"};

	string s = get_string("String: ");	// enter a string, like “iron”
	for (int i = 0; i < 6; i++)		// 6 positions in the array, 0-5
	{
		if (strcmp(words[i], s) == 0)
		// strcmp from strings.h compares 2 strings and returns 0 if equal,
		// in this case comparing “words[i]” to the input string “s”
		{
			printf("Found\n");
			return 0;	// 0 usually means success
		}
	}
	printf("Not found\n");
}
