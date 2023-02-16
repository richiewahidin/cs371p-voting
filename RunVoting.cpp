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

        // number of votes to be at > 50%
        const int num_votes_to_win = curr.ballots.size() / 2 + 1;

        bool finished = false;

        while (!finished) {
            
            clear_ballot(curr);
            count_votes(curr);

            int max = 0;
            int min = 1000;
            Candidate* current;

            for (int i = 0;  i < curr.candidates.size(); i++) {
                current = &curr.candidates[i];
                int size = current->candidate_ballots.size();
                assert (size <= curr.ballots.size());

                // found winner
                if (size >= num_votes_to_win) {
                    cout << current->name << endl;
                    finished = true;

                } else {
                    // get max and min
                    if (size > max) {
                        max = size;
                    } if (size < min && size != 0) {
                        min = size;
                    }
                } 
            }            

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