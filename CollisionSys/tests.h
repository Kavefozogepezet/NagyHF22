#include <iostream>
#include <thread>
#include <chrono>
#include <iostream>

#include "debug/memtrace.h"
#include "debug/gtest_lite.h"

#include "Shapes/Shapes.h"

#include "general/string.h"
#include "general/list.h"
#include "general/vec2.h"

#include "general/linebuffer.h"

#include "GJK.h"

const float speed = 0.02f;
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
    const glib::vec2d& dir, glib::vec2d& point)
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

void RenderPass(sf::RenderWindow& win, glib::list<CollSys::AbstractShape*>& objs, sf::VertexArray& debug) {
    win.clear(sf::Color::Black);
    for (auto obj : objs) {
        win.draw(*obj);
    }
    win.draw(debug);
    win.display();
}

void ContactPass(glib::list<CollSys::AbstractShape*>& objs) {
    for (auto obj : objs) {
        obj->setColor(sf::Color::White);
    }
    for (auto obj1 : objs) {
        for (auto obj2 : objs) {
            if (obj1 != obj2) {
                CollSys::GJKSolver gjk_test(*obj1, *obj2);
                if (gjk_test.isContact()) {
                    obj1->setColor(sf::Color::Red);
                    obj2->setColor(sf::Color::Red);
                }
            }
        }
    }
}

void main_test() {
    sf::RenderWindow win(sf::VideoMode(800, 800), "Test");
    win.setFramerateLimit(30);
    sf::View view({ 0.0f, 0.0f }, { 2.0f, 2.0f });
    win.setView(view);

    glib::list<CollSys::AbstractShape*> objs;

    CollSys::Ellipse t1;
    CollSys::Circle t2(0.4);
    CollSys::Polygon t3;
    CollSys::Polygon t4 = {
        { -1.0, 0.0 },
        { 0.0, 0.5 },
        { 1.0, 0.1 },
        { 0.5, -0.5 }
    };


    /*
    t1.setPosition({ -0.4, 0.0 }).setScale({ 0.4f, 0.4f }).setRotation(30.0f);
    t2.setScale({ 0.4f, 0.4f });
    */
    objs.push_back(&t1);
    objs.push_back(&t2);
    objs.push_back(&t3);
    objs.push_back(&t4);

    sf::VertexArray simplex(sf::LineStrip, 2), Mdiff(sf::LineStrip, 0);
    calcFirstSimplex(t3, t4, simplex);

    sf::CircleShape centre(0.005f);
    centre.setOrigin(0.005f, 0.005f);
    centre.setFillColor(sf::Color::Red);
    /*
    sf::CircleShape centre2(0.005f);
    centre2.setOrigin(0.005f, 0.005f);
    centre2.setFillColor(sf::Color::Green);
    objs.push_back(&centre2);
    */
    CollSys::AbstractShape* selected = nullptr;

    while (win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                win.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
                    glib::vec2d worldPos = glib::VectorCast<double>(win.mapPixelToCoords(pixelPos));
                    CollSys::Point point(worldPos);

                    for (auto obj : objs) {
                        CollSys::GJKSolver point_test(*obj, point);
                        if (point_test.isContact()) {
                            selected = obj;
                            break;
                        }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    selected = nullptr;
                }
            }
        }
        if (selected) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { selected->rotate(angular); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { selected->rotate(-angular); }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { selected->move(-speed, 0.0f); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { selected->move(0.0f, -speed); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { selected->move(0.0f, speed); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { selected->move(speed, 0.0f); }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { selected->scale(1.0, 0.96); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) { selected->scale(1.0, 1.04); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) { selected->scale(0.96, 1.0); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) { selected->scale(1.04, 1.0); }
        }

        ContactPass(objs);
        if (selected) {
            selected->setColor(sf::Color::Yellow);
        }
        win.clear(sf::Color::Black);
        for (auto obj : objs) {
            win.draw(*obj);
        }
        win.display();
    }
    win.close();
}

inline void printVertexArray(glib::VertexArray& arr) {
    for (glib::vec2d& v : arr) {
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

void linebuff_test() {
    glib::linebuffer lbuff;
    std::cin >> lbuff;

    char c1, c2;
    glib::string str;
    lbuff.get(c1);

    lbuff >> c2 >> str;
    std::cout << c1 << " " << c2 << " " << str << std::endl;
}

#ifndef COLLSYS_MAIN
int main() {
    main_test();
    return 0;
}
#endif
