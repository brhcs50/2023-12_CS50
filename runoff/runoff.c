#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name); // use "strcmp" from hints, like previous assignment
void tabulate(void);                         // count votes
bool print_winner(void);
int find_min(void);      // calculate minimum vote total
bool is_tie(int min);    // using "min", The function should return true if every candidate remaining in the election has the same
                         // number of votes, and should return false otherwise.
void eliminate(int min); // The function should eliminate the candidate (or candidates) who have "min" number of votes.

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
// If the preference is successfully recorded, the function should return true; the function should return false otherwise
// (if, for instance, name is not the name of one of the candidates).
bool vote(int voter, int rank, string name)
{
    // TODO
    //-- compare the candidate name to the ones available, add to votes if a match
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    //-- if not eliminated, loop for i voter_count, and j candidate_count, and add votes to total
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (candidates[preferences[i][j]].eliminated == false) //-- not yet eliminated
            {
                candidates[preferences[i][j]].votes++; //-- so increment votes
                break;                                 //-- have to break here or it only does this one time
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
// If any candidate has more than half of the vote, their name should be printed and the function should return true.
// If nobody has won the election yet, the function should return false.
bool print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > voter_count * .5) //-- if votes are greater than 50%
        {
            printf("%s\n", candidates[i].name);
            return true; //-- return true or it keeps going
        }
    }
    return false; //-- return false means nobody has won, this was already done for me
}

// Return the minimum number of votes any remaining candidate has
// You’ll likely want to loop through the candidates to find the one who is both still in the election and has the fewest number of
// votes. What information should you keep track of as you loop through the candidates? (votes of candidates still in the running,
// ".votes" and ".eliminated")
int find_min(void)
{
    // TODO
    int min = voter_count; //-- initialize min as the total number of voters, everything else coming up will be lower

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes < min &&
            candidates[i].eliminated == false) //-- this person's votes are less than the min, and they are not yet eliminated
        {
            min = candidates[i].votes; //-- the min is now set to how many votes this person has received
        }
    }
    return min; //-- this was set to 0 for me, but doesn't make sense because I want to return the minimum votes
}

// Return true if the election is tied between all candidates, false otherwise
// The function takes an argument min, which will be the minimum number of votes that anyone in the election currently has.
// The function should return true if every candidate remaining in the election has the same number of votes, and should return
// false otherwise.
bool is_tie(int min)
{
    // TODO
    //-- the only way for everyone to have the same number of votes is if they all have the minimum that was calculated in another
    //-- function (find_min)
    //-- need to track how many people are eliminated (out), and also compare that to the overall min vote (votelow)?
    //-- naming is getting confusing so I am naming these difrerently
    int out = 0;
    int votelow = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated) //-- candidateS.eliminated is how many people out of the total, ex: 9 total candidates and 5 are out
        {
            out = out + 1;
        }
        if (candidates[i].votes == min) //-- candidateS.votes and tracking how many people have the same min
        {
            votelow = votelow + 1;
        }
    }
    if (out == votelow) //-- number of eliminated people equals the minimum vote count, so everyone has the same number of minimum votes
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
// The function takes an argument min, which will be the minimum number of votes that anyone in the election currently has.
// The function should eliminate the candidate (or candidates) who have min number of votes.
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
