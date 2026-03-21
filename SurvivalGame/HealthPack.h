#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class HealthPack {
    sf::RectangleShape m_base;
    sf::RectangleShape m_crossV, m_crossH;
    sf::Vector2f m_pos;
    bool m_active = true;
    int m_heal = 30;
    sf::Clock m_lifeClock;
public:
    HealthPack(float x, float y) : m_pos({ x,y }) {
        m_base.setSize({ 25,25 });
        m_base.setFillColor(sf::Color::White);
        m_base.setOutlineColor(sf::Color::Black);
        m_base.setOutlineThickness(1);
        m_crossV.setSize({ 5,15 }); m_crossV.setFillColor(sf::Color::Red);
        m_crossH.setSize({ 15,5 }); m_crossH.setFillColor(sf::Color::Red);
        m_crossV.setOrigin(2.5f, 7.5f);
        m_crossH.setOrigin(7.5f, 2.5f);
    }
    void update() {
        if (!m_active) return;
        if (m_lifeClock.getElapsedTime().asSeconds() > 5.f) m_active = false;
        float off = std::sin(m_lifeClock.getElapsedTime().asSeconds() * 5) * 3;
        m_base.setPosition(m_pos.x, m_pos.y + off);
        m_crossV.setPosition(m_pos.x + 12.5f, m_pos.y + 12.5f + off);
        m_crossH.setPosition(m_pos.x + 12.5f, m_pos.y + 12.5f + off);
    }
    void draw(sf::RenderWindow& w) {
        if (!m_active) return;
        w.draw(m_base); w.draw(m_crossV); w.draw(m_crossH);
    }
    bool isActive() const { return m_active; }
    void setActive(bool a) { m_active = a; }
    int getHealAmount() const { return m_heal; }
    sf::FloatRect getBounds() const { return m_base.getGlobalBounds(); }
};