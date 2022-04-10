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
    sf::View view({ 0.0f, 0.0f }, { 4.0f, 4.0f });
    win.setView(view);

    CollSys::BezierCurve t1("");

    sf::CircleShape sup_point(0.05f);
    sup_point.setOrigin(0.05f, 0.05f);
    sup_point.setFillColor(sf::Color::Red);

    sf::VertexArray va(sf::LineStrip, 2);
    va[0].color = va[1].color = sf::Color::Green;
    va[0].position = { 0.0f, 0.0f };

    glib::vec2d dir = { 1.0, 0.0 };

    while (win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                win.close();
            }
        }
        glib::vec2d p = t1.support(dir);
        va[1].position = glib::VectorCast(dir);
        sup_point.setPosition(glib::VectorCast(p));
        dir.rotate(2);

        win.clear(sf::Color::Black);

        win.draw(t1);
        win.draw(va);
        win.draw(sup_point);

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

class A
{
    friend std::ostream& operator << (std::ostream& stream, const A& a) {
        stream << "hello from A";
        return stream;
    }
};

class B : public A
{
    friend std::ostream& operator << (std::ostream& stream, const B& b) {
        stream << "hello from B";
        return stream;
    }
};

void misc_test() {
    B obj;
    A* obj_ptr = &obj;
    std::cout << *obj_ptr << std::endl;
}

#ifndef COLLSYS_MAIN
int main() {
    main_test();
    return 0;
}
#endif
