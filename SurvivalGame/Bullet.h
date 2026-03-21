#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Bullet {
    sf::ConvexShape m_shape;
    sf::Vector2f m_pos, m_dir;
    float m_speed = 10.f;
    bool m_active = true;
    bool m_isEnemy;
    int m_damage;
    int m_bounceCount = 0;
    const int MAX_BOUNCE = 2;
    bool m_fromPlayer;           // true=玩家子弹，false=防御塔子弹

    // 创建菱形（长:宽 = 2:1）
    void createDiamondShape(float length, float width) {
        m_shape.setPointCount(4);
        m_shape.setPoint(0, sf::Vector2f(0, -width / 2));
        m_shape.setPoint(1, sf::Vector2f(length / 2, 0));
        m_shape.setPoint(2, sf::Vector2f(0, width / 2));
        m_shape.setPoint(3, sf::Vector2f(-length / 2, 0));
    }

public:
    Bullet(sf::Vector2f start, sf::Vector2f target, bool enemy, int dmg, bool fromPlayer = true)
        : m_pos(start), m_isEnemy(enemy), m_damage(dmg), m_fromPlayer(fromPlayer) {

        if (fromPlayer) {
            // 玩家子弹：黄色方形
            m_shape.setPointCount(4);
            m_shape.setPoint(0, sf::Vector2f(-2.5f, -2.5f));
            m_shape.setPoint(1, sf::Vector2f(2.5f, -2.5f));
            m_shape.setPoint(2, sf::Vector2f(2.5f, 2.5f));
            m_shape.setPoint(3, sf::Vector2f(-2.5f, 2.5f));
            m_shape.setFillColor(sf::Color::Yellow);
        }
        else if (enemy) {
            // 敌人子弹：红色方形
            m_shape.setPointCount(4);
            m_shape.setPoint(0, sf::Vector2f(-2.5f, -2.5f));
            m_shape.setPoint(1, sf::Vector2f(2.5f, -2.5f));
            m_shape.setPoint(2, sf::Vector2f(2.5f, 2.5f));
            m_shape.setPoint(3, sf::Vector2f(-2.5f, 2.5f));
            m_shape.setFillColor(sf::Color::Red);
        }
        else {
            // 防御塔子弹：青蓝色菱形
            createDiamondShape(10.f, 5.f);
            m_shape.setFillColor(sf::Color(0, 255, 255));
        }

        m_dir = target - start;
        float len = std::hypot(m_dir.x, m_dir.y);
        if (len != 0) m_dir /= len;

        float angle = std::atan2(m_dir.y, m_dir.x) * 180 / 3.14159f;
        m_shape.setRotation(angle);
        m_shape.setPosition(m_pos);
    }

    // 移动构造函数
    Bullet(Bullet&& other) noexcept
        : m_shape(std::move(other.m_shape)), m_pos(other.m_pos), m_dir(other.m_dir),
        m_speed(other.m_speed), m_active(other.m_active), m_isEnemy(other.m_isEnemy),
        m_damage(other.m_damage), m_bounceCount(other.m_bounceCount), m_fromPlayer(other.m_fromPlayer) {
        other.m_active = false;
    }

    // 移动赋值
    Bullet& operator=(Bullet&& other) noexcept {
        if (this != &other) {
            m_shape = std::move(other.m_shape);
            m_pos = other.m_pos;
            m_dir = other.m_dir;
            m_speed = other.m_speed;
            m_active = other.m_active;
            m_isEnemy = other.m_isEnemy;
            m_damage = other.m_damage;
            m_bounceCount = other.m_bounceCount;
            m_fromPlayer = other.m_fromPlayer;
            other.m_active = false;
        }
        return *this;
    }

    // 禁止拷贝
    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;

    void update() {
        if (!m_active) return;
        m_pos += m_dir * m_speed;
        m_shape.setPosition(m_pos);
        if (m_pos.x < -50 || m_pos.x > 850 || m_pos.y < -50 || m_pos.y > 650)
            m_active = false;
    }

    void draw(sf::RenderWindow& w) { if (m_active) w.draw(m_shape); }

    bool isActive() const { return m_active; }
    void setActive(bool a) { m_active = a; }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }
    bool isEnemy() const { return m_isEnemy; }
    int getDamage() const { return m_damage; }
    bool fromPlayer() const { return m_fromPlayer; }

    void bounce(const sf::Vector2f& normal) {
        if (m_bounceCount >= MAX_BOUNCE) return;
        float dot = m_dir.x * normal.x + m_dir.y * normal.y;
        m_dir.x -= 2 * dot * normal.x;
        m_dir.y -= 2 * dot * normal.y;
        float len = std::hypot(m_dir.x, m_dir.y);
        if (len != 0) m_dir /= len;
        m_bounceCount++;
        float angle = std::atan2(m_dir.y, m_dir.x) * 180 / 3.14159f;
        m_shape.setRotation(angle);
    }

    bool canBounce() const { return m_bounceCount < MAX_BOUNCE; }
    void setPosition(float x, float y) { m_pos = { x,y }; m_shape.setPosition(m_pos); }
    sf::Vector2f getPosition() const { return m_pos; }

    // 判断子弹是否可以穿过防御塔和水晶（玩家和防御塔子弹可以穿过）
    bool canPassThroughTowerAndCrystal() const {
        return !m_isEnemy;  // 非敌人子弹（玩家或防御塔）可以穿过
    }
};