#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

class Crystal {
    sf::Sprite m_sprite;
    sf::RectangleShape m_shape;
    sf::RectangleShape m_inner;
    sf::Vector2f m_pos;
    int m_hp;
    int m_maxHp;
    bool m_useSprite = false;

    static inline sf::Texture m_texture;
    static inline bool m_textureLoaded = false;

public:
    Crystal(float x, float y) : m_pos({ x, y }) {
        m_maxHp = 500;
        m_hp = m_maxHp;

        // 尺寸 240x240
        m_shape.setSize({ 240, 240 });
        m_shape.setFillColor(sf::Color(173, 216, 230));
        m_shape.setOrigin(120, 120);
        m_shape.setPosition(m_pos);

        // 内部白色方块 120x120
        m_inner.setSize({ 120, 120 });
        m_inner.setFillColor(sf::Color::White);
        m_inner.setOrigin(60, 60);
        m_inner.setPosition(m_pos);

        if (m_textureLoaded) {
            m_useSprite = true;
            m_sprite.setTexture(m_texture);
            float scale = 240.f / m_texture.getSize().x;
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

    void takeDamage(int d) { m_hp = std::max(0, m_hp - d); }

    void draw(sf::RenderWindow& w) const {
        if (m_useSprite) w.draw(m_sprite);
        else {
            w.draw(m_shape);
            w.draw(m_inner);
        }
    }

    void drawHealthBar(sf::RenderWindow& w, const sf::Font* font) const {
        // 血条位于屏幕最顶端中央
        float barWidth = 200.f;
        float barHeight = 12.f;
        float barX = 400 - barWidth / 2;  // 屏幕宽度800，居中
        float barY = 10.f;               // 屏幕顶端留10像素

        sf::RectangleShape bg({ barWidth, barHeight });
        bg.setFillColor({ 50,50,50 });
        bg.setOutlineColor(sf::Color::White);
        bg.setOutlineThickness(1);
        bg.setPosition(barX, barY);
        w.draw(bg);

        float p = float(m_hp) / m_maxHp;
        sf::RectangleShape fg({ barWidth * p, barHeight });
        fg.setFillColor(sf::Color::Green);
        fg.setPosition(barX, barY);
        w.draw(fg);

        if (font) {
            sf::Text t(std::to_wstring(m_hp) + L"/" + std::to_wstring(m_maxHp), *font, 18);
            t.setFillColor(sf::Color::White);
            auto b = t.getLocalBounds();
            t.setPosition(barX + barWidth / 2 - b.width / 2, barY - 18);
            w.draw(t);
        }
    }

    sf::Vector2f getPos() const { return m_pos; }
    sf::FloatRect getBounds() const {
        if (m_useSprite) return m_sprite.getGlobalBounds();
        else return m_shape.getGlobalBounds();
    }
    int getHp() const { return m_hp; }
    int getMaxHp() const { return m_maxHp; }
};