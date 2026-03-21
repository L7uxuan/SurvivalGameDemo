#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "HealthPack.h"
#include "HealthBar.h"
#include "EnemyHealthBar.h"
#include "TrailParticle.h"
#include "SlashEffect.h"
#include "DamageNumber.h"
#include "Crystal.h"
#include "Tower.h"

sf::Font g_font;
bool g_fontLoaded = false;

enum class State { MENU, NAME, DIALOGUE, PLAYING, GAME_OVER, WIN, INSTRUCTION };

struct Button {
    sf::FloatRect rect; std::wstring text; bool hover;
    Button(float x, float y, float w, float h, const std::wstring& t)
        : rect(x, y, w, h), text(t), hover(false) {
    }
    void draw(sf::RenderWindow& win) {
        sf::RectangleShape shape({ rect.width, rect.height });
        shape.setPosition(rect.left, rect.top);
        shape.setFillColor(hover ? sf::Color(120, 120, 120) : sf::Color(70, 70, 70));
        shape.setOutlineColor(sf::Color::White); shape.setOutlineThickness(2);
        win.draw(shape);
        if (g_fontLoaded) {
            sf::Text txt(text, g_font, 24);
            txt.setFillColor(sf::Color::White);
            auto b = txt.getLocalBounds();
            txt.setPosition(rect.left + (rect.width - b.width) / 2, rect.top + (rect.height - b.height) / 2);
            win.draw(txt);
        }
    }
};

template<typename T>
T clamp(const T& value, const T& low, const T& high) {
    return std::max(low, std::min(value, high));
}

sf::Vector2f randomEdge() {
    int side = rand() % 4;
    if (side == 0) return { float(rand() % 800), 10.f };
    if (side == 1) return { float(rand() % 800), 590.f };
    if (side == 2) return { 10.f, float(rand() % 600) };
    return { 790.f, float(rand() % 600) };
}

void drawCenteredText(sf::RenderWindow& win, const sf::Text& text, float y) {
    sf::Text t = text;
    auto bounds = t.getLocalBounds();
    t.setOrigin(bounds.width / 2, bounds.height / 2);
    t.setPosition(400, y);
    win.draw(t);
}

int main() {
    std::srand(static_cast<unsigned>(time(nullptr)));
    sf::RenderWindow win({ 800,600 }, L"生存游戏");
    win.setFramerateLimit(60);

    for (auto& p : { "C:/Windows/Fonts/msyh.ttc", "msyh.ttf", "simhei.ttf" })
        if (g_font.loadFromFile(p)) { g_fontLoaded = true; break; }

    // 背景纹理加载
    sf::Texture bgTexture, menuBgTexture, storyBgTexture;
    sf::Sprite bgSprite, menuBgSprite, storyBgSprite;
    if (bgTexture.loadFromFile("assets/background.png") ||
        bgTexture.loadFromFile("assets/background.jpg")) {
        bgSprite.setTexture(bgTexture);
        bgSprite.setScale(800.f / bgTexture.getSize().x, 600.f / bgTexture.getSize().y);
    }
    if (menuBgTexture.loadFromFile("assets/menu_bg.png")) {
        menuBgSprite.setTexture(menuBgTexture);
        menuBgSprite.setScale(800.f / menuBgTexture.getSize().x, 600.f / menuBgTexture.getSize().y);
    }
    if (storyBgTexture.loadFromFile("assets/story_bg.png")) {
        storyBgSprite.setTexture(storyBgTexture);
        storyBgSprite.setScale(800.f / storyBgTexture.getSize().x, 600.f / storyBgTexture.getSize().y);
    }

    State state = State::MENU;
    sf::Clock clock, gameClock, invClock, reviveClock, waveTimer;
    std::vector<Button> menu = { {300,200,200,50, L"开始游戏"}, {300,270,200,50, L"游戏说明"}, {300,340,200,50, L"退出游戏"} };

    std::wstring playerName;
    sf::Text nameText, dialogue;
    if (g_fontLoaded) {
        nameText.setFont(g_font); nameText.setCharacterSize(30); nameText.setFillColor(sf::Color::White); nameText.setPosition(300, 250);
        dialogue.setFont(g_font); dialogue.setCharacterSize(20); dialogue.setFillColor(sf::Color::White); dialogue.setPosition(50, 450);
    }
    std::vector<std::wstring> dialogueLines = {
        L"你不知道为何睡着，醒来发现自己处在一片圣地之中...",
        L"“嗯..? 我在哪。”",
        L"无线电：“呲...喂{}，情况有变！守护好我们的水晶！”",
        L"“......”"
    };
    int dialogueIdx = 0;

    sf::Text instruction;
    if (g_fontLoaded) instruction.setFont(g_font), instruction.setString(L"WASD移动\n左键攻击\n右键冲锋\nX建塔\n空格暂停\n保护水晶！"), instruction.setPosition(200, 100);

    Player player;
    player.loadTex("assets/Rifleman/");
    player.setPos(300, 300);
    Crystal crystal(400, 300);
    std::vector<Tower> towers;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets, enemyBullets;
    std::vector<HealthPack> healthPacks;
    std::vector<TrailParticle> trails;
    std::vector<SlashEffect> slashes;
    std::vector<DamageNumber> dmgNums;
    HealthBar hpBar; hpBar.setPosition(10, 10);
    EnemyHealthBar enemyHpBar;

    int wave = 1, spawned = 0;
    const int waveSize[3] = { 15,25,500 };
    const float waveSpawnTime[3] = { 30.f,30.f,50.f };
    const float waveDelay[3] = { 5.f,5.f,5.f };
    bool waveActive = false, waveStarting = false;
    float waveStartTimer = 0, waveGenTimer = 0;
    bool paused = false, playerDead = false;
    float reviveTime = 3.f;
    bool playerInvincible = false;
    const float playerInvincibleTime = 0.3f;

    bool waveAnnounceActive = false;
    sf::Clock waveAnnounceClock;
    sf::Text waveAnnounceText;
    if (g_fontLoaded) {
        waveAnnounceText.setFont(g_font);
        waveAnnounceText.setCharacterSize(40);
        waveAnnounceText.setFillColor(sf::Color::Red);
    }

    sf::Text hpTxt, energyTxt, waveTxt, crystalTxt, pauseText, reviveText;
    if (g_fontLoaded) {
        hpTxt.setFont(g_font); hpTxt.setString(L"生命"); hpTxt.setPosition(10, 10);
        energyTxt.setFont(g_font); energyTxt.setString(L"能量"); energyTxt.setPosition(10, 25);
        waveTxt.setFont(g_font); waveTxt.setPosition(10, 50);
        crystalTxt.setFont(g_font); crystalTxt.setPosition(10, 75);
        pauseText.setFont(g_font); pauseText.setString(L"暂停"); pauseText.setCharacterSize(50); pauseText.setFillColor(sf::Color::White);
        reviveText.setFont(g_font); reviveText.setCharacterSize(30); reviveText.setFillColor(sf::Color::Yellow);
        for (auto& t : { &hpTxt,&energyTxt,&waveTxt,&crystalTxt }) t->setCharacterSize(20), t->setFillColor(sf::Color::White);
    }
    sf::Text resultText; if (g_fontLoaded) resultText.setFont(g_font), resultText.setCharacterSize(40), resultText.setFillColor(sf::Color::Yellow);

    static bool texLoaded = false;
    if (!texLoaded) { Enemy::loadTextures("assets/landmaster", "assets/flyingmaster"); texLoaded = true; }

    Tower::loadTexture("assets/tower.png");
    Crystal::loadTexture("assets/crystal.png");

    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            if (e.type == sf::Event::MouseMoved && state == State::MENU) {
                sf::Vector2f p = win.mapPixelToCoords({ e.mouseMove.x, e.mouseMove.y });
                for (auto& b : menu) b.hover = b.rect.contains(p);
            }
            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f p = win.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y });
                if (state == State::MENU) {
                    if (menu[0].rect.contains(p)) state = State::NAME;
                    else if (menu[1].rect.contains(p)) state = State::INSTRUCTION;
                    else win.close();
                }
                else if (state == State::INSTRUCTION) state = State::MENU;
                else if (state == State::DIALOGUE) {
                    dialogueIdx++;
                    if (dialogueIdx >= (int)dialogueLines.size()) {
                        state = State::PLAYING;
                        player = Player(); player.loadTex("assets/Rifleman/"); player.setPos(300, 300);
                        crystal = Crystal(400, 300);
                        towers.clear(); enemies.clear(); bullets.clear(); enemyBullets.clear(); healthPacks.clear(); trails.clear(); slashes.clear(); dmgNums.clear();
                        for (int i = 0; i < 6; i++) {
                            auto pos = randomEdge();
                            enemies.emplace_back(pos.x, pos.y, EnemyType::NORMAL);
                            enemies.back().setSpeed(0.5f);
                        }
                        wave = 1; spawned = 0; waveActive = false; waveStarting = true; waveStartTimer = 0;
                        gameClock.restart(); playerDead = false; paused = false;
                        std::cout << "游戏开始，5秒后第一波来袭" << std::endl;
                    }
                    else {
                        std::wstring line = dialogueLines[dialogueIdx];
                        size_t pos = line.find(L"{}");
                        if (pos != std::wstring::npos) line.replace(pos, 2, playerName);
                        dialogue.setString(line);
                    }
                }
            }
            if (state == State::DIALOGUE && e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                dialogueIdx++;
                if (dialogueIdx >= (int)dialogueLines.size()) {
                    state = State::PLAYING;
                    player = Player(); player.loadTex("assets/Rifleman/"); player.setPos(300, 300);
                    crystal = Crystal(400, 300);
                    towers.clear(); enemies.clear(); bullets.clear(); enemyBullets.clear(); healthPacks.clear(); trails.clear(); slashes.clear(); dmgNums.clear();
                    for (int i = 0; i < 6; i++) {
                        auto pos = randomEdge();
                        enemies.emplace_back(pos.x, pos.y, EnemyType::NORMAL);
                        enemies.back().setSpeed(0.5f);
                    }
                    wave = 1; spawned = 0; waveActive = false; waveStarting = true; waveStartTimer = 0;
                    gameClock.restart(); playerDead = false; paused = false;
                }
                else {
                    std::wstring line = dialogueLines[dialogueIdx];
                    size_t pos = line.find(L"{}");
                    if (pos != std::wstring::npos) line.replace(pos, 2, playerName);
                    dialogue.setString(line);
                }
            }
            if (state == State::NAME) {
                if (e.type == sf::Event::TextEntered) {
                    if (e.text.unicode == '\b') { if (!playerName.empty()) playerName.pop_back(); }
                    else if (e.text.unicode >= 32 && e.text.unicode != 127 && playerName.size() < 20)
                        playerName += static_cast<wchar_t>(e.text.unicode);
                }
                else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                    dialogueIdx = 0; dialogue.setString(dialogueLines[0]); state = State::DIALOGUE;
                }
            }
            if (state == State::PLAYING && !paused) {
                if (e.type == sf::Event::MouseMoved) player.setMouse(win.mapPixelToCoords({ e.mouseMove.x, e.mouseMove.y }));
                if (e.type == sf::Event::MouseButtonPressed) {
                    if (e.mouseButton.button == sf::Mouse::Left && !playerDead) {
                        player.setFaceMouse(true);
                        bullets.emplace_back(player.getCenter(), win.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y }), false, player.getAtk(), true);
                    }
                    else if (e.mouseButton.button == sf::Mouse::Right && !playerDead) {
                        player.setFaceMouse(true);
                        sf::Vector2f dir = player.getMouse() - player.getPos();
                        float len = std::hypot(dir.x, dir.y);
                        if (len > 0) dir /= len;
                        sf::Vector2f newPos = player.getPos() + dir * 200.f;
                        newPos.x = clamp(newPos.x, 0.f, 770.f);
                        newPos.y = clamp(newPos.y, 0.f, 570.f);
                        player.setPos(newPos);
                        for (int i = 0; i < 10; i++)
                            trails.emplace_back(player.getPos() - dir * static_cast<float>(i) * 20.f);
                    }
                }
                if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) player.setFaceMouse(false);
                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::X && !playerDead && player.useEnergy(30))
                    towers.emplace_back(player.getPos().x, player.getPos().y);
                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space) paused = !paused;
            }
            if ((state == State::GAME_OVER || state == State::WIN) && e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                state = State::MENU;
            }
        }

        if (state == State::PLAYING && !paused) {
            float elapsed = gameClock.getElapsedTime().asSeconds();

            if (playerInvincible && invClock.getElapsedTime().asSeconds() > playerInvincibleTime) playerInvincible = false;

            if (wave <= 3) {
                if (waveStarting) {
                    if (waveStartTimer == 0) waveStartTimer = elapsed;
                    if (elapsed - waveStartTimer >= waveDelay[wave - 1]) {
                        waveActive = true; waveStarting = false; spawned = 0;
                        waveAnnounceText.setString(L"第" + std::to_wstring(wave) + L"波来袭");
                        waveAnnounceActive = true;
                        waveAnnounceClock.restart();
                        waveGenTimer = elapsed;
                        std::cout << "第" << wave << "波来袭！" << std::endl;
                    }
                }
                else if (waveActive) {
                    float progress = (elapsed - waveGenTimer) / waveSpawnTime[wave - 1];
                    int target = waveSize[wave - 1] * std::min(1.f, progress);
                    while (spawned < target && spawned < waveSize[wave - 1]) {
                        auto pos = randomEdge();
                        enemies.emplace_back(pos.x, pos.y, (rand() % 5 == 0) ? EnemyType::FAST : EnemyType::NORMAL);
                        spawned++;
                    }
                    if (spawned >= waveSize[wave - 1] && enemies.empty()) {
                        wave++;
                        if (wave <= 3) { waveStarting = true; waveStartTimer = elapsed; waveActive = false; }
                        else state = State::WIN;
                    }
                }
                else if (enemies.empty()) { wave++; if (wave <= 3) { waveStarting = true; waveStartTimer = elapsed; } else state = State::WIN; }
            }

            if (!playerDead) { player.handleInput(); player.update(); hpBar.update(player.getHp(), player.getMaxHp()); }

            for (auto& e : enemies) e.update(towers, crystal.getPos(), enemyBullets, false, player.getPos());
            for (auto& b : bullets) b.update();
            for (auto& b : enemyBullets) b.update();
            for (auto& p : healthPacks) p.update();
            for (auto& t : trails) t.update();
            for (auto& s : slashes) s.update();
            for (auto& d : dmgNums) d.update();
            for (auto& t : towers) t.update(enemies, bullets);

            // 子弹反弹（玩家子弹）
            for (auto& b : bullets) {
                if (!b.isActive()) continue;
                sf::FloatRect bounds = b.getBounds();
                bool bounced = false;
                sf::Vector2f normal;
                if (bounds.left < 0) { normal = { 1,0 }; bounced = true; }
                else if (bounds.left + bounds.width > 800) { normal = { -1,0 }; bounced = true; }
                else if (bounds.top < 0) { normal = { 0,1 }; bounced = true; }
                else if (bounds.top + bounds.height > 600) { normal = { 0,-1 }; bounced = true; }
                if (bounced && b.canBounce()) {
                    b.bounce(normal);
                    b.setPosition(clamp(b.getPosition().x, 1.f, 798.f), clamp(b.getPosition().y, 1.f, 598.f));
                }
                // 只有敌人子弹才会与塔和水晶碰撞
                if (b.isEnemy()) {
                    for (auto& t : towers) {
                        if (!t.isActive()) continue;
                        if (b.getBounds().intersects(t.getBounds())) {
                            if (b.canBounce()) {
                                sf::Vector2f dirToCenter = b.getPosition() - t.getPos();
                                float len = std::hypot(dirToCenter.x, dirToCenter.y);
                                if (len > 0) dirToCenter /= len;
                                b.bounce(dirToCenter);
                            }
                            break;
                        }
                    }
                    if (b.getBounds().intersects(crystal.getBounds())) {
                        if (b.canBounce()) {
                            sf::Vector2f dirToCenter = b.getPosition() - crystal.getPos();
                            float len = std::hypot(dirToCenter.x, dirToCenter.y);
                            if (len > 0) dirToCenter /= len;
                            b.bounce(dirToCenter);
                        }
                    }
                }
            }

            // 玩家子弹碰撞敌人（设置仇恨）
            for (auto& b : bullets) {
                if (!b.isActive()) continue;
                for (auto& e : enemies) {
                    if (!e.isActive() || !b.getBounds().intersects(e.getBounds())) continue;
                    b.setActive(false);
                    e.takeDamage(b.getDamage());
                    dmgNums.emplace_back(b.getDamage(), e.getPos(), sf::Color(255, 165, 0));
                    if (b.fromPlayer()) {
                        e.setTarget(player.getPos());
                    }
                    else {
                        e.setTarget(b.getPosition());
                    }
                    if (!e.isActive()) { player.addEnergy(10); if (rand() % 100 < 5) healthPacks.emplace_back(e.getPos().x, e.getPos().y); }
                    break;
                }
            }

            // 敌人子弹碰撞玩家
            for (auto& b : enemyBullets) {
                if (!b.isActive() || !b.getBounds().intersects(player.getBounds())) continue;
                b.setActive(false);
                if (!playerDead && !playerInvincible) {
                    player.takeDamage(b.getDamage());
                    dmgNums.emplace_back(b.getDamage(), player.getPos(), sf::Color::Red);
                    playerInvincible = true; invClock.restart();
                }
            }

            // 血包
            for (auto& p : healthPacks) {
                if (!p.isActive() || !player.getBounds().intersects(p.getBounds())) continue;
                player.heal(p.getHealAmount()); p.setActive(false);
            }

            // 近战攻击玩家（基于圆心距离检测）
            for (auto& e : enemies) {
                if (!e.isActive() || e.getType() == EnemyType::FAST) continue;
                float dist = std::hypot(player.getPos().x - e.getPos().x, player.getPos().y - e.getPos().y);
                float playerRadius = 15.f;
                float enemyRadius = e.getType() == EnemyType::NORMAL ? 2.f : 1.f;
                if (dist < playerRadius + enemyRadius && !playerDead && !playerInvincible && e.canAttackPlayer()) {
                    player.takeDamage(30);
                    dmgNums.emplace_back(30, player.getPos(), sf::Color::Red);
                    playerInvincible = true;
                    invClock.restart();
                    sf::Vector2f dir = player.getPos() - e.getPos();
                    slashes.emplace_back(e.getPos() + dir * 0.5f, atan2(dir.y, dir.x));
                    e.resetAttackClock();
                }
            }

            // 近战攻击水晶
            for (auto& e : enemies) {
                if (!e.isActive() || e.getType() == EnemyType::FAST) continue;
                if (e.getBounds().intersects(crystal.getBounds())) {
                    if (e.isAttacking()) {
                        crystal.takeDamage(30);
                        dmgNums.emplace_back(30, crystal.getPos(), sf::Color::Red);
                        e.resetAttack();
                    }
                }
            }
            // 近战攻击塔
            for (auto& e : enemies) {
                if (!e.isActive() || e.getType() == EnemyType::FAST) continue;
                for (auto& t : towers) {
                    if (!t.isActive()) continue;
                    if (e.getBounds().intersects(t.getBounds())) {
                        if (e.isAttacking()) {
                            t.takeDamage(30);
                            dmgNums.emplace_back(30, t.getPos(), sf::Color::Red);
                            e.resetAttack();
                        }
                        break;
                    }
                }
            }

            // 自爆兵爆炸
            for (auto& e : enemies) {
                if (e.getType() != EnemyType::FAST) continue;
                if (e.isActive()) continue;
                float explosionRange = 60.f;
                if (std::hypot(player.getPos().x - e.getPos().x, player.getPos().y - e.getPos().y) <= explosionRange && !playerDead && !playerInvincible) {
                    player.takeDamage(88);
                    dmgNums.emplace_back(88, player.getPos(), sf::Color::Red);
                    playerInvincible = true;
                    invClock.restart();
                }
                if (std::hypot(crystal.getPos().x - e.getPos().x, crystal.getPos().y - e.getPos().y) <= explosionRange) {
                    crystal.takeDamage(88);
                    dmgNums.emplace_back(88, crystal.getPos(), sf::Color::Red);
                }
                for (auto& t : towers) {
                    if (std::hypot(t.getPos().x - e.getPos().x, t.getPos().y - e.getPos().y) <= explosionRange) {
                        t.takeDamage(88);
                        dmgNums.emplace_back(88, t.getPos(), sf::Color::Red);
                    }
                }
            }

            // 清理失效对象
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return !e.isActive(); }), enemies.end());
            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.isActive(); }), bullets.end());
            enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const Bullet& b) { return !b.isActive(); }), enemyBullets.end());
            healthPacks.erase(std::remove_if(healthPacks.begin(), healthPacks.end(), [](const HealthPack& p) { return !p.isActive(); }), healthPacks.end());
            trails.erase(std::remove_if(trails.begin(), trails.end(), [](const TrailParticle& t) { return !t.isActive(); }), trails.end());
            slashes.erase(std::remove_if(slashes.begin(), slashes.end(), [](const SlashEffect& s) { return !s.isActive(); }), slashes.end());
            dmgNums.erase(std::remove_if(dmgNums.begin(), dmgNums.end(), [](const DamageNumber& d) { return !d.isActive(); }), dmgNums.end());

            if (crystal.getHp() <= 0) {
                state = State::GAME_OVER;
                resultText.setString(L"挑战失败，菜就多练");
            }

            if (!playerDead && !player.alive()) { playerDead = true; reviveClock.restart(); for (auto& e : enemies) e.clearTarget(); }
            if (playerDead && reviveClock.getElapsedTime().asSeconds() >= reviveTime) {
                player = Player(); player.loadTex("assets/Rifleman/"); player.setPos(300, 300); playerDead = false; playerInvincible = true; invClock.restart();
            }

            if (g_fontLoaded) {
                waveTxt.setString(L"第" + std::to_wstring(wave) + L"波/共3波");
                crystalTxt.setString(L"水晶: " + std::to_wstring(crystal.getHp()) + L"/500");
                if (playerDead) reviveText.setString(L"复活倒计时: " + std::to_wstring(int(reviveTime - reviveClock.getElapsedTime().asSeconds()) + 1) + L"秒");
                else reviveText.setString(L"");
            }
            if (waveAnnounceActive && waveAnnounceClock.getElapsedTime().asSeconds() > 2.f)
                waveAnnounceActive = false;
        }

        if (state == State::WIN && g_fontLoaded) {
            resultText.setString(L"恭喜你，挑战成功！");
        }

        // 渲染
        win.clear(sf::Color::Black);

        if (state == State::MENU) {
            if (menuBgTexture.getSize().x > 0) win.draw(menuBgSprite);
            for (auto& b : menu) b.draw(win);
        }
        else if (state == State::NAME) {
            if (storyBgTexture.getSize().x > 0) win.draw(storyBgSprite);
            if (g_fontLoaded) {
                sf::Text prompt(L"请输入你的名字：", g_font, 30); prompt.setPosition(200, 200);
                win.draw(prompt);
                nameText.setString(playerName + (clock.getElapsedTime().asSeconds() > 0.5f ? L"_" : L""));
                win.draw(nameText);
            }
        }
        else if (state == State::DIALOGUE) {
            if (storyBgTexture.getSize().x > 0) win.draw(storyBgSprite);
            if (g_fontLoaded) win.draw(dialogue);
        }
        else if (state == State::INSTRUCTION) {
            if (g_fontLoaded) win.draw(instruction);
        }
        else if (state == State::PLAYING || state == State::GAME_OVER || state == State::WIN) {
            // 背景
            if (bgTexture.getSize().x > 0) win.draw(bgSprite);

            // 水晶（底层）
            crystal.draw(win);
            crystal.drawHealthBar(win, &g_font);

            // 防御塔（中间层）
            for (auto& t : towers) {
                t.draw(win);
                if (t.isActive()) {
                    sf::FloatRect tb = t.getBounds();
                    float barWidth = 35.f;
                    float barHeight = 4.f;
                    float barX = tb.left + tb.width / 2 - barWidth / 2;
                    float barY = tb.top - 12;

                    sf::RectangleShape towerBg({ barWidth, barHeight });
                    towerBg.setFillColor({ 50,50,50 });
                    towerBg.setOutlineColor(sf::Color::White);
                    towerBg.setOutlineThickness(1);
                    towerBg.setPosition(barX, barY);
                    win.draw(towerBg);

                    float p = float(t.getHp()) / t.getMaxHp();
                    sf::RectangleShape towerFg({ barWidth * p, barHeight });
                    towerFg.setFillColor(sf::Color::Green);
                    towerFg.setPosition(barX, barY);
                    win.draw(towerFg);

                    if (g_fontLoaded) {
                        sf::Text hpNum(std::to_wstring(t.getHp()) + L"/" + std::to_wstring(t.getMaxHp()), g_font, 9);
                        hpNum.setFillColor(sf::Color::White);
                        auto b = hpNum.getLocalBounds();
                        hpNum.setPosition(barX + barWidth / 2 - b.width / 2, barY - 10);
                        win.draw(hpNum);
                    }
                }
            }

            // 玩家和敌人（顶层）
            if (!playerDead) player.draw(win);
            for (auto& e : enemies) e.draw(win);

            // 子弹、特效等
            for (auto& b : bullets) b.draw(win);
            for (auto& b : enemyBullets) b.draw(win);
            for (auto& p : healthPacks) p.draw(win);
            for (auto& t : trails) t.draw(win);
            for (auto& s : slashes) s.draw(win);
            for (auto& d : dmgNums) d.draw(win, &g_font);

            hpBar.draw(win);
            for (auto& e : enemies) enemyHpBar.draw(win, e, &g_font);

            if (g_fontLoaded) {
                win.draw(hpTxt); win.draw(energyTxt); win.draw(waveTxt); win.draw(crystalTxt);
                sf::RectangleShape bg({ 200,15 }); bg.setFillColor({ 50,50,50 }); bg.setOutlineColor(sf::Color::White); bg.setOutlineThickness(1); bg.setPosition(10, 35);
                sf::RectangleShape fg({ 200 * player.getEnergy() / player.getMaxEnergy(), 15 }); fg.setFillColor(sf::Color::Cyan); fg.setPosition(10, 35);
                win.draw(bg); win.draw(fg);
                if (waveAnnounceActive) drawCenteredText(win, waveAnnounceText, 250);
                if (playerDead) drawCenteredText(win, reviveText, 300);
                if (paused) drawCenteredText(win, pauseText, 250);
            }
            if (state == State::GAME_OVER || state == State::WIN) {
                sf::RectangleShape overlay({ 400,200 }); overlay.setFillColor({ 0,0,0,200 }); overlay.setOutlineColor(sf::Color::White); overlay.setOutlineThickness(3); overlay.setPosition(200, 200);
                win.draw(overlay);
                drawCenteredText(win, resultText, 300);
            }
        }
        win.display();
    }
    return 0;
}