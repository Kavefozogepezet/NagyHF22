// CollisionSys.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>

#include "debug/memtrace.h"
#include "debug/gtest_lite.h"

#include "Shapes/Polygon.h"
#include "Shapes/Circle.h"
#include "Shapes/Ellipse.h"
#include "general/sfmlmath.h"

#include "general/glib/string.h"

#include "general/glib/list.h"
#include <list>

const float speed = 0.02f;
const float angular = 2.0f;

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

void maintest() {
    sf::RenderWindow win(sf::VideoMode(640, 480), "Test");
    win.setFramerateLimit(30);
    sf::View view({ 0.0f, 0.0f }, { 4.0f, 4.0f });
    win.setView(view);

    sf::CircleShape c(0.02f);
    c.setOrigin(0.02f, 0.02f);
    c.setFillColor(sf::Color::Red);
    CollSys::Polygon t1;
    t1.setPosition({ -1.0, 0.0 });
    t1.setScale({ 0.5f, 0.5f });
    t1.setRotation(30.0f);
    CollSys::Ellipse t2(0.5f, 0.1f);
    t2.setPosition({ -0.5f, 0.0f });
    //t2.setScale({ 0.5f, 0.5f });

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

        if (Mdif) { delete Mdif; }
        Mdif = calculateMdiff(t1, t2);

        win.clear(sf::Color::Black);
        win.draw(*Mdif);
        win.draw(t1);
        win.draw(t2);
        win.draw(c);
        win.display();
    }
    win.close();
}

void glib_string_test() {
    glib::string str1 = "apple",
                 str2 = "peach";

    GTINIT(std::cin);

    TEST(glib, string) {
        EXPECT_STREQ("apple", str1.c_str());
        EXPECT_STREQ("peach", str2.c_str());
    } END;

    TEST(glib, string_ctor) {
        EXPECT_STREQ("apple", glib::string(str1).c_str());
        EXPECT_THROW(glib::string((const char*)nullptr).c_str(), std::invalid_argument);
        EXPECT_STREQ("book", glib::string("book").c_str());
    } END;

    TEST(glib, string_subscript) {
        EXPECT_NO_THROW(str1[0]);
        EXPECT_EQ('p', str1[1]);
        EXPECT_THROW(str1[5], std::out_of_range);
        EXPECT_THROW(str1[6], std::out_of_range);
    } END;

    TEST(glib, string_assign) {
        glib::string str3;
        str3 = str2;
        EXPECT_STREQ("peach", str3.c_str());
        str3 = "door";
        EXPECT_STREQ("door", str3.c_str());
    } END;

    TEST(glib, string_add) {
        glib::string str3;

        str3 = str1 + str2;
        EXPECT_STREQ("applepeach", str3.c_str());

        str3 = str1 + " pie";
        EXPECT_STREQ("apple pie", str3.c_str());
        str3 = "green " + str1;
        EXPECT_STREQ("green apple", str3.c_str());

        str3 = str1 + 's';
        EXPECT_STREQ("apples", str3.c_str());
        str3 = 'a' + str1;
        EXPECT_STREQ("aapple", str3.c_str());
    } END;

    TEST(glib, string_pluseq) {
        glib::string str3 = str1;

        str3 += str2;
        EXPECT_STREQ("applepeach", str3.c_str());
        str3 += " what";
        EXPECT_STREQ("applepeach what", str3.c_str());
        str3 += '?';
        EXPECT_STREQ("applepeach what?", str3.c_str());
    } END;

    TEST(glib, string_eq) {
        EXPECT_TRUE(str1 == "apple");
        EXPECT_TRUE(str1 != str2);
        EXPECT_FALSE("peac" == str2);
    } END;

    std::cout << str1 << ' ' << str2 << std::endl;

    glib::string str3, str4;
    std::cin >> str3 >> str4;
    std::cout << str3 << ", " << str4 << std::endl;

    glib::getline(std::cin, str3);
    std::cout << str3 << std::endl;

    GTEND(std::cerr);
}

void glib_list_test() {
    glib::list<int> x;

    glib::list<int>::forwardIt it = x.begin();
}

int main() {
    return 0;
}
