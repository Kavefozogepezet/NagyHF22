#pragma once

#include "debug/memtrace.h"
#include "debug/gtest_lite.h"

#include "Shapes/Shapes.h"

#include "general/string.h"
#include "general/list.h"
#include "general/vec2.h"

#include "GJK.h"

namespace mdif {
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

    void test() {
        sf::RenderWindow win(sf::VideoMode(800, 800), "Test");
        win.setFramerateLimit(30);
        sf::View view({ 0.0f, 0.0f }, { 4.0f, 4.0f });
        win.setView(view);

        CollSys::BezierCurve bezier("");
        CollSys::Point point("");
        point.scale(0.5, 0.5);
        sf::VertexArray dif, simplex;

        calcMdiff(bezier, point, dif);
        calcFirstSimplex(bezier, point, simplex);

        sf::Event ev;
        while (win.waitEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                win.close();
            }
            else if (ev.type == sf::Event::KeyPressed) {
                switch (ev.key.code)
                {
                case sf::Keyboard::A:
                    point.move(-0.1, 0.0);
                    break;
                case sf::Keyboard::W:
                    point.move(0.0, -0.1);
                    break;
                case sf::Keyboard::S:
                    point.move(0.0, 0.1);
                    break;
                case sf::Keyboard::D:
                    point.move(0.1, 0.0);
                    break;
                case sf::Keyboard::E:
                    bezier.rotate(5);
                    break;
                case sf::Keyboard::Q:
                    bezier.rotate(-5);
                    break;
                case sf::Keyboard::U:
                    calcSimplex(bezier, point, simplex);
                    break;
                case sf::Keyboard::R:
                    calcMdiff(bezier, point, dif);
                    calcFirstSimplex(bezier, point, simplex);
                    break;
                default:
                    break;
                }
            }
            win.clear();
            win.draw(bezier);
            win.draw(point);
            win.draw(dif);
            win.draw(simplex);
            win.display();
        }
    }
}
