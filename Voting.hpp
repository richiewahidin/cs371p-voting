// -----------
// Voting.hpp
// -----------

#include <cassert> // assert
#include <algorithm>
#include <string>
#include <vector>
#include <iostream> // cin, cout, endl
#include <cstring>
#include <sstream>

#ifndef Voting_hpp
#define Voting_hpp

using namespace std;

class Ballot {
public:
    vector<unsigned> preferences;
    unsigned         current_index;

    Ballot() {
        this->current_index = 0;
    }
};

class Candidate {
public:
    string name;
    vector<Ballot*> candidate_ballots;
    bool out;

    Candidate(string candidate_name) {
        this->name = candidate_name;
        this->out = false;
    }
};

class Election {
public:
    vector<Candidate> candidates;
    vector<Ballot>    ballots;

    void get_names(unsigned num_candidates) {
        string name;
        for (unsigned i = 0; i < num_candidates; ++i) {
            getline(cin, name);
            Candidate temp(name);
            this->candidates.push_back(temp);
        }
        return;
    }

    void get_ballots(unsigned num_candidates) {
        string line;
        while (getline(cin, line) && !line.empty()) {
            int temp_val;
            stringstream ss(line);
            Ballot temp;
            for (unsigned i = 0; i < num_candidates; ++i) {
                ss >> temp_val;
                temp.preferences.push_back(temp_val);
            }
            this->ballots.push_back(temp);
        }
        return;
    }
};
void clear_ballot (Election& curr);
void count_votes (Election& curr);
bool check_winner (Election& curr, int* max_min);
int populate_losers_list (vector<Candidate*>& losers, Election& curr, int min,
                          int max);
bool check_tie( Election& curr, int min, int max);
void shift_current_index(vector<Candidate*>& losers, Election& curr);

#endif