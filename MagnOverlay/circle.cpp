#include "circle.h"

MovingCircle::MovingCircle(float radius) : m_x(0.0f), m_y(0.0f), m_radius(radius), m_speedX(2.0f), m_speedY(2.0f), m_updateTimer(0.0f), m_rng(static_cast<unsigned int>(time(nullptr))), m_dist(-1.0f, 1.0f) { }
MovingCircle::~MovingCircle() { }

void MovingCircle::Update() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    if (m_x == 0.0f && m_y == 0.0f) {
        m_x = screenWidth / 2.0f;
        m_y = screenHeight / 2.0f;
    }
    
    m_x += m_speedX;
    m_y += m_speedY;
    
    if (m_x - m_radius < 0) {
        m_x = m_radius;
        m_speedX = -m_speedX * 0.8f + m_dist(m_rng) * 1.5f;
    }
    else if (m_x + m_radius > screenWidth) {
        m_x = screenWidth - m_radius;
        m_speedX = -m_speedX * 0.8f + m_dist(m_rng) * 1.5f;
    }
    
    if (m_y - m_radius < 0) {
        m_y = m_radius;
        m_speedY = -m_speedY * 0.8f + m_dist(m_rng) * 1.5f;
    }
    else if (m_y + m_radius > screenHeight) {
        m_y = screenHeight - m_radius;
        m_speedY = -m_speedY * 0.8f + m_dist(m_rng) * 1.5f;
    }
    
    m_updateTimer += 0.016f;
    if (m_updateTimer > 2.0f) {
        m_updateTimer = 0.0f;
        m_speedX += m_dist(m_rng) * 3.0f;
        m_speedY += m_dist(m_rng) * 3.0f;
        const float maxSpeed = 5.0f;
        if (m_speedX > maxSpeed) m_speedX = maxSpeed;
        if (m_speedX < -maxSpeed) m_speedX = -maxSpeed;
        if (m_speedY > maxSpeed) m_speedY = maxSpeed;
        if (m_speedY < -maxSpeed) m_speedY = -maxSpeed;
    }
}