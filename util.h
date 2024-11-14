#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

void DrawLine(int x1, int y1, int x2, int y2, RenderWindow& window) {
    Vertex line[] = {
        Vertex(Vector2f(x1, y1)),
        Vertex(Vector2f(x2, y2))
    };
    window.draw(line, 2, sf::Lines);
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RenderWindow& window) {
    DrawLine(x1, y1, x2, y2, window);
    DrawLine(x2, y2, x3, y3, window);
    DrawLine(x3, y3, x1, y1, window);
}
