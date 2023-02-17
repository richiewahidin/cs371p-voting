// ---------------
// TestVoting.cpp
// ---------------

// https://github.com/google/googletest
// https://github.com/google/googletest/blob/master/docs/reference/assertions.md

// --------
// includes
// --------

#include "gtest/gtest.h"

#include "Voting.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

Election test1;
int num_candidates;
int max_min[2] = {0, 1000};
vector<Candidate*> losers;

TEST(VotingFixture, get_names_0) {
    ifstream in("test.txt");
    cin.rdbuf(in.rdbuf()); //redirect std::cin to test.txt!
    cin >> num_candidates;
    string name;
    getline(cin, name);
    test1.get_names(num_candidates);
    test1.get_ballots(num_candidates);
    char* c = const_cast<char*>(test1.candidates[0].name.c_str());
    ASSERT_STREQ(c, "Alpha");
}

TEST(VotingFixture, get_names_1) {
    char* c = const_cast<char*>(test1.candidates[3].name.c_str());
    ASSERT_STREQ(c, "Delta");
}

TEST(VotingFixture, get_ballots_0) {
    ASSERT_EQ(test1.ballots[0].preferences[0], 1);
}

TEST(VotingFixture, get_ballots_1) {
    ASSERT_EQ(test1.ballots[3].preferences[2], 4);
}

TEST(VotingFixture, get_ballots_2) {
    ASSERT_EQ(test1.ballots[5].preferences[1], 1);
}

TEST(VotingFixture, clear_ballots_0) {
    clear_ballot(test1);
    ASSERT_EQ(test1.candidates[0].candidate_ballots.size(), 0);
    ASSERT_EQ(test1.candidates[1].candidate_ballots.size(), 0);
    ASSERT_EQ(test1.candidates[2].candidate_ballots.size(), 0);
    ASSERT_EQ(test1.candidates[3].candidate_ballots.size(), 0);
}

TEST(VotingFixture, count_votes_0) {
    count_votes(test1);
    ASSERT_EQ(test1.candidates[0].candidate_ballots.size(), 2);
}

TEST(VotingFixture, count_votes_1) {
    ASSERT_EQ(test1.candidates[2].candidate_ballots.size(), 1);
}

TEST(VotingFixture, count_votes_2) {
    ASSERT_EQ(test1.candidates[3].candidate_ballots.size(), 2);
}

TEST(VotingFixture, check_winner_0) {
    ASSERT_EQ(check_winner(test1, max_min), false);
}


TEST(VotingFixture, check_winner_1) {
    ASSERT_EQ(max_min[0], 2);
}

TEST(VotingFixture, check_winner_2) {
    ASSERT_EQ(max_min[1], 1);
}

TEST(VotingFixture, populate_losers_list_0) {
    ASSERT_EQ(populate_losers_list(losers, test1, max_min[1], max_min[0]), 2);
}

TEST(VotingFixture, populate_losers_list_1) {
    char* c = const_cast<char*>(losers[0]->name.c_str());
    ASSERT_STREQ(c, "Beta");
}

TEST(VotingFixture, populate_losers_list_2) {
    char* c = const_cast<char*>(losers[1]->name.c_str());
    ASSERT_STREQ(c, "Charlie");
}

TEST(VotingFixture, check_tie_0) {
    ASSERT_EQ(check_tie(test1, max_min[0], max_min[1]), false);
}

TEST(VotingFixture, shift_current_index_0) {
    shift_current_index(losers, test1);
    ASSERT_EQ(test1.ballots[0].current_index, 0);
}

TEST(VotingFixture, shift_current_index_1) {
    ASSERT_EQ(test1.ballots[2].current_index, 2);
}

TEST(VotingFixture, count_votes_3) {
    clear_ballot(test1);
    count_votes(test1);
    ASSERT_EQ(test1.candidates[0].candidate_ballots.size(), 3);
}

TEST(VotingFixture, check_tie_1) {
    max_min[0] = 0;
    max_min[1] = 1000;
    check_winner (test1, max_min);
    populate_losers_list(losers, test1, max_min[1], max_min[0]);
    ASSERT_EQ(check_tie(test1, max_min[0], max_min[1]), true);
}

