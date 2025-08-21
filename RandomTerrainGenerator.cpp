#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

//
// === Isometric projection helper ===
// Converts 3D grid coords (x, y, z) into 2D isometric screen coords
//
sf::Vector2f project(int x, int y, int z, float scale = 20.0f, float zoom = 1.0f) {
    float isoX = (x - y) * scale * zoom;
    float isoY = (x + y) * scale / 2 * zoom - z * zoom;
    return sf::Vector2f(isoX, isoY);
}

//
// === Heightmap generator ===
// Creates a random 2D grid of heights (z-values)
//
std::vector<std::vector<int>> generateHeightmap(int w, int h, int maxZ) {
    std::vector<std::vector<int>> hmap(h, std::vector<int>(w));
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            hmap[y][x] = std::rand() % maxZ;
        }
    }
    return hmap;
}

//
// === Save seed to file ===
// Stores the random seed used in seed.txt
//
void saveSeedToFile(unsigned int seed) {
    std::ofstream file("seed.txt");
    if (file.is_open()) {
        file << seed;
        file.close();
        std::cout << "Seed saved to seed.txt\n";
    }
    else {
        std::cerr << "Failed to save seed!\n";
    }
}

//
// === MAIN ===
//
int main() {
    // --- Window / grid settings ---
    const int winW = 800;
    const int winH = 600;
    const int gridW = 30;
    const int gridH = 30;
    const int maxHeight = 30;
    const float cameraSpeed = 5.0f;
    const float zoomSpeed = 0.1f;

    sf::RenderWindow window(sf::VideoMode(winW, winH), "Random Terrain Generator");
    window.setFramerateLimit(60);

    // --- Load font ---
    sf::Font font;
    if (!font.loadFromFile("fonts/font.ttf")) {
        std::cerr << "Failed to load font!\n";
        return -1;
    }

    //
    // === START MENU SETUP ===
    //
    unsigned int startSeed = static_cast<unsigned int>(std::time(nullptr));
    std::srand(startSeed);

    sf::Text titleText("Random Terrain Generator", font, 30);
    titleText.setPosition(winW / 2 - 150, 100);
    titleText.setFillColor(sf::Color::White);

    sf::Text seedDisplay("Seed: " + std::to_string(startSeed), font, 20);
    seedDisplay.setPosition(winW / 2 - 100, 200);
    seedDisplay.setFillColor(sf::Color::White);

    sf::RectangleShape startButton(sf::Vector2f(150, 40));
    startButton.setPosition(winW / 2 - 75, 300);
    startButton.setFillColor(sf::Color(100, 100, 200));

    sf::Text startButtonText("Start", font, 20);
    startButtonText.setPosition(startButton.getPosition().x + 45, startButton.getPosition().y + 5);
    startButtonText.setFillColor(sf::Color::White);

    sf::RectangleShape enterSeedButton(sf::Vector2f(150, 40));
    enterSeedButton.setPosition(winW / 2 - 75, 360);
    enterSeedButton.setFillColor(sf::Color(100, 100, 200));

    sf::Text enterSeedButtonText("Enter Seed", font, 20);
    enterSeedButtonText.setPosition(enterSeedButton.getPosition().x + 25, enterSeedButton.getPosition().y + 5);
    enterSeedButtonText.setFillColor(sf::Color::White);

    sf::RectangleShape exitStartButton(sf::Vector2f(150, 40));
    exitStartButton.setPosition(winW / 2 - 75, 420);
    exitStartButton.setFillColor(sf::Color(200, 50, 50));

    sf::Text exitStartButtonText("Exit", font, 20);
    exitStartButtonText.setPosition(exitStartButton.getPosition().x + 55, exitStartButton.getPosition().y + 5);
    exitStartButtonText.setFillColor(sf::Color::White);

    //
    // === GAME VARIABLES ===
    //
    bool inMain = false;
    unsigned int seed = startSeed;
    std::vector<std::vector<int>> height = generateHeightmap(gridW, gridH, maxHeight);
    sf::Vector2f cameraOffset(winW / 2.0f, 150.0f);
    float zoom = 1.0f;

    //
    // === MAIN UI BUTTONS ===
    //
    sf::RectangleShape regenButton(sf::Vector2f(100, 30));
    regenButton.setPosition(10, 10);
    regenButton.setFillColor(sf::Color(100, 100, 200));

    sf::Text regenText("Regenerate", font, 14);
    regenText.setPosition(15, 12);
    regenText.setFillColor(sf::Color::White);

    sf::RectangleShape loadSeedButton(sf::Vector2f(100, 30));
    loadSeedButton.setPosition(10, 50);
    loadSeedButton.setFillColor(sf::Color(100, 100, 200));

    sf::Text loadSeedText("Load Seed", font, 14);
    loadSeedText.setPosition(15, 52);
    loadSeedText.setFillColor(sf::Color::White);

    sf::RectangleShape downloadSeedButton(sf::Vector2f(120, 30));
    downloadSeedButton.setPosition(winW - 130, 10);
    downloadSeedButton.setFillColor(sf::Color(100, 100, 200));

    sf::Text downloadSeedText("Download Seed", font, 14);
    downloadSeedText.setPosition(downloadSeedButton.getPosition().x + 5, downloadSeedButton.getPosition().y + 6);
    downloadSeedText.setFillColor(sf::Color::White);

    sf::RectangleShape exitButton(sf::Vector2f(100, 30));
    exitButton.setPosition(10, 90);
    exitButton.setFillColor(sf::Color(200, 50, 50));

    sf::Text exitText("Exit", font, 14);
    exitText.setPosition(35, 92);
    exitText.setFillColor(sf::Color::White);

    // Info text (FPS + seed)
    sf::Text infoText("", font, 14);
    infoText.setFillColor(sf::Color::White);

    // Credits (bottom right)
    sf::Text madeByText("Made by Avazbek", font, 14);
    sf::FloatRect madeByBounds = madeByText.getLocalBounds();
    madeByText.setPosition(winW - madeByBounds.width - 10, winH - madeByBounds.height - 10);
    madeByText.setFillColor(sf::Color(180, 180, 180));

    // Camera drag vars
    bool dragging = false;
    sf::Vector2i lastMousePos;

    // FPS tracking
    sf::Clock fpsClock;
    int frameCount = 0;
    float fps = 0.0f;

    //
    // === GAME LOOP ===
    //
    while (window.isOpen()) {
        //
        // === EVENT HANDLING ===
        //
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!inMain) {
                // --- Start menu events ---
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (startButton.getGlobalBounds().contains(mousePos)) {
                        inMain = true;
                        seed = startSeed;
                        std::srand(seed);
                        height = generateHeightmap(gridW, gridH, maxHeight);
                    }
                    else if (enterSeedButton.getGlobalBounds().contains(mousePos)) {
                        std::cout << "Enter seed: ";
                        unsigned int inputSeed;
                        std::cin >> inputSeed;
                        startSeed = inputSeed;
                        seedDisplay.setString("Seed: " + std::to_string(startSeed));
                    }
                    else if (exitStartButton.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                }
            }
            else {
                // --- Main game events ---
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (regenButton.getGlobalBounds().contains(mousePos)) {
                            seed = static_cast<unsigned int>(std::time(nullptr));
                            std::srand(seed);
                            height = generateHeightmap(gridW, gridH, maxHeight);
                        }
                        else if (loadSeedButton.getGlobalBounds().contains(mousePos)) {
                            std::cout << "Enter seed: ";
                            unsigned int inputSeed;
                            std::cin >> inputSeed;
                            seed = inputSeed;
                            std::srand(seed);
                            height = generateHeightmap(gridW, gridH, maxHeight);
                        }
                        else if (downloadSeedButton.getGlobalBounds().contains(mousePos)) {
                            saveSeedToFile(seed);
                        }
                        else if (exitButton.getGlobalBounds().contains(mousePos)) {
                            window.close();
                        }
                        else {
                            // Start camera dragging
                            dragging = true;
                            lastMousePos = sf::Mouse::getPosition(window);
                        }
                    }
                }

                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        dragging = false;
                }

                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.delta > 0)
                        zoom += zoomSpeed;
                    else
                        zoom = std::max(0.1f, zoom - zoomSpeed);
                }
            }
        }

        //
        // === MAIN GAME UPDATE ===
        //
        if (inMain) {
            // Camera dragging
            if (dragging) {
                sf::Vector2i currentPos = sf::Mouse::getPosition(window);
                sf::Vector2i delta = currentPos - lastMousePos;
                cameraOffset += sf::Vector2f(delta.x, delta.y);
                lastMousePos = currentPos;
            }

            // Camera movement via keys
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cameraOffset.y += cameraSpeed;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cameraOffset.y -= cameraSpeed;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cameraOffset.x += cameraSpeed;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cameraOffset.x -= cameraSpeed;

            // FPS counter
            frameCount++;
            if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
                fps = frameCount / fpsClock.getElapsedTime().asSeconds();
                frameCount = 0;
                fpsClock.restart();
            }

            // Update info text
            infoText.setString("FPS: " + std::to_string(static_cast<int>(fps)) + " | Seed: " + std::to_string(seed));
            sf::FloatRect textBounds = infoText.getLocalBounds();
            infoText.setPosition(winW - textBounds.width - 10, 50);

            //
            // === RENDERING ===
            //
            window.clear(sf::Color::Black);

            // Draw UI
            window.draw(regenButton);
            window.draw(regenText);
            window.draw(loadSeedButton);
            window.draw(loadSeedText);
            window.draw(downloadSeedButton);
            window.draw(downloadSeedText);
            window.draw(exitButton);
            window.draw(exitText);
            window.draw(infoText);
            window.draw(madeByText);

            // Lighting setup (directional light from top-left-front)
            sf::Vector3f lightDir(-1, -1, 1);
            float len = std::sqrt(lightDir.x * lightDir.x + lightDir.y * lightDir.y + lightDir.z * lightDir.z);
            lightDir /= len;

            // Draw terrain grid with shading
            for (int y = 0; y < gridH - 1; ++y) {
                for (int x = 0; x < gridW - 1; ++x) {
                    int z = height[y][x];
                    int zX = height[y][x + 1];
                    int zY = height[y + 1][x];

                    // Calculate surface normal
                    sf::Vector3f v1(1, 0, float(zX - z));
                    sf::Vector3f v2(0, 1, float(zY - z));
                    sf::Vector3f normal(
                        v1.y * v2.z - v1.z * v2.y,
                        v1.z * v2.x - v1.x * v2.z,
                        v1.x * v2.y - v1.y * v2.x
                    );

                    float nLen = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
                    if (nLen != 0) {
                        normal.x /= nLen;
                        normal.y /= nLen;
                        normal.z /= nLen;
                    }

                    // Lighting intensity
                    float intensity = normal.x * lightDir.x + normal.y * lightDir.y + normal.z * lightDir.z;
                    if (intensity < 0) intensity = 0;

                    // Terrain line color (greenish tint)
                    sf::Color lineColor(
                        static_cast<sf::Uint8>(0 * intensity),
                        static_cast<sf::Uint8>(200 * intensity + 55),
                        static_cast<sf::Uint8>(0 * intensity)
                    );

                    // Projected points
                    sf::Vector2f p1 = project(x, y, z, 20.0f, zoom) + cameraOffset;
                    sf::Vector2f p2 = project(x + 1, y, zX, 20.0f, zoom) + cameraOffset;
                    sf::Vector2f p3 = project(x, y + 1, zY, 20.0f, zoom) + cameraOffset;

                    // Draw edges
                    sf::Vertex line1[] = { sf::Vertex(p1, lineColor), sf::Vertex(p2, lineColor) };
                    sf::Vertex line2[] = { sf::Vertex(p1, lineColor), sf::Vertex(p3, lineColor) };

                    window.draw(line1, 2, sf::Lines);
                    window.draw(line2, 2, sf::Lines);
                }
            }
        }
        else {
            //
            // === START MENU RENDERING ===
            //
            window.clear(sf::Color(20, 20, 20));
            window.draw(titleText);
            window.draw(seedDisplay);
            window.draw(startButton);
            window.draw(startButtonText);
            window.draw(enterSeedButton);
            window.draw(enterSeedButtonText);
            window.draw(exitStartButton);
            window.draw(exitStartButtonText);
        }

        window.display();
    }

    return 0;
}
