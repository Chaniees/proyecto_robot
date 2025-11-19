#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>

void keepInBounds(sf::Sprite& robot, sf::RenderWindow& window) {
    sf::Vector2f position = robot.getPosition();
    sf::FloatRect bounds = robot.getGlobalBounds();
    
    float margin = bounds.width / 2;
    if (position.x < margin) robot.setPosition(margin, position.y);
    if (position.x > window.getSize().x - margin) robot.setPosition(window.getSize().x - margin, position.y);
    if (position.y < margin) robot.setPosition(position.x, margin);
    if (position.y > window.getSize().y - margin) robot.setPosition(position.x, window.getSize().y - margin);
}

class Speedometer {
private:
    sf::Font font;
    sf::Text speedText;
    sf::Text gearText;
    sf::RectangleShape background;
    sf::CircleShape tacometer;
    sf::RectangleShape needle;
    
public:
    Speedometer() {
        if (!font.loadFromFile("arial.ttf")) {
            // Fuente por defecto si no se carga
        }
        
        // Fondo del tacómetro
        background.setSize(sf::Vector2f(200, 120));
        background.setFillColor(sf::Color(30, 30, 30, 200));
        background.setPosition(20, 920);
        
        // Círculo del tacómetro
        tacometer.setRadius(40);
        tacometer.setFillColor(sf::Color::Black);
        tacometer.setOutlineThickness(2);
        tacometer.setOutlineColor(sf::Color::White);
        tacometer.setPosition(140, 940);
        
        // Aguja del tacómetro
        needle.setSize(sf::Vector2f(35, 3));
        needle.setFillColor(sf::Color::Red);
        needle.setOrigin(0, 1.5f);
        needle.setPosition(180, 980);
        
        // Texto de velocidad
        speedText.setFont(font);
        speedText.setCharacterSize(24);
        speedText.setFillColor(sf::Color::White);
        speedText.setPosition(30, 930);
        
        // Texto de cambio
        gearText.setFont(font);
        gearText.setCharacterSize(32);
        gearText.setFillColor(sf::Color::Yellow);
        gearText.setPosition(80, 970);
    }
    
    void update(float speed, int gear, bool deadmanActive) {
        // Actualizar texto de velocidad
        std::stringstream speedStream;
        speedStream << std::fixed << std::setprecision(1) << std::abs(speed) << " m/s";
        speedText.setString(speedStream.str());
        
        // Actualizar texto de cambio
        if (!deadmanActive) {
            gearText.setString("P");
            gearText.setFillColor(sf::Color::White);
        } else if (gear == -1) {
            gearText.setString("R");
            gearText.setFillColor(sf::Color::Red);
        } else {
            gearText.setString("GEAR " + std::to_string(gear));
            gearText.setFillColor(sf::Color::Yellow);
        }
        
        // Actualizar aguja del tacómetro (0-180 grados para 0-5 m/s)
        float maxSpeed = 5.0f;
        float angle = (std::abs(speed) / maxSpeed) * 180.0f;
        needle.setRotation(angle);
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(tacometer);
        window.draw(needle);
        window.draw(speedText);
        window.draw(gearText);
    }
};

class CityEnvironment {
private:
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape road;
    std::vector<sf::RectangleShape> streets;
    std::vector<sf::Text> streetLabels;
    sf::Font font;
    
public:
    bool load() {
        // Intentar cargar imagen de fondo
        if (!bgTexture.loadFromFile("img/miami_map.jpg")) {
            // Crear fondo por defecto
            createDefaultBackground();
        } else {
            background.setTexture(bgTexture);
            background.setScale(
                1920.0f / bgTexture.getSize().x,
                1080.0f / bgTexture.getSize().y
            );
        }
        
        if (!font.loadFromFile("arial.ttf")) {
            // Fuente por defecto
        }
        
        createStreets();
        return true;
    }
    
    void createDefaultBackground() {
        // Crear un fondo urbano simple
        bgTexture.create(1920, 1080);
        sf::Image tempImage;
        tempImage.create(1920, 1080, sf::Color(100, 150, 200)); // Color cielo
        
        // Dibujar calles
        for (int y = 200; y < 1080; y += 150) {
            for (int x = 0; x < 1920; x++) {
                if (y % 150 == 0) {
                    for (int i = -2; i <= 2; i++) {
                        if (x + i >= 0 && x + i < 1920) {
                            tempImage.setPixel(x + i, y, sf::Color::Black);
                        }
                    }
                }
            }
        }
        
        bgTexture.loadFromImage(tempImage);
        background.setTexture(bgTexture);
    }
    
    void createStreets() {
        // Crear calles principales
        for (int y = 200; y < 1000; y += 150) {
            sf::RectangleShape street(sf::Vector2f(1920, 8));
            street.setFillColor(sf::Color(80, 80, 80));
            street.setPosition(0, y);
            streets.push_back(street);
            
            // Líneas divisorias
            for (int x = 0; x < 1920; x += 40) {
                sf::RectangleShape line(sf::Vector2f(20, 2));
                line.setFillColor(sf::Color::Yellow);
                line.setPosition(x, y + 3);
                streets.push_back(line);
            }
            
            // Etiquetas de calles
            sf::Text label;
            label.setFont(font);
            label.setCharacterSize(16);
            label.setFillColor(sf::Color::White);
            label.setString("NE " + std::to_string((y - 200) / 150 + 1) + " St");
            label.setPosition(20, y - 25);
            streetLabels.push_back(label);
        }
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(background);
        for (auto& street : streets) {
            window.draw(street);
        }
        for (auto& label : streetLabels) {
            window.draw(label);
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Robot Simulator PS5 - Delivery System");
    window.setFramerateLimit(60);

    // Cargar entorno de ciudad
    CityEnvironment city;
    city.load();

    // Crear robot
    sf::Texture robotTexture;
    sf::Sprite robot;
    
    if (!robotTexture.loadFromFile("img/robot.png")) {
        // Crear textura temporal de robot de delivery
        sf::Image tempImage;
        tempImage.create(80, 120, sf::Color(200, 50, 50)); // Color rojo delivery
        for (int i = 0; i < 80; ++i) {
            for (int j = 0; j < 120; ++j) {
                if (i == 0 || i == 79 || j == 0 || j == 119) {
                    tempImage.setPixel(i, j, sf::Color::White);
                }
                // Dibujar ventanas
                if (i > 10 && i < 35 && j > 20 && j < 50) {
                    tempImage.setPixel(i, j, sf::Color(150, 200, 255));
                }
                if (i > 45 && i < 70 && j > 20 && j < 50) {
                    tempImage.setPixel(i, j, sf::Color(150, 200, 255));
                }
            }
        }
        robotTexture.loadFromImage(tempImage);
        std::cout << "Usando robot de delivery temporal\n";
    }
    
    robot.setTexture(robotTexture);
    robot.setOrigin(robotTexture.getSize().x / 2, robotTexture.getSize().y / 2);
    robot.setPosition(400, 300);

    // Sistema de velocidad y cambios
    Speedometer speedometer;
    float currentSpeed = 0.0f;
    float acceleration = 0.1f;
    float deceleration = 0.2f;
    int currentGear = 0; // 0: P, 1-3: gears, -1: reversa
    bool deadmanActive = false;
    
    // Límites de velocidad por cambio
    const float gearSpeeds[] = {0.0f, 2.07f, 2.88f, 4.05f};
    const float reverseSpeed = -1.38f;

    float rotationSpeed = 3.0f;

    std::cout << "=== SISTEMA DE DELIVERY ===\n";
    std::cout << "Controles Mando PS5:\n";
    std::cout << "- L1: Deadman switch (activar motor)\n";
    std::cout << "- Stick izquierdo: Direccion\n";
    std::cout << "- R2: Acelerar\n";
    std::cout << "- L2: Reversa\n";
    std::cout << "- Triangulo: Subir cambio\n";
    std::cout << "- X: Bajar cambio\n";
    std::cout << "\nControles Teclado:\n";
    std::cout << "- WASD: Movimiento y direccion\n";
    std::cout << "- T: Subir cambio\n";
    std::cout << "- X: Bajar cambio\n";
    std::cout << "- ESC: Salir\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || 
               (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            
            // Cambio de marchas con teclado
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::T) {
                    // Subir cambio
                    if (currentGear < 3) currentGear++;
                }
                if (event.key.code == sf::Keyboard::X) {
                    // Bajar cambio
                    if (currentGear > -1) currentGear--;
                }
            }
        }

        // Control con teclado (WASD)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            currentSpeed = std::min(currentSpeed + acceleration, gearSpeeds[1]);
            deadmanActive = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            currentSpeed = std::max(currentSpeed - acceleration, reverseSpeed);
            deadmanActive = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            robot.rotate(-rotationSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            robot.rotate(rotationSpeed);
        }

        // Control con mando PS5
        if (sf::Joystick::isConnected(0)) {
            // Deadman switch (L1)
            deadmanActive = sf::Joystick::isButtonPressed(0, 4);
            
            // Stick izquierdo para dirección
            float leftX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
            if (std::abs(leftX) > 15.0f && deadmanActive) {
                robot.rotate(leftX * rotationSpeed / 100.0f);
            }
            
            // Acelerador (R2) - el eje Z positivo generalmente es R2
            float r2Axis = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);
            if (deadmanActive && r2Axis > 10.0f && currentGear > 0) {
                float targetSpeed = gearSpeeds[currentGear];
                currentSpeed = std::min(currentSpeed + acceleration, targetSpeed);
            }
            
            // Freno/Reversa (L2) - el eje Z negativo generalmente es L2
            float l2Axis = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);
            if (deadmanActive && l2Axis < -10.0f) {
                if (currentGear == -1) {
                    currentSpeed = std::max(currentSpeed - acceleration, reverseSpeed);
                } else {
                    currentSpeed = std::max(currentSpeed - deceleration, 0.0f);
                }
            }
            
            // Cambio de marchas con botones
            static bool trianglePressed = false;
            static bool xButtonPressed = false;
            
            if (sf::Joystick::isButtonPressed(0, 3) && !trianglePressed) { // Triangulo
                if (currentGear < 3) currentGear++;
                trianglePressed = true;
            } else if (!sf::Joystick::isButtonPressed(0, 3)) {
                trianglePressed = false;
            }
            
            if (sf::Joystick::isButtonPressed(0, 2) && !xButtonPressed) { // X
                if (currentGear > -1) currentGear--;
                xButtonPressed = true;
            } else if (!sf::Joystick::isButtonPressed(0, 2)) {
                xButtonPressed = false;
            }
        }

        // Aplicar movimiento si hay velocidad
        if (std::abs(currentSpeed) > 0.01f && deadmanActive) {
            float angleRad = robot.getRotation() * 3.14159f / 180.0f;
            robot.move(std::cos(angleRad) * currentSpeed, std::sin(angleRad) * currentSpeed);
        } else if (!deadmanActive) {
            // Frenado natural cuando no hay deadman
            currentSpeed *= 0.8f;
            if (std::abs(currentSpeed) < 0.1f) currentSpeed = 0.0f;
        }

        // Actualizar tacómetro
        speedometer.update(currentSpeed, currentGear, deadmanActive);

        keepInBounds(robot, window);

        // Dibujar todo
        window.clear(sf::Color::Black);
        city.draw(window);
        window.draw(robot);
        speedometer.draw(window);
        
        // Mostrar estado del deadman
        sf::Font font;
        if (font.loadFromFile("arial.ttf")) {
            sf::Text statusText;
            statusText.setFont(font);
            statusText.setCharacterSize(20);
            statusText.setFillColor(deadmanActive ? sf::Color::Green : sf::Color::Red);
            statusText.setString(deadmanActive ? "MOTOR ACTIVO" : "MOTOR APAGADO");
            statusText.setPosition(1700, 20);
            window.draw(statusText);
        }
        
        window.display();
    }

    return 0;
}