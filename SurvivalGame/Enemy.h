#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"

class Tower; // 前向声明

enum class EnemyType { NORMAL, FAST };

class Enemy {
private:
    // 渲染相关
    sf::RectangleShape m_shape;
    sf::Sprite m_sprite;
    sf::Vector2f m_pos;
    float m_speed;
    int m_hp, m_maxHp;
    bool m_active = true;
    EnemyType m_type;
    sf::Color m_color;

    // 动画相关（静态纹理）
    static std::vector<sf::Texture> m_normalTextures;
    static std::vector<sf::Texture> m_fastTextures;
    static bool m_texturesLoaded;
    int m_currentFrame;
    sf::Clock m_animClock;
    float m_frameDuration;
    bool m_useSprite;
    bool m_isMoving;

    // 攻击相关
    sf::Clock m_attackClock;
    float m_attackCooldown = 1.0f;
    float m_meleeRange = 35.f;
    bool m_attacking = false;
    sf::Clock m_animAttackClock;

    // 自爆相关
    bool m_exploding = false;
    sf::Clock m_explodeClock;
    float m_explodeDelay = 0.5f;
    float m_explodeRange = 30.f;

    // 目标追踪（仇恨系统）
    sf::Vector2f m_targetPos;   // 当前追击的目标位置
    bool m_hasTarget = false;    // 是否有仇恨目标

public:
    Enemy(float x, float y, EnemyType t);
    ~Enemy() = default;

    static bool loadTextures(const std::string& normalPath, const std::string& fastPath);

    void update(const std::vector<Tower>& towers, const sf::Vector2f& crystalPos,
        std::vector<Bullet>& enemyBullets, bool playerAttacked,
        const sf::Vector2f& playerPos);
    void draw(sf::RenderWindow& w) const;
    void takeDamage(int d);

    // 仇恨系统接口
    void setTarget(const sf::Vector2f& target);
    void clearTarget();

    // 原有 getter/setter
    bool isActive() const { return m_active; }
    void setActive(bool a) { m_active = a; }
    int getHp() const { return m_hp; }
    int getMaxHp() const { return m_maxHp; }
    EnemyType getType() const { return m_type; }
    sf::Vector2f getPos() const { return m_pos; }
    sf::FloatRect getBounds() const {
        return m_useSprite ? m_sprite.getGlobalBounds() : m_shape.getGlobalBounds();
    }
    bool isAttacking() const { return m_attacking; }
    void resetAttack() { m_attacking = false; }
    void clearPlayerAggro() { m_hasTarget = false; }
    bool canAttackPlayer() const { return m_attackClock.getElapsedTime().asSeconds() > m_attackCooldown; }
    void resetAttackClock() { m_attackClock.restart(); }
    void setSpeed(float multiplier) { m_speed *= multiplier; }

private:
    void updateAnimation();
};