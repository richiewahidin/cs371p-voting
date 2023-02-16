// -----------
// Voting.cpp
// -----------

// --------
// includes
// --------

using namespace std;

#include "Voting.hpp"

// clears all ballots for all candidates at the start
void clear_ballot (Election& curr) {
    for (int i = 0;  i < curr.candidates.size(); i++) {
        curr.candidates[i].candidate_ballots.clear();
    }
    return;
}

// tallies up the votes for each candidate
void count_votes(Election& curr) {
    Candidate* current;
    Ballot* temp;
    for (int i = 0; i < curr.ballots.size(); i++) {
        temp = &curr.ballots[i];
        current = &curr.candidates[temp->preferences[temp->current_index] - 1];
        current->candidate_ballots.push_back(temp);
    }
    return;
}

// populate the losers list with the losers
void populate_losers_list(vector<Candidate*>& losers, Election& curr, int min, 
        int max) {
    for (int i = 0; i < curr.candidates.size(); i++) {
        if (min != max && curr.candidates[i].candidate_ballots.size() == min 
                || curr.candidates[i].candidate_ballots.size() == 0) {
            losers.push_back(&curr.candidates[i]);
            curr.candidates[i].out = true;
        }
    }
}

// return true if tie and print candidate names, return false otherwise
bool check_tie( Election& curr, int min, int max) {
    if (max == min) {
            for (int i = 0; i < curr.candidates.size(); i++) {
                if (!curr.candidates[i].out) {
                cout << curr.candidates[i].name << endl;   
            }
        }
        return true;
    }
    return false;
}

// update the current index, increment if voted for a loser to a contender
void shift_current_index(vector<Candidate*>& losers, Election& curr) {
    for (int i = 0; i < losers.size(); i++) {
        Candidate* current = losers[i];
        for (int i = 0; i < current->candidate_ballots.size(); i++) {
            Ballot* temp = current->candidate_ballots[i];
            while (curr.candidates[temp->preferences[temp->current_index] - 1].out) {
                temp->current_index++;
            } 
        }   
    }
}


