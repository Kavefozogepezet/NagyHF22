// CollisionSys.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <iostream>

#include "debug/memtrace.h"

#include "debug/memtrace.h"
#include "debug/gtest_lite.h"

#include "Shapes/Polygon.h"
#include "Shapes/Circle.h"
#include "Shapes/Ellipse.h"
#include "general/sfmlmath.h"

#include "general/string.h"

#include "general/list.h"
#include "general/vec2.h"

const float speed = 0.002f;
const float angular = 2.0f;

void calcMdiff(const CollSys::AbstractShape& s1, const CollSys::AbstractShape& s2, sf::VertexArray& arr, size_t r = 512) {
    glib::VertexArray temp(r);
    float angle = 360.0f / static_cast<float>(r);
    glib::vec2d dir(1.0, 0.0);

    for (size_t i = 0; i < r; i++) {
        glib::vec2d p1 = s1.support(dir);
        glib::vec2d p2 = s2.support(-dir);
        temp[i] = p1 - p2;
        dir.rotate(360.0 / static_cast<double>(r));
    }
    glib::VertexArrayCast(arr, temp, sf::Color(100, 100, 100));
}

bool calcSupPoint(
    const CollSys::AbstractShape& s1, const CollSys::AbstractShape& s2,
    const glib::vec2d& dir, glib::vec2d& point )
{
    point = s1.support(dir) - s2.support(-dir);
    return (point * dir) > 0.0f;
}

void calcSimplex(const CollSys::AbstractShape& s1, const CollSys::AbstractShape& s2, sf::VertexArray& simplex) {
    if (simplex.getVertexCount() == 3 || simplex[0].color == sf::Color::Red) {
        return;
    }
    sf::Color col = sf::Color::Yellow;
    glib::vec2d
        svec = glib::VectorCast<double>(simplex[0].position - simplex[1].position),
        origo = glib::VectorCast<double>(sf::Vector2f(0.0f, 0.0f) - simplex[1].position),
        n = glib::getNormal(svec, origo),
        new_point;
    if (!calcSupPoint(s1, s2, n, new_point)) {
        col = sf::Color::Red;
    }
    glib::vec2d
        dir1 = glib::getNormal(
            glib::VectorCast<double>(simplex[0].position) - new_point, 
            -(glib::VectorCast<double>(simplex[1].position) - new_point)),
        dir2 = glib::getNormal(
            glib::VectorCast<double>(simplex[1].position) - new_point,
            -(glib::VectorCast<double>(simplex[0].position) - new_point)),
        np2o = -new_point;
    if ((dir1 * np2o) > 0.0f) {
        simplex[1] = glib::VectorCast(new_point);
    }
    else if ((dir2 * np2o) > 0.0f) {
        simplex[0] = glib::VectorCast(new_point);
    }
    else {
        std::cout << "jeee" << std::endl; 
        simplex.resize(3);
        simplex.setPrimitiveType(sf::Triangles);
        simplex[2].position = glib::VectorCast(new_point);
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
    glib::vec2d b = { 1.0f, 0.0f }, temp;
    if (!calcSupPoint(s1, s2, b, temp)) {
        col = sf::Color::Red;
    }
    simplex[0].position = glib::VectorCast<double>(temp);

    b = glib::vec2d(0.0f, 0.0f) - glib::VectorCast<double>(simplex[0].position);
    if (!calcSupPoint(s1, s2, b, temp)) {
        col = sf::Color::Red;
    }
    simplex[1].position = glib::VectorCast<double>(temp);

    simplex[0].color = simplex[1].color = col;
}

void main_test() {
    sf::RenderWindow win(sf::VideoMode(800, 800), "Test");
    win.setFramerateLimit(30);
    sf::View view({ 0.0f, 0.0f }, { 3.0f, 3.0f });
    win.setView(view);

    glib::list<sf::Drawable*> objs;

    sf::CircleShape centre(0.005f);
    centre.setOrigin(0.005f, 0.005f);
    centre.setFillColor(sf::Color::Red);
    objs.push_back(&centre);

    CollSys::Polygon t1;
    CollSys::Ellipse t2;
    t1.setPosition({ -1.0, 0.0 });
    t1.setScale({ 0.4f, 0.4f });
    t1.setRotation(30.0f);
    //t2.setScale({ 0.5f, 0.5f });
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
    main_test();
    return 0;
}
