#include "LeaderboardManager.h"
#include <fstream>
#include <algorithm>
#include <sstream>

LeaderboardManager::LeaderboardManager(const std::string& filename)
    : m_filename(filename)
{
}

void LeaderboardManager::load()
{
    m_entries.clear();
    std::ifstream in(m_filename);
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;
        if (iss >> name >> score) {
            m_entries.push_back({ name, score });
        }
    }
    std::sort(m_entries.begin(), m_entries.end());
    if (m_entries.size() > MAX_ENTRIES)
        m_entries.resize(MAX_ENTRIES);
}

void LeaderboardManager::save() const
{
    std::ofstream out(m_filename);
    for (const auto& entry : m_entries) {
        out << entry.name << " " << entry.score << "\n";
    }
}

void LeaderboardManager::tryAddScore(const std::string& name, int score)
{
    m_entries.push_back({ name, score });
    std::sort(m_entries.begin(), m_entries.end());
    if (m_entries.size() > MAX_ENTRIES)
        m_entries.resize(MAX_ENTRIES);
}

const std::vector<LeaderboardEntry>& LeaderboardManager::getEntries() const{
    return m_entries;
}