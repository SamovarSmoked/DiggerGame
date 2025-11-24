#pragma once
#include <string>

struct LeaderboardEntry {
    std::string name;
    int score;

    bool operator<(const LeaderboardEntry& other) const {
        return score > other.score;
    }
};