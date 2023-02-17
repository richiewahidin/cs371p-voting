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

// check for winner and also getting min and max number of votes
bool check_winner (Election& curr, int* max_min) {
    Candidate* current;

    // number of votes to be at > 50%
    const int num_votes_to_win = curr.ballots.size() / 2 + 1;

    for (int i = 0;  i < curr.candidates.size(); i++) {
        current = &curr.candidates[i];
        int size = current->candidate_ballots.size();
        assert (size <= curr.ballots.size());

        // found winner
        if (size >= num_votes_to_win) {
            cout << current->name << endl;
            return true;

        } else {
            // get max and min
            if (size > max_min[0]) {
                max_min[0] = size;
            } if (size < max_min[1] && size != 0) {
                max_min[1] = size;
            }
        } 
    }

    return false;     
}


// populate the losers list with the losers
int populate_losers_list(vector<Candidate*>& losers, Election& curr, int min, 
        int max) {
    for (int i = 0; i < curr.candidates.size(); i++) {

        // if min == max, means its a draw, don't want to add those to losers
        if (min != max && curr.candidates[i].candidate_ballots.size() == min 
                || curr.candidates[i].candidate_ballots.size() == 0) {
            losers.push_back(&curr.candidates[i]);
            curr.candidates[i].out = true;
        }
    }
    return losers.size();
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

            // do in a while loop to ensure that current_index points to an 
            // eligible candidate
            while (curr.candidates[temp->preferences[temp->current_index] - 1].out) {
                temp->current_index++;
            } 
        }   
    }
}


