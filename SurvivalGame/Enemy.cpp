#include "Enemy.h"
#include "Tower.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <iostream>

// 静态成员初始化
std::vector<sf::Texture> Enemy::m_normalTextures;
std::vector<sf::Texture> Enemy::m_fastTextures;
bool Enemy::m_texturesLoaded = false;

Enemy::Enemy(float x, float y, EnemyType t) : m_type(t) {
    m_pos = { x, y };
    m_useSprite = false;
    m_currentFrame = 0;
    m_frameDuration = 0.1f;
    m_isMoving = false;

    switch (t) {
    case EnemyType::NORMAL:
        // 近战兵尺寸 4x4
        m_shape.setSize({ 4, 4 });
        m_color = sf::Color(255, 182, 193);
        m_speed = 135.f / 60.f * 1.2f;
        m_maxHp = 100;
        break;
    case EnemyType::FAST:
        // 自爆兵尺寸 2x2
        m_shape.setSize({ 2, 2 });
        m_color = sf::Color(139, 0, 0);
        m_speed = 180.f / 60.f * 1.2f;
        m_maxHp = 50;
        break;
    }
    m_hp = m_maxHp;
    m_shape.setFillColor(m_color);

    if (m_texturesLoaded) {
        const auto& texSet = (m_type == EnemyType::NORMAL) ? m_normalTextures : m_fastTextures;
        if (!texSet.empty()) {
            m_useSprite = true;
            m_sprite.setTexture(texSet[0]);
            float scale = m_shape.getSize().x / 40.f;
            m_sprite.setScale(scale, scale);
            m_sprite.setPosition(m_pos);
        }
    }
}

bool Enemy::loadTextures(const std::string& normalPath, const std::string& fastPath) {
    sf::Texture tex;
    const int frameCount = 18;

    for (int i = 0; i < frameCount; ++i) {
        std::ostringstream oss;
        oss << normalPath << "/landmaster_" << std::setw(3) << std::setfill('0') << i << ".png";
        if (!tex.loadFromFile(oss.str())) {
            std::cout << "Failed to load " << oss.str() << " (will use rectangle)" << std::endl;
            return false;
        }
        m_normalTextures.push_back(tex);
    }

    for (int i = 0; i < frameCount; ++i) {
        std::ostringstream oss;
        oss << fastPath << "/flyingmaster_" << std::setw(3) << std::setfill('0') << i << ".png";
        if (!tex.loadFromFile(oss.str())) {
            std::cout << "Failed to load " << oss.str() << " (will use rectangle)" << std::endl;
            return false;
        }
        m_fastTextures.push_back(tex);
    }

    m_texturesLoaded = true;
    std::cout << "Enemy textures loaded successfully." << std::endl;
    return true;
}

void Enemy::updateAnimation() {
    if (!m_useSprite) return;
    const auto& texSet = (m_type == EnemyType::NORMAL) ? m_normalTextures : m_fastTextures;
    if (texSet.empty()) return;
    if (m_isMoving) {
        if (m_animClock.getElapsedTime().asSeconds() > m_frameDuration) {
            m_currentFrame = (m_currentFrame + 1) % texSet.size();
            m_sprite.setTexture(texSet[m_currentFrame]);
            m_animClock.restart();
        }
    }
    else {
        if (m_currentFrame != 0) {
            m_sprite.setTexture(texSet[0]);
            m_currentFrame = 0;
        }
    }
    m_sprite.setPosition(m_pos);
}

void Enemy::update(const std::vector<Tower>& towers, const sf::Vector2f& crystalPos,
    std::vector<Bullet>& enemyBullets, bool playerAttacked,
    const sf::Vector2f& playerPos) {
    if (!m_active) return;

    // 目标选择：优先追击仇恨目标
    sf::Vector2f target;
    bool hasValidTarget = false;

    if (m_hasTarget) {
        // 有仇恨目标，直接追击
        target = m_targetPos;
        hasValidTarget = true;
    }
    else {
        // 无仇恨时，默认目标：最近的塔，若无塔则水晶
        target = crystalPos;
        float minDist = std::hypot(target.x - m_pos.x, target.y - m_pos.y);
        for (const auto& t : towers) {
            if (!t.isActive()) continue;
            float d = std::hypot(t.getPos().x - m_pos.x, t.getPos().y - m_pos.y);
            if (d < minDist) {
                minDist = d;
                target = t.getPos();
            }
        }
        hasValidTarget = true;
    }

    if (!hasValidTarget) return;

    // 计算方向与距离
    sf::Vector2f dir = target - m_pos;
    float len = std::hypot(dir.x, dir.y);

    // 距离死区：非常接近目标时停止移动，防止抖动
    if (len < 5.f) {
        m_isMoving = false;
        // 仍然更新攻击标记
        if (len < m_meleeRange) {
            if (m_attackClock.getElapsedTime().asSeconds() > m_attackCooldown) {
                m_attacking = true;
                m_animAttackClock.restart();
                m_attackClock.restart();
            }
        }
    }
    else {
        // 移动
        dir /= len;
        m_pos += dir * m_speed;
        m_isMoving = true;
    }

    // 近战攻击标记（伤害由外部处理）
    if (len < m_meleeRange) {
        if (m_attackClock.getElapsedTime().asSeconds() > m_attackCooldown) {
            m_attacking = true;
            m_animAttackClock.restart();
            m_attackClock.restart();
        }
    }

    // 自爆逻辑（FAST 类型）
    if (m_type == EnemyType::FAST) {
        float distToPlayer = std::hypot(playerPos.x - m_pos.x, playerPos.y - m_pos.y);
        if (distToPlayer <= m_explodeRange && !m_exploding) {
            m_exploding = true;
            m_explodeClock.restart();
            if (!m_useSprite) m_shape.setFillColor(sf::Color(255, 0, 0));
        }
        if (m_exploding && m_explodeClock.getElapsedTime().asSeconds() > m_explodeDelay) {
            m_active = false;
        }
    }

    // 攻击动画复位
    if (m_attacking && m_animAttackClock.getElapsedTime().asSeconds() > 0.2f) {
        m_attacking = false;
    }

    // 边界软限制
    if (m_pos.x < 0) m_pos.x = 0;
    else if (m_pos.x > 800 - m_shape.getSize().x) m_pos.x = 800 - m_shape.getSize().x;
    if (m_pos.y < 0) m_pos.y = 0;
    else if (m_pos.y > 600 - m_shape.getSize().y) m_pos.y = 600 - m_shape.getSize().y;

    // 更新位置
    if (m_useSprite) updateAnimation();
    else m_shape.setPosition(m_pos);
}

void Enemy::draw(sf::RenderWindow& w) const {
    if (!m_active) return;
    if (m_useSprite) w.draw(m_sprite);
    else w.draw(m_shape);
}

void Enemy::takeDamage(int d) {
    m_hp -= d;
    if (m_hp <= 0) m_active = false;
}

void Enemy::setTarget(const sf::Vector2f& target) {
    m_targetPos = target;
    m_hasTarget = true;
}

void Enemy::clearTarget() {
    m_hasTarget = false;
}