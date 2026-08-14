#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    // vote takes a single argument, a string called name, representing the name of the candidate who was voted for.
    // -- make a loop to compare the name input to the names in candidate[i].name
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            // If name matches one of the names of the candidates in the election, then update that candidate’s vote total to
            // account for the new vote. The vote function in this case should return true to indicate a successful ballot.
            // -- add a vote to candidate[i].votes, return true
            candidates[i].votes++;
            // --for testing: printf("%i\n", candidates[i].votes);
            return true;
        }
    }
    // If name does not match the name of any of the candidates in the election, no vote totals should change,
    // and the vote function should return false to indicate an invalid ballot.
    // You may assume that no two candidates will have the same name.
    // -- return false if candidate[i].name not found
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    // The function should print out the name of the candidate who received the most votes in the election, and then print a
    // newline. It is possible that the election could end in a tie if multiple candidates each have the maximum number of votes. In
    // that case, you should output the names of each of the winning candidates, each on a separate line.
    // -- need a variable to track highest vote count, then compare it to the candidates (2 for loops needed?)
    int topvotes = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (topvotes < candidates[i].votes)
        {
            topvotes = candidates[i].votes;
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == topvotes)
        {
            printf("%s\n", candidates[i].name);
        }
    }

    return;
}
