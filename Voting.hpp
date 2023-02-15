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

using std::vector;
using std::string;

class Candidate {
    public:
        string name;
        int num_votes;

        Candidate(string candidate_name) {
            this->name = candidate_name;
            this->num_votes = 0;
        }
};

class Ballot {
    public: 
        vector<unsigned> preferences;
        unsigned         current_index;

        Ballot() {
            this->current_index = 0;
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


#endif