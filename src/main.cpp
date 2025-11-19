#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Robot Control", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // --- Robot SOLO ---
    sf::Texture tex;
    sf::Sprite robot;
    if (!tex.loadFromFile("img/robot.png")) {
        sf::Image img; 
        img.create(80, 120, sf::Color(200, 50, 50));
        tex.loadFromImage(img);
    }
    
    robot.setTexture(tex);
    robot.setScale(0.25f, 0.25f);
    robot.setOrigin(tex.getSize().x/2.f, tex.getSize().y/2.f);
    robot.setPosition(960.f, 540.f);

    // --- Variables ---
    float speed = 0.f;
    int gear = 1;
    const float gearSpeeds[4] = {0.f, 2.07f, 2.88f, 4.05f};
    const float reverseSpeed = -1.38f;
    const float acceleration = 0.1f;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed || 
               (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape))
                window.close();
        }

        if (sf::Joystick::isConnected(0)) {
            bool L1 = sf::Joystick::isButtonPressed(0, 4);
            float stickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
            
            if (std::abs(stickX) > 25.f) {
                float sensitivity = 0.02f;
                float rotationDirection = (speed < -0.1f) ? -1.0f : 1.0f;
                robot.rotate(stickX * sensitivity * rotationDirection);
            }

            if (L1) {
                if (sf::Joystick::isButtonPressed(0, 7)) {
                    speed = std::min(speed + acceleration, gearSpeeds[gear]);
                }
                else if (sf::Joystick::isButtonPressed(0, 6)) {
                    speed = std::max(speed - acceleration, reverseSpeed);
                }
                else {
                    speed *= 0.9f;
                    if (std::abs(speed) < 0.05f) speed = 0.0f;
                }
            } else {
                speed *= 0.7f;
                if (std::abs(speed) < 0.02f) speed = 0.0f;
            }

            static bool triPrev = false, xPrev = false;
            bool tri = sf::Joystick::isButtonPressed(0, 3);
            bool x = sf::Joystick::isButtonPressed(0, 1);

            if (tri && !triPrev && gear < 3) gear++;
            if (x && !xPrev && gear > 1) gear--;
            triPrev = tri; xPrev = x;
        }

        if (std::abs(speed) > 0.01f) {
            float rad = robot.getRotation() * 3.141592f / 180.f;
            robot.move(std::cos(rad) * speed, std::sin(rad) * speed);
        }

        // Límites normales
        sf::FloatRect b = robot.getGlobalBounds();
        sf::Vector2f p = robot.getPosition();
        float margin = b.width / 2;
        p.x = std::max(margin, std::min(1920.f - margin, p.x));
        p.y = std::max(margin, std::min(1080.f - margin, p.y));
        robot.setPosition(p);

        // Dibujar SOLO robot
        window.clear(sf::Color::Black);
        window.draw(robot);

        // HUD simple
        sf::Font font; 
        if (font.loadFromFile("arial.ttf")) {
            sf::Text t;
            t.setFont(font); 
            t.setCharacterSize(24); 
            t.setPosition(30,20);
            t.setString("Vel: " + std::to_string(speed).substr(0,4) + " m/s\nGear: " + std::to_string(gear));
            t.setFillColor(sf::Color::White);
            window.draw(t);
        }

        window.display();
    }
    return 0;
}