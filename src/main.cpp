// Copyright (c) Tyler Veness

#include <stdint.h>

#include <cmath>
#include <random>
#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

struct Dot {
  sf::CircleShape shape;
  int x_vel = 1;
  int y_vel = 1;

  Dot(uint32_t x, uint32_t y) {
    shape.setOutlineThickness(10);
    shape.setOutlineColor(sf::Color{100, 250, 50});
    shape.setFillColor(sf::Color{150, 50, 250});
    shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
    shape.setRadius(10);
  }
};

int main() {
  sf::Clock clock;

  bool last_left_pressed = false;
  int level = 0;

  sf::RectangleShape line;
  line.setFillColor(sf::Color::Black);
  line.setSize(sf::Vector2f{1, 2});

  sf::RectangleShape block1;
  block1.setFillColor(sf::Color::Black);
  block1.setSize(sf::Vector2f{100, 50});
  block1.setPosition(sf::Vector2f{0, 300});

  sf::RectangleShape block2;
  block2.setFillColor(sf::Color::Black);
  block2.setSize(sf::Vector2f{100, 50});
  block2.setPosition(sf::Vector2f{700, 300});

  // Add starting dots
  std::vector<Dot> dots;
  {
    std::random_device rd;
    std::mt19937 generator{rd()};
    std::uniform_int_distribution<int> xDist{1, 800};
    std::uniform_int_distribution<int> yDist{1, 600};

    for (size_t i = 0; i < 2; ++i) {
      dots.emplace_back(xDist(generator), yDist(generator));
    }
  }
  Dot* clicked_dot = nullptr;

  sf::RenderWindow window{sf::VideoMode{{800, 600}}, "Two Dots"};
  window.setFramerateLimit(60);

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();

    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear(sf::Color::White);
    bool dot_connected = false;

    bool current_left_pressed =
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    bool just_pressed_left = !last_left_pressed && current_left_pressed;
    bool just_released_left = last_left_pressed && !current_left_pressed;

    sf::Vector2i mouse_position_int = sf::Mouse::getPosition(window);
    sf::Vector2f mouse_position(mouse_position_int.x, mouse_position_int.y);

    for (auto& dot : dots) {
      // Bounce off of left side of window
      if (dot.shape.getPosition().x <= 0) {
        dot.x_vel *= -1;
      }

      // Bounce off of right side of window
      if (dot.shape.getPosition().x >= 800) {
        dot.x_vel *= -1;
      }

      // Bounce off of bottom side of window
      if (dot.shape.getPosition().y <= 0) {
        dot.y_vel *= -1;
      }

      // Bounce off of top side of window
      if (dot.shape.getPosition().y >= 600) {
        dot.y_vel *= -1;
      }

      auto distance_from_dot =
          mouse_position - dot.shape.getPosition() + sf::Vector2f{-10, -10};

      if (std::hypot(distance_from_dot.x, distance_from_dot.y) <
          dot.shape.getRadius() + dot.shape.getOutlineThickness()) {
        if (just_pressed_left) {
          clicked_dot = &dot;
        } else if (just_released_left && clicked_dot != nullptr &&
                   clicked_dot != &dot) {
          dot_connected = true;
          clicked_dot = nullptr;
        }
      }

      dot.shape.move({dot.x_vel * dt, dot.y_vel * dt});
      window.draw(dot.shape);
    }

    if (just_released_left && dot_connected) {
      ++level;

      for (auto& dot : dots) {
        dot.x_vel += std::copysign(50, dot.x_vel);
        dot.y_vel += std::copysign(50, dot.y_vel);
      }
    }

    if (level > 2) {
      window.draw(block1);
      window.draw(block2);
    }

    if (level > 10) {
      block1.setSize(sf::Vector2f{150, 50});
      block2.setSize(sf::Vector2f{150, 50});
      block2.setPosition(sf::Vector2f{650, 300});
    }

    // If a shape was clicked and held, draw a line from it to the mouse
    if (clicked_dot != nullptr) {
      auto distance_from_dot =
          mouse_position - clicked_dot->shape.getPosition();

      line.setPosition(clicked_dot->shape.getPosition());
      line.setSize(
          sf::Vector2f{std::hypot(distance_from_dot.x, distance_from_dot.y),
                       line.getSize().y});
      line.setRotation(
          sf::radians(std::atan2(distance_from_dot.y, distance_from_dot.x)));

      window.draw(line);
    }

    if (!current_left_pressed) {
      clicked_dot = nullptr;
    }

    last_left_pressed = current_left_pressed;

    window.display();
  }
}
