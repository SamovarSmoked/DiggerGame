#pragma once
#include "LeaderboardEntry.h"
#include <vector>
#include <string>

class LeaderboardManager {
public:
    explicit LeaderboardManager(const std::string& filename);
    void load();
    void save() const;

    void tryAddScore(const std::string& name, int score);
    const std::vector<LeaderboardEntry>& getEntries() const;

private:
    std::string m_filename;
    std::vector<LeaderboardEntry> m_entries;
    static constexpr size_t MAX_ENTRIES = 10;
};