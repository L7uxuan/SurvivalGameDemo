#include "Player.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

Player::Player() {
    m_shape.setSize({ 15,15 });
    m_shape.setFillColor(sf::Color::Green);
    m_pos = { 400,300 };
}

bool Player::loadTex(const std::string& base) {
    auto load = [&](const std::string& dir, std::vector<sf::Texture>& vec) -> bool {
        for (int i = 0; i < 12; i++) {
            std::ostringstream ss;
            ss << base << "rifleman_" << dir << "_" << std::setw(3) << std::setfill('0') << i << ".png";
            sf::Texture t;
            if (!t.loadFromFile(ss.str())) {
                std::cout << "Failed to load " << ss.str() << std::endl;
                return false;
            }
            vec.push_back(t);
        }
        return true;
        };
    if (!load("back", m_back) || !load("front", m_front) ||
        !load("left", m_left) || !load("right", m_right)) return false;
    m_sprite.setTexture(m_front[0]);
    m_sprite.setScale(20.f / 160.f, 20.f / 160.f);
    m_useSprite = true;
    return true;
}

void Player::handleInput() {
    m_moving = false;
    sf::Vector2f move;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { move.y -= 1; m_dir = BACK; m_moving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { move.y += 1; m_dir = FRONT; m_moving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { move.x -= 1; m_dir = LEFT; m_moving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { move.x += 1; m_dir = RIGHT; m_moving = true; }

    if (m_moving) {
        if (move.x != 0 && move.y != 0) move /= std::sqrt(2.f);
        m_pos += move * (m_speed / 60.f);
        m_pos.x = std::max(0.f, std::min(770.f, m_pos.x));
        m_pos.y = std::max(0.f, std::min(570.f, m_pos.y));
    }
    else if (m_faceMouse) {
        auto d = m_mousePos - m_pos;
        m_dir = std::abs(d.x) > std::abs(d.y) ? (d.x > 0 ? RIGHT : LEFT) : (d.y > 0 ? FRONT : BACK);
    }
}

void Player::update() {
    if (!m_useSprite) { m_shape.setPosition(m_pos); return; }
    auto* set = m_dir == FRONT ? &m_front : m_dir == BACK ? &m_back : m_dir == LEFT ? &m_left : &m_right;
    if (set->empty()) return;
    if (m_moving) {
        if (m_animClock.getElapsedTime().asSeconds() > m_frameTime) {
            m_frame = (m_frame + 1) % set->size();
            m_sprite.setTexture((*set)[m_frame]);
            m_animClock.restart();
        }
    }
    else {
        m_sprite.setTexture((*set)[0]);
        m_frame = 0;
    }
    m_sprite.setPosition(m_pos);
}

void Player::draw(sf::RenderWindow& w) {
    if (m_useSprite) w.draw(m_sprite);
    else w.draw(m_shape);
}

sf::Vector2f Player::getCenter() const {
    auto b = getBounds();
    return { b.left + b.width / 2, b.top + b.height / 2 };
}

sf::FloatRect Player::getBounds() const {
    return m_useSprite ? m_sprite.getGlobalBounds() : m_shape.getGlobalBounds();
}

int Player::getAtk() {
    int d = m_atkMin + rand() % (m_atkMax - m_atkMin + 1);
    return (rand() % 100 < m_crit * 100) ? d * 2 : d;
}

bool Player::useEnergy(float p) {
    float need = m_maxEnergy * p / 100.f;
    if (m_energy < need) return false;
    m_energy -= need;
    return true;
}