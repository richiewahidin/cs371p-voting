// --------------
// RunVoting.cpp
// --------------

// --------
// includes
// --------

#include "Voting.hpp"

using namespace std;

// ----
// main
// ----

int main () {
    unsigned num_elections;
    unsigned num_candidates;
    cin >> num_elections;

    // repeat for the number of elections held
    while (num_elections--) {

        Election curr;
        cin >> num_candidates;
        string name;
        getline(cin, name);
        curr.get_names(num_candidates);
        curr.get_ballots(num_candidates);

        bool finished = false;

        while (!finished) {
            
            clear_ballot(curr);
            count_votes(curr);

            // max and min number of votes by all the candidates (excluding zero)
            int max_min[2] = {0, 1000};
            int& max = max_min[0];
            int& min = max_min[1];

            finished = check_winner(curr, max_min);
                
            if (!finished) {
                vector<Candidate*> losers;
                populate_losers_list(losers, curr, min, max);

                // check tie after losers are appropriately updated
                finished = check_tie(curr, min, max);

                // shift the current index of ballots for the losers
                shift_current_index(losers, curr);
            }
        }
        if (num_elections) {
            cout << endl;
        }
    }
}