#pragma once
#include <SFML/Graphics.hpp>

class HealthBar {
    sf::RectangleShape m_bg, m_fg;
    sf::Vector2f m_pos;
public:
    HealthBar() {
        m_bg.setSize({ 200,15 });
        m_bg.setFillColor({ 50,50,50 });
        m_bg.setOutlineColor(sf::Color::White);
        m_bg.setOutlineThickness(1);
        m_fg.setSize({ 200,15 });
        m_pos = { 10,10 };
    }
    void setPosition(float x, float y) { m_pos = { x,y }; }
    void update(int cur, int max) {
        float p = float(cur) / max;
        m_fg.setSize({ 200 * p, 15 });
        if (p > 0.6f) m_fg.setFillColor(sf::Color::Green);
        else if (p > 0.2f) m_fg.setFillColor(sf::Color::Yellow);
        else m_fg.setFillColor(sf::Color::Red);
        m_bg.setPosition(m_pos);
        m_fg.setPosition(m_pos);
    }
    void draw(sf::RenderWindow& w) { w.draw(m_bg); w.draw(m_fg); }
};