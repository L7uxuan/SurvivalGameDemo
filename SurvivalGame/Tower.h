#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Bullet.h"
#include "Enemy.h"

class Tower {
    sf::Sprite m_sprite;
    sf::RectangleShape m_shape;
    sf::Vector2f m_pos;
    int m_hp = 100, m_maxHp = 100;
    sf::Clock m_shootClock;
    float m_cooldown = 0.2f;
    bool m_active = true;
    bool m_useSprite = false;

    static inline sf::Texture m_texture;
    static inline bool m_textureLoaded = false;

public:
    Tower(float x, float y) : m_pos({ x, y }) {
        m_shape.setSize({ 15, 15 });
        m_shape.setFillColor(sf::Color::Magenta);
        m_shape.setOrigin(7.5f, 7.5f);
        m_shape.setPosition(m_pos);

        if (m_textureLoaded) {
            m_useSprite = true;
            m_sprite.setTexture(m_texture);
            float scale = 60.f / m_texture.getSize().x;
            m_sprite.setScale(scale, scale);
            m_sprite.setOrigin(m_texture.getSize().x / 2.f, m_texture.getSize().y / 2.f);
            m_sprite.setPosition(m_pos);
        }
    }

    static bool loadTexture(const std::string& filename) {
        if (m_texture.loadFromFile(filename)) {
            m_textureLoaded = true;
            return true;
        }
        return false;
    }

    void update(std::vector<Enemy>& enemies, std::vector<Bullet>& bullets) {
        if (!m_active) return;
        if (m_shootClock.getElapsedTime().asSeconds() > m_cooldown) {
            Enemy* target = nullptr;
            float minDist = 1e9;
            for (auto& e : enemies) {
                if (!e.isActive()) continue;
                float d = std::hypot(e.getPos().x - m_pos.x, e.getPos().y - m_pos.y);
                if (d < minDist) {
                    minDist = d;
                    target = &e;
                }
            }
            if (target) {
                // 关键：防御塔子弹的 fromPlayer 参数为 false
                bullets.emplace_back(m_pos, target->getPos(), false, 20, false);
            }
            m_shootClock.restart();
        }
    }

    void draw(sf::RenderWindow& w) const {
        if (!m_active) return;
        if (m_useSprite) w.draw(m_sprite);
        else w.draw(m_shape);
    }

    void takeDamage(int d) { if ((m_hp -= d) <= 0) m_active = false; }
    sf::Vector2f getPos() const { return m_pos; }
    sf::FloatRect getBounds() const {
        if (m_useSprite) return m_sprite.getGlobalBounds();
        else return m_shape.getGlobalBounds();
    }
    bool isActive() const { return m_active; }
    int getHp() const { return m_hp; }
    int getMaxHp() const { return m_maxHp; }
};