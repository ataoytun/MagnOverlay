#pragma once
#include <windows.h>
#include <random>
#include <ctime>

class MovingCircle {
public:
    MovingCircle(float radius = 30.0f);
    ~MovingCircle();

    void Update();
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetRadius() const { return m_radius; }

private:
    float m_x;
    float m_y;
    float m_radius;
    float m_speedX;
    float m_speedY;
    float m_updateTimer;

    std::mt19937 m_rng;
    std::uniform_real_distribution<float> m_dist;
}; 