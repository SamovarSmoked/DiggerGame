#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include "Player.h"
#include "Enemy.h"
#include "Level.h"
#include "LeaderboardManager.h"

int main()
{

    int sessionScore = 0;
    const std::vector<std::string> MENU = { "Play", "Help", "Exit" };
    const std::vector<std::string> LEVELS = { "levels/level1.txt", "levels/level2.txt", "levels/level3.txt", "levels/level4.txt" };

    enum class GameState { Menu, EnterName, Help, Playing, Win, GameOver, Pause };
    GameState state = GameState::Menu;
    int menuIndex = 0;
    int currentLvl = 0;
    int pauseIndex = 0;
    const std::vector<std::string> PAUSE_MENU = { "Resume", "Exit" };

    std::string playerName;
    std::string inputBuffer;
    LeaderboardManager leaderboard("scores.txt");
    leaderboard.load();

    auto tryUpdateScore = [&](const std::string& name, int score) {
        auto& entries = const_cast<std::vector<LeaderboardEntry>&>(leaderboard.getEntries());
        auto it = std::remove_if(entries.begin(), entries.end(), [&](const LeaderboardEntry& e) {
            return e.name == name && e.score < score;
            });
        if (it != entries.end()) entries.erase(it, entries.end());

        auto exists = std::any_of(entries.begin(), entries.end(), [&](const LeaderboardEntry& e) {
            return e.name == name && e.score >= score;
            });

        if (!exists) {
            leaderboard.tryAddScore(name, score);
            leaderboard.save();
        }
        };

    sf::RenderWindow window(sf::VideoMode(640, 480), "Digger Game");
    window.setFramerateLimit(60);

    sf::Font font; if (!font.loadFromFile("Textures/arial.ttf")) return -1;

    sf::Music menuMusic, gameMusic;
    if (menuMusic.openFromFile("Sounds/menu_music.wav")) { menuMusic.setLoop(true); menuMusic.setVolume(15.f); menuMusic.play(); }
    if (gameMusic.openFromFile("Sounds/game_music.wav")) { gameMusic.setLoop(true); gameMusic.setVolume(10.f); }

    sf::SoundBuffer bufSelect, bufWin, bufLose;
    sf::Sound selectSound, winSound, loseSound;
    if (bufSelect.loadFromFile("Sounds/select.wav")) { selectSound.setBuffer(bufSelect); selectSound.setVolume(30.f); }
    if (bufWin.loadFromFile("Sounds/win.wav")) { winSound.setBuffer(bufWin); winSound.setVolume(15.f); }
    if (bufLose.loadFromFile("Sounds/lose.wav")) { loseSound.setBuffer(bufLose); loseSound.setVolume(15.f); }

    sf::Text title("DIGGERTHEGAME", font, 40); title.setFillColor(sf::Color::Yellow); title.setPosition(100.f, 80.f);
    sf::Text helpMsg("Use arrows to move. Collect gold. Esc to back.", font, 20); helpMsg.setPosition(50.f, 220.f);
    sf::Text scoreText("Score: 0", font, 18); scoreText.setPosition(10.f, 10.f);

    sf::Text nameInputText("", font, 24); nameInputText.setPosition(100.f, 250.f); nameInputText.setFillColor(sf::Color::White);
    sf::Text leaderboardText("", font, 18); leaderboardText.setPosition(500.f, 10.f);

    Level level(LEVELS[currentLvl]);
    Player player("Textures/player.png", level.getPlayerSpawn());
    Enemy enemy("Textures/enemy.png", level.getEnemySpawn());

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed) window.close();

            if (state == GameState::EnterName)
            {
                if (ev.type == sf::Event::TextEntered)
                {
                    if (ev.text.unicode == '' && !inputBuffer.empty()) inputBuffer.pop_back();
                    else if (ev.text.unicode < 128 && std::isprint(ev.text.unicode) && inputBuffer.size() < 10)
                        inputBuffer += static_cast<char>(ev.text.unicode);
                }
                else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter && !inputBuffer.empty())
                {
                    playerName = inputBuffer;
                    inputBuffer.clear();
                    state = GameState::Playing;
                    menuMusic.stop();
                    if (gameMusic.getStatus() != sf::Music::Playing) gameMusic.play();
                }
                continue;
            }

            if (state == GameState::Menu && ev.type == sf::Event::KeyPressed)
            {
                if (ev.key.code == sf::Keyboard::Up)
                {
                    menuIndex = (menuIndex + MENU.size() - 1) % MENU.size();
                    selectSound.play();
                }
                else if (ev.key.code == sf::Keyboard::Down)
                {
                    menuIndex = (menuIndex + 1) % MENU.size();
                    selectSound.play();
                }
                else if (ev.key.code == sf::Keyboard::Enter)
                {
                    if (menuIndex == 0)
                    {
                        state = GameState::EnterName;
                        inputBuffer.clear();
                    }
                    else if (menuIndex == 1) state = GameState::Help;
                    else return 0;
                }
            }
            else if (state == GameState::Help && ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
            {
                state = GameState::Menu;
            }
            else if (state == GameState::Pause && ev.type == sf::Event::KeyPressed)
            {
                if (ev.key.code == sf::Keyboard::Up || ev.key.code == sf::Keyboard::Down)
                {
                    pauseIndex = 1 - pauseIndex;
                    selectSound.play();
                }
                else if (ev.key.code == sf::Keyboard::Enter)
                {
                    if (pauseIndex == 0) state = GameState::Playing;
                    else {
                        state = GameState::Menu;
                        currentLvl = 0;
                        level = Level(LEVELS[currentLvl]);
                        player = Player("Textures/player.png", level.getPlayerSpawn());
                        enemy = Enemy("Textures/enemy.png", level.getEnemySpawn());
                        if (menuMusic.getStatus() != sf::Music::Playing) menuMusic.play();
                        gameMusic.stop();
                    }
                }
            }
            else if (state == GameState::Playing && ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
            {
                state = GameState::Pause;
            }
        }

        if (state == GameState::Playing)
        {
            player.handleInput(level);
            enemy.update(level, player.getPosition());
            if (enemy.getPosition() == player.getPosition())
                state = GameState::GameOver;
            if (!level.hasGold())
                state = GameState::Win;
        }

        window.clear();

        switch (state)
        {
        case GameState::Menu:
        {
            window.draw(title);
            for (int i = 0; i < static_cast<int>(MENU.size()); ++i)
            {
                sf::Text item(MENU[i], font, 28);
                item.setPosition(220.f, 160.f + i * 40.f);
                item.setFillColor(i == menuIndex ? sf::Color::Yellow : sf::Color::White);
                window.draw(item);
            }
            std::string lbText = "Top 10:\n";
            for (const auto& entry : leaderboard.getEntries())
                lbText += entry.name + ": " + std::to_string(entry.score) + "\n";
            leaderboardText.setString(lbText);
            window.draw(leaderboardText);
            break;
        }
        case GameState::Help:
            window.draw(helpMsg); break;
        case GameState::EnterName:
        {
            sf::Text prompt("Enter your name (max 10 chars):", font, 26);
            prompt.setPosition(100.f, 200.f);
            prompt.setFillColor(sf::Color::Cyan);
            window.draw(prompt);
            nameInputText.setString(inputBuffer + "_");
            window.draw(nameInputText);
            break;
        }
        case GameState::Playing:
            sessionScore = 0;
            level.draw(window); player.draw(window); enemy.draw(window);
            scoreText.setString("Score: " + std::to_string(sessionScore + player.getScore()));
            window.draw(scoreText); break;
        case GameState::Pause:
        {
            sf::Text pauseTitle("Paused", font, 36);
            pauseTitle.setFillColor(sf::Color::Cyan);
            pauseTitle.setPosition(230.f, 100.f);
            window.draw(pauseTitle);
            for (int i = 0; i < 2; ++i)
            {
                sf::Text item(PAUSE_MENU[i], font, 28);
                item.setPosition(240.f, 160.f + i * 40.f);
                item.setFillColor(i == pauseIndex ? sf::Color::Yellow : sf::Color::White);
                window.draw(item);
            }
            break;
        }
        case GameState::Win:
        {
            if (winSound.getStatus() != sf::Sound::Playing) winSound.play();
            gameMusic.stop();
            sf::Text winMsg("You Win! Press ENTER to continue", font, 28);
            winMsg.setFillColor(sf::Color::Green);
            winMsg.setPosition(70.f, 220.f);
            window.draw(winMsg);

            sessionScore += player.getScore();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                ++currentLvl;
                if (currentLvl < static_cast<int>(LEVELS.size()))
                {
                    level = Level(LEVELS[currentLvl]);
                    player = Player("Textures/player.png", level.getPlayerSpawn());
                    enemy = Enemy("Textures/enemy.png", level.getEnemySpawn());
                    state = GameState::Playing;
                    winSound.stop();
                    gameMusic.play();
                }
                else {
                    tryUpdateScore(playerName, sessionScore);
                    sessionScore = 0;
                    state = GameState::Menu;
                    currentLvl = 0;
                    level = Level(LEVELS[currentLvl]);
                    player = Player("Textures/player.png", level.getPlayerSpawn());
                    enemy = Enemy("Textures/enemy.png", level.getEnemySpawn());
                    if (menuMusic.getStatus() != sf::Music::Playing) menuMusic.play();
                }
            }
            break;
        }
        case GameState::GameOver:
        {

            sessionScore += player.getScore();
            tryUpdateScore(playerName, sessionScore);
            sessionScore = 0;

            if (loseSound.getStatus() != sf::Sound::Playing) loseSound.play();
            gameMusic.stop();
            sf::Text overMsg("Game Over! Press ENTER to return to menu", font, 28);
            overMsg.setFillColor(sf::Color::Red);
            overMsg.setPosition(50.f, 220.f);
            window.draw(overMsg);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                tryUpdateScore(playerName, player.getScore());
                state = GameState::Menu;
                currentLvl = 0;
                level = Level(LEVELS[currentLvl]);
                player = Player("Textures/player.png", level.getPlayerSpawn());
                enemy = Enemy("Textures/enemy.png", level.getEnemySpawn());
                loseSound.stop();
                if (menuMusic.getStatus() != sf::Music::Playing) menuMusic.play();
            }
            break;
        }
        }

        window.display();
    }

    return 0;
}
