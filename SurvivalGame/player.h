#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Player {
private:
    sf::RectangleShape m_shape;
    sf::Sprite m_sprite;
    sf::Vector2f m_pos;
    float m_speed = 135.f;
    int m_hp = 100, m_maxHp = 100;
    int m_atkMin = 25, m_atkMax = 35;
    float m_crit = 0.1f;
    float m_energy = 0, m_maxEnergy = 100;

    // ¶¯»­
    std::vector<sf::Texture> m_front, m_back, m_left, m_right;
    int m_frame = 0;
    sf::Clock m_animClock;
    float m_frameTime = 0.15f;
    bool m_useSprite = false, m_moving = false;
    enum Dir { FRONT, BACK, LEFT, RIGHT } m_dir = FRONT;

    sf::Vector2f m_mousePos;
    bool m_faceMouse = false;

public:
    Player();
    bool loadTex(const std::string& basePath);
    void handleInput();
    void update();
    void draw(sf::RenderWindow& w);

    void takeDamage(int d) { m_hp = std::max(0, m_hp - d); addEnergy(d * 0.5f); }
    void heal(int a) { m_hp = std::min(m_maxHp, m_hp + a); }
    int getHp() const { return m_hp; }
    int getMaxHp() const { return m_maxHp; }
    bool alive() const { return m_hp > 0; }

    sf::Vector2f getPos() const { return m_pos; }
    sf::Vector2f getCenter() const;
    sf::FloatRect getBounds() const;
    void setPos(float x, float y) { m_pos = { x, y }; }
    void setPos(const sf::Vector2f& p) { m_pos = p; }
    void setMouse(const sf::Vector2f& p) { m_mousePos = p; }
    sf::Vector2f getMouse() const { return m_mousePos; }
    void setFaceMouse(bool f) { m_faceMouse = f; }

    int getAtk();
    float getEnergy() const { return m_energy; }
    float getMaxEnergy() const { return m_maxEnergy; }
    void addEnergy(float a) { m_energy = std::min(m_maxEnergy, m_energy + a); }
    bool useEnergy(float p);
};