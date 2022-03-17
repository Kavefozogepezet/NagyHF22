// CollisionSys.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <iostream>

#include "debug/memtrace.h"
#include "debug/gtest_lite.h"

#include "Shapes/Polygon.h"
#include "Shapes/Circle.h"
#include "Shapes/Ellipse.h"
#include "general/sfmlmath.h"

#include "general/string.h"

#include "general/list.h"
#include "general/vec2.h"

const float speed = 0.02f;
const float angular = 2.0f;

void calcMdiff(const CollSys::AbstractShape& s1, const CollSys::AbstractShape& s2, sf::VertexArray& arr, size_t r = 64) {
    arr.resize(r);
    float angle = 360.0f / static_cast<float>(r);
    sf::Vector2f dir(1.0, 0.0);

    for (size_t i = 0; i < r; i++) {
        arr[i].color = sf::Color(100, 100, 100);

        sf::Vector2f p1 = s1.support(dir);
        sf::Vector2f p2 = s2.support(-dir);
        arr[i].position = p1 - p2;

        dir = sfmath::rotate(dir, 360.0f / 32.0f);
    }
}

bool calcSupPoint(
    const CollSys::AbstractShape& s1, const CollSys::AbstractShape& s2,
    const sf::Vector2f& dir, sf::Vector2f& point )
{
    point = s1.support(dir) - s2.support(-dir);
    return sfmath::dot(point, dir) > 0.0f;
}

void calcSimplex(const CollSys::AbstractShape& s1, const CollSys::AbstractShape& s2, sf::VertexArray& simplex) {
    sf::Color col = sf::Color::Yellow;
    sf::Vector2f
        svec = simplex[0].position - simplex[1].position,
        origo = sf::Vector2f(0.0f, 0.0f) - simplex[1].position,
        n = sfmath::normalInDir(svec, origo),
        new_point;
    if (!calcSupPoint(s1, s2, n, new_point)) {
        col = sf::Color::Red;
    }
    sf::Vector2f
        dir1 = sfmath::normalInDir(simplex[0].position - new_point, -(simplex[1].position - new_point)),
        dir2 = sfmath::normalInDir(simplex[1].position - new_point, -(simplex[0].position - new_point)),
        np2o = -new_point;
    if (sfmath::dot(dir1, np2o) > 0.0f) {
        simplex[1] = new_point;
    }
    else if (sfmath::dot(dir2, np2o) > 0.0f) {
        simplex[0] = new_point;
    }
    else {
        std::cout << "jeee" << std::endl; 
        simplex.resize(3);
        simplex.setPrimitiveType(sf::Triangles);
        simplex[2].position = new_point;
        col = sf::Color::Green;
    }
    for (size_t i = 0; i < simplex.getVertexCount(); i++) {
        simplex[i].color = col;
    }
}

void calcFirstSimplex(const CollSys::AbstractShape& s1, const CollSys::AbstractShape& s2, sf::VertexArray& simplex) {
    simplex.resize(2);
    simplex.setPrimitiveType(sf::LineStrip);
    
    sf::Color col = sf::Color::Yellow;
    sf::Vector2f b = { 1.0f, 0.0f };
    if (!calcSupPoint(s1, s2, b, simplex[0].position)) {
        col = sf::Color::Red;
    }
    b = sf::Vector2f(0.0f, 0.0f) - simplex[0].position;
    if (!calcSupPoint(s1, s2, b, simplex[1].position)) {
        col = sf::Color::Red;
    }
    simplex[0].color = simplex[1].color = col;
}

void main_test() {
    sf::RenderWindow win(sf::VideoMode(800, 800), "Test");
    win.setFramerateLimit(30);
    sf::View view({ 0.0f, 0.0f }, { 8.0f, 8.0f });
    win.setView(view);

    glib::list<sf::Drawable*> objs;

    sf::CircleShape c(0.02f);
    c.setOrigin(0.02f, 0.02f);
    c.setFillColor(sf::Color::Red);
    objs.push_back(&c);

    CollSys::Polygon t1, t2;
    t1.setPosition({ -1.0, 0.0 });
    t1.setScale({ 0.5f, 0.5f });
    t1.setRotation(30.0f);
    t2.setScale({ 0.5f, 0.5f });
    objs.push_back(&t1);
    objs.push_back(&t2);

    sf::VertexArray simplex(sf::LineStrip, 2), Mdiff(sf::LineStrip, 0);
    calcFirstSimplex(t1, t2, simplex);
    objs.push_back(&Mdiff);
    objs.push_back(&simplex);

    while (win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                win.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::C) {
                    calcSimplex(t1, t2, simplex);
                }
                if (event.key.code == sf::Keyboard::U) {
                    calcFirstSimplex(t1, t2, simplex);
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            t2.rotate(angular);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            t2.rotate(-angular);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            t2.move(-speed, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            t2.move(0.0f, -speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            t2.move(0.0f, speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            t2.move(speed, 0.0f);
        }
        calcMdiff(t1, t2, Mdiff);

        win.clear(sf::Color::Black);
        for (auto obj : objs) {
            win.draw(*obj);
        }
        win.display();
    }
    win.close();
}

inline void printVertexArray(glib::VertexArray& arr) {
    for(glib::vec2d& v : arr) {
        std::cout << "vec:" << v.x << " " << v.y << std::endl;
    }
}

void array_test() {
    glib::VertexArray va(2);
    va.resize(3);
    for (auto& v : va) {
        v = { 1.0, 1.1 };
    }

    /*
    for (size_t i = 0; i < 3; i++) {
        double d = static_cast<double>(i);
        va[i] = glib::vec2d(d, d);
    }
    */
    printVertexArray(va);
}

int main() {
    array_test();
    return 0;
}
