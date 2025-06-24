#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>

using namespace sf;

struct SnakePart {
    int x, y;
    SnakePart(int x, int y) : x(x), y(y) {}
};

enum Direction { UP, DOWN, LEFT, RIGHT };

const int SIZE = 20;   // Grid size
const int WIDTH = 30;  // 600 / 20
const int HEIGHT = 20; // 400 / 20

class SnakeGame {
private:
    RenderWindow window;
    std::vector<SnakePart> snake;
    Direction dir;
    int foodX, foodY;
    int score;
    float delay;
    Clock clock;
    Time timer;

    Texture foodTex;
    Sprite foodSprite;
    SoundBuffer eatBuffer;
    Sound eatSound;

public:
    SnakeGame() : window(VideoMode(WIDTH * SIZE, HEIGHT * SIZE), "Snake Game with SFML") {
        snake.push_back(SnakePart(WIDTH / 2, HEIGHT / 2));
        dir = RIGHT;
        score = 0;
        delay = 0.2f;

        srand(time(0));
        placeFood();

        foodTex.loadFromFile("food.png");  // Add food.png in project directory
        foodSprite.setTexture(foodTex);

        eatBuffer.loadFromFile("eat.wav"); // Add eat.wav in project directory
        eatSound.setBuffer(eatBuffer);
    }

    void placeFood() {
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            timer += clock.restart();

            if (timer.asSeconds() >= delay) {
                update();
                timer = Time::Zero;
            }

            render();
        }
    }

    void processEvents() {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) && dir != DOWN) dir = UP;
        if (Keyboard::isKeyPressed(Keyboard::Down) && dir != UP) dir = DOWN;
        if (Keyboard::isKeyPressed(Keyboard::Left) && dir != RIGHT) dir = LEFT;
        if (Keyboard::isKeyPressed(Keyboard::Right) && dir != LEFT) dir = RIGHT;
    }

    void update() {
        SnakePart head = snake.front();
        switch (dir) {
            case UP: head.y--; break;
            case DOWN: head.y++; break;
            case LEFT: head.x--; break;
            case RIGHT: head.x++; break;
        }

        // Check wall collision
        if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
            std::cout << "Game Over! Score: " << score << "\n";
            window.close();
            return;
        }

        // Check self collision
        for (auto part : snake)
            if (head.x == part.x && head.y == part.y) {
                std::cout << "Game Over! Score: " << score << "\n";
                window.close();
                return;
            }

        // Add new head
        snake.insert(snake.begin(), head);

        // Eat food
        if (head.x == foodX && head.y == foodY) {
            score++;
            eatSound.play();
            placeFood();
            if (delay > 0.05f) delay -= 0.01f; // Increase difficulty
        } else {
            snake.pop_back(); // Remove tail
        }
    }

    void render() {
        window.clear(Color::Black);

        // Draw snake
        RectangleShape rect(Vector2f(SIZE - 2, SIZE - 2));
        rect.setFillColor(Color::Green);
        for (auto part : snake) {
            rect.setPosition(part.x * SIZE, part.y * SIZE);
            window.draw(rect);
        }

        // Draw food
        foodSprite.setPosition(foodX * SIZE, foodY * SIZE);
        window.draw(foodSprite);

        window.display();
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}