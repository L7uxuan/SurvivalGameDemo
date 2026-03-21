#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class DamageNumber {
    int m_val;
    sf::Vector2f m_pos;
    sf::Clock m_life;
    bool m_active;
    sf::Color m_color;
public:
    DamageNumber(int v, sf::Vector2f p, sf::Color c = sf::Color::White) : m_val(v), m_pos(p), m_color(c), m_active(true) {}
    void update() { m_pos.y -= 0.5f; if (m_life.getElapsedTime().asSeconds() > 1.f) m_active = false; }
    void draw(sf::RenderWindow& w, const sf::Font* f) {
        if (!m_active || !f) return;
        sf::Text t(std::to_wstring(m_val), *f, 20);
        t.setFillColor(m_color);
        t.setPosition(m_pos);
        w.draw(t);
    }
    bool isActive() const { return m_active; }
};