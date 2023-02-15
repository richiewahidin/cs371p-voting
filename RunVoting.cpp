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

        const int num_votes_to_win = (curr.ballots.size() + 1)/ 2;
        bool finished = false;

        while (!finished) {
            
            for (int i = 0;  i < curr.candidates.size(); i++) {
                    curr.candidates[i].num_votes = 0;
            }
            
            // count votes
            Candidate* current;
            for (int i = 0; i < curr.ballots.size(); i++) {
                Ballot temp = curr.ballots[i];
                current = &curr.candidates[temp.preferences[temp.current_index] - 1];
                current->num_votes++;

                // found a winner
                if (current->num_votes >= num_votes_to_win) {
                    finished = true;
                    cout << current->name << endl;
                }
            }

            // check for a tie
            if (!finished) {
                bool tie = true;
                int tie_vote = curr.candidates[0].num_votes;
                int index = 1;
                while (tie && index < curr.candidates.size()) {
                    tie = tie_vote == curr.candidates[index].num_votes;
                    index++;
                }

                // reached a tie, print all names
                if (tie) {
                    for (int i = 0; i < curr.candidates.size(); i++) {
                        cout << curr.candidates[i].name << endl;
                    }
                }
                finished = tie;
            }

            // need to go check the next preference for those who voted for the losers
            if (!finished) {
                vector<Candidate*> losers;
                int min_votes = 1000;

                // populating losers list
                for (int i = 0; i < curr.candidates.size(); i++) {
                    if (curr.candidates[i].num_votes == min_votes) {
                        losers.push_back(&curr.candidates[i]);
                     
                    } else if (curr.candidates[i].num_votes < min_votes) {
                        losers.clear();
                        losers.push_back(&curr.candidates[i]);
                        min_votes = curr.candidates[i].num_votes;
                    }
                }
    
                // update the current index, increment if voted for a loser
                for (int i = 0; i < curr.ballots.size(); i++) {
                    Ballot *temp = &curr.ballots[i];
                    Candidate* current = &curr.candidates[temp->preferences[temp->current_index]-1];
                    for (int i = 0; i < losers.size(); i++) {
                        if (losers[i]->name == current->name) {
                            temp->current_index++;
                        }
                    }
                }
            }
        }
    }
}
