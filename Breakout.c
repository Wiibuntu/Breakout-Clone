#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BLOCK_ROWS = 5;
const int BLOCK_COLS = 10;
const float BALL_RADIUS = 10.0f;
const float PADDLE_WIDTH = 100.0f;
const float PADDLE_HEIGHT = 20.0f;
const float BLOCK_WIDTH = 75.0f;
const float BLOCK_HEIGHT = 30.0f;

sf::Color randomColor() {
    return sf::Color(rand() % 256, rand() % 256, rand() % 256);
}

class Block : public sf::RectangleShape {
public:
    bool destroyed = false;
    Block(float x, float y, sf::Color color) {
        setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
        setPosition(x, y);
        setFillColor(color);
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Breakout Clone");
    window.setFramerateLimit(60);

    sf::CircleShape ball(BALL_RADIUS);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    sf::RectangleShape paddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    paddle.setFillColor(sf::Color::White);
    paddle.setPosition(WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2, WINDOW_HEIGHT - 50);

    std::vector<Block> blocks;
    for (int i = 0; i < BLOCK_ROWS; ++i) {
        for (int j = 0; j < BLOCK_COLS; ++j) {
            blocks.emplace_back(j * (BLOCK_WIDTH + 5), i * (BLOCK_HEIGHT + 5), randomColor());
        }
    }

    float ballVelocityX = 4.0f;
    float ballVelocityY = -4.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Paddle Movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 0) {
            paddle.move(-8.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x + PADDLE_WIDTH < WINDOW_WIDTH) {
            paddle.move(8.0f, 0);
        }

        // Ball Movement
        ball.move(ballVelocityX, ballVelocityY);

        if (ball.getPosition().x < 0 || ball.getPosition().x + BALL_RADIUS * 2 > WINDOW_WIDTH)
            ballVelocityX = -ballVelocityX;
        if (ball.getPosition().y < 0)
            ballVelocityY = -ballVelocityY;

        // Ball and Paddle Collision
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            ballVelocityY = -ballVelocityY;
        }

        // Ball and Block Collision
        for (auto &block : blocks) {
            if (!block.destroyed && ball.getGlobalBounds().intersects(block.getGlobalBounds())) {
                block.destroyed = true;
                ballVelocityY = -ballVelocityY;
            }
        }

        // Check if all blocks are destroyed
        if (std::all_of(blocks.begin(), blocks.end(), [](Block &block) { return block.destroyed; })) {
            blocks.clear();
            for (int i = 0; i < BLOCK_ROWS; ++i) {
                for (int j = 0; j < BLOCK_COLS; ++j) {
                    blocks.emplace_back(j * (BLOCK_WIDTH + 5), i * (BLOCK_HEIGHT + 5), randomColor());
                }
            }
        }

        // Lose Condition
        if (ball.getPosition().y > WINDOW_HEIGHT) {
            ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
            ballVelocityY = -4.0f;
        }

        // Rendering
        window.clear();
        window.draw(ball);
        window.draw(paddle);
        for (auto &block : blocks) {
            if (!block.destroyed)
                window.draw(block);
        }
        window.display();
    }

    return 0;
}
