#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    //
    // compare the 2 scores, ex: if word1 > word2, printf("Player 1  Wins!), also do for player2, tie
    //
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    //
    // points[0] is the value of A
    // loop through each of the letters and add up the score, word1 for player1 and word2 for player2
    // while word[i] != '\0' do the adding (can do with strlen)
    // convert uppers to lower so U&L letter values are equal ex: if its between ascii 65-90 then add 32 (use isupper/islower)
    // get the number value of each char in the string
    //
    int score = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        if islower (word[i])
        {
            score = score + POINTS[word[i] - 97];
        }
        if isupper (word[i])
        {
            score = score + POINTS[word[i] - 65];
        }
    }
    return score;
}
