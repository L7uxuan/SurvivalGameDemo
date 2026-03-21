#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <string>

class EnemyHealthBar {
    sf::RectangleShape m_bg, m_fg;
public:
    EnemyHealthBar() {
        m_bg.setSize({ 40,5 });
        m_bg.setFillColor({ 50,50,50 });
        m_bg.setOutlineColor(sf::Color::White);
        m_bg.setOutlineThickness(1);
        m_fg.setSize({ 40,5 });
        m_fg.setFillColor(sf::Color::Red);
    }
    void draw(sf::RenderWindow& w, const Enemy& e, const sf::Font* font) {
        if (!e.isActive()) return;
        float p = float(e.getHp()) / e.getMaxHp();
        auto b = e.getBounds();
        float x = b.left + b.width / 2 - 20;
        float y = b.top - 25;
        m_bg.setPosition(x, y);
        m_fg.setPosition(x, y);
        m_fg.setSize({ 40 * p, 5 });
        w.draw(m_bg); w.draw(m_fg);

        if (font) {
            sf::Text t(std::to_wstring(e.getHp()), *font, 16);
            t.setFillColor(sf::Color::White);
            auto tb = t.getLocalBounds();
            t.setPosition(b.left + b.width / 2 - tb.width / 2, y - 18);
            w.draw(t);
        }
    }
};