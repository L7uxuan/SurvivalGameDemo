#pragma once
#include <SFML/Graphics.hpp>

class TrailParticle {
    sf::RectangleShape m_shape;
    sf::Clock m_lifeClock;
    bool m_active = true;
public:
    TrailParticle(sf::Vector2f pos) {
        m_shape.setSize({ 30,30 });
        m_shape.setFillColor({ 255,255,255,100 });
        m_shape.setPosition(pos);
    }
    void update() {
        float e = m_lifeClock.getElapsedTime().asSeconds();
        if (e > 0.3f) m_active = false;
        else {
            auto c = m_shape.getFillColor();
            c.a = static_cast<sf::Uint8>(255 * (1 - e / 0.3f));
            m_shape.setFillColor(c);
        }
    }
    void draw(sf::RenderWindow& w) { if (m_active) w.draw(m_shape); }
    bool isActive() const { return m_active; }
};