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

    while (num_elections--) {

        Election curr;
        cin >> num_candidates;
        string name;
        getline(cin, name);
        curr.get_names(num_candidates);
        curr.get_ballots(num_candidates);

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

                // found winner
                if (current->candidate_ballots.size() >= num_votes_to_win) {
                    cout << current->name << endl;
                    finished = true;

                } else {
                    if (current->candidate_ballots.size() > max) {
                        max = current->candidate_ballots.size();
                    } if (current->candidate_ballots.size() < min && 
                            current->candidate_ballots.size() != 0) {
                        min = current->candidate_ballots.size();
                    }
                } 
            }            

            // need to go check the next preference for those who voted for the losers
            if (!finished) {
                vector<Candidate*> losers;
                populate_losers_list(losers, curr, min, max);
                finished = check_tie(curr, min, max);
                shift_current_index(losers, curr);
            }
        }
        if (num_elections) {
            cout << endl;
        }
    }
}