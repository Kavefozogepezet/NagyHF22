// CollisionSys.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>

#include "Shapes/Polygon.h"
#include "general/sfmlmath.h"

sf::VertexArray* calculateMdiff(const CollSys::AbstractShape& s1, const CollSys::AbstractShape& s2, size_t r = 64) {
    sf::VertexArray* arr = new sf::VertexArray(sf::LineStrip, r);
    float angle = 360.0f / static_cast<float>(r);
    sf::Vector2f dir(1.0, 0.0);

    for (size_t i = 0; i < r; i++) {
        (*arr)[i].color = sf::Color::Green;

        sf::Vector2f p1 = s1.support(dir);
        sf::Vector2f p2 = s2.support(-dir);
        (*arr)[i].position = p1 - p2;

        dir = sfmath::rotate(dir, 360.0f / 32.0f);
    }
    return arr;
}

int main() {
    sf::RenderWindow win(sf::VideoMode(640, 480), "Test");
    win.setFramerateLimit(30);
    sf::View view({ 0.0f, 0.0f }, { 4.0f, 4.0f });
    win.setView(view);

    CollSys::Polygon t1, t2;
    t1.setPosition({ -1.0, 0.0 });
    t1.setScale({ 0.5f, 0.5f });
    t1.setRotation(30.0f);
    t2.setPosition({ -0.5f, 0.0f });
    t2.setScale({ 0.5f, 0.5f });

    sf::VertexArray* Mdif = nullptr;

    while (win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                win.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            t2.rotate(2.0f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            t2.rotate(-2.0f);
        }

        if (Mdif) { delete Mdif; }
        Mdif = calculateMdiff(t1, t2);
        
        win.clear(sf::Color::Black);
        win.draw(*Mdif);
        win.draw(t1);
        win.draw(t2);
        win.display();
    }
    win.close();

    return 0;
}
