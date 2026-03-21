#pragma once
#include <SFML/Graphics.hpp>

class SlashEffect {
    sf::RectangleShape m_shape;
    sf::Clock m_life;
    bool m_active = true;
public:
    SlashEffect(sf::Vector2f pos, float angle) {
        m_shape.setSize({ 40,20 });
        m_shape.setFillColor({ 255,0,0,150 });
        m_shape.setOrigin(20, 10);
        m_shape.setPosition(pos);
        m_shape.setRotation(angle * 180 / 3.14159f);
    }
    void update() {
        float e = m_life.getElapsedTime().asSeconds();
        if (e > 0.2f) m_active = false;
        else {
            auto c = m_shape.getFillColor();
            c.a = static_cast<sf::Uint8>(150 * (1 - e / 0.2f));
            m_shape.setFillColor(c);
        }
    }
    void draw(sf::RenderWindow& w) { if (m_active) w.draw(m_shape); }
    bool isActive() const { return m_active; }
};