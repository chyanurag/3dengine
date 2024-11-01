#include <SFML/Graphics.hpp>
#include <math.h>

typedef struct {
    sf::Vector3f p[3];
} Triangle;

typedef struct {
    Triangle tris[12];
} Cube;

typedef struct {
    float m[4][4] = { 0 };
} mat4x4;

mat4x4 matProj;
mat4x4 matRotX;
float fNear = .1f;
float fFar = 1000.0f;
float fFov = 90.0f;
float fAspectRatio = 1080.f/1920.f;
float fFovRad = 1.0f / tanf(fFov * .5f / 180.f * 3.14159f);
float fTheta = 0.0f;

void MultiplyMatrixVector(sf::Vector3f& i, sf::Vector3f& o, mat4x4& m) {
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];

    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
    if(w != 0.f) {
        o.x /= w; o.y /= w; o.z /= w;
    }
}

void DrawLine(float x1, float y1, float x2, float y2, sf::RenderWindow& window) {
    sf::VertexArray line(sf::Lines, 2);
    line[0].position.x = x1;
    line[0].position.y = y1;

    line[1].position.x = x2;
    line[1].position.y = y2;

    window.draw(line);
}

void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, sf::RenderWindow& window) {
    DrawLine(x1, y1, x2, y2, window);
    DrawLine(x2, y2, x3, y3, window);
    DrawLine(x3, y3, x1, y1, window);
}

void DrawCube(Cube& cube, sf::RenderWindow& window) {
    for(int i = 0; i < 12; i++) {
        Triangle tri = cube.tris[i];
        Triangle triProjected, triRotated, triTranslated;
    
        MultiplyMatrixVector(tri.p[0], triRotated.p[0], matRotX);
        MultiplyMatrixVector(tri.p[1], triRotated.p[1], matRotX);
        MultiplyMatrixVector(tri.p[2], triRotated.p[2], matRotX);
    
        triTranslated = triRotated;
        triTranslated.p[0].z = triRotated.p[0].z + 3.0f;
        triTranslated.p[1].z = triRotated.p[1].z + 3.0f;
        triTranslated.p[2].z = triRotated.p[2].z + 3.0f;

        MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
        MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
        MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

        triProjected.p[0].x += 1.f; triProjected.p[0].y += 1.f;
        triProjected.p[1].x += 1.f; triProjected.p[1].y += 1.f;
        triProjected.p[2].x += 1.f; triProjected.p[2].y += 1.f;

        triProjected.p[0].x *= 0.4 * 1920.f;
        triProjected.p[0].y *= 0.4 * 1080.f;
        triProjected.p[1].x *= 0.4 * 1920.f;
        triProjected.p[1].y *= 0.4 * 1080.f;
        triProjected.p[2].x *= 0.4 * 1920.f;
        triProjected.p[2].y *= 0.4 * 1080.f;

        DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                triProjected.p[1].x, triProjected.p[1].y,
                triProjected.p[2].x, triProjected.p[2].y, window);
    }
}

float DegreesToRadians(float angle) {
    return angle * (M_PI / 180.f);
}

int main() {

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "3d Engine", sf::Style::Fullscreen);

    Cube cube = {
        .tris = {
            { sf::Vector3f(0.f, 0.f, 0.f), sf::Vector3f(0.f, 1.f, 0.f), sf::Vector3f(1.f, 1.f, 0.f) },
            { sf::Vector3f(0.f, 0.f, 0.f), sf::Vector3f(1.f, 1.f, 0.f), sf::Vector3f(1.f, 0.f, 0.f) },

            { sf::Vector3f(1.f, 0.f, 0.f), sf::Vector3f(1.f, 1.f, 0.f), sf::Vector3f(1.f, 1.f, 1.f) },
            { sf::Vector3f(1.f, 0.f, 0.f), sf::Vector3f(1.f, 0.f, 0.f), sf::Vector3f(1.f, 0.f, 1.f) },

            { sf::Vector3f(1.f, 0.f, 1.f), sf::Vector3f(1.f, 1.f, 1.f), sf::Vector3f(0.f, 1.f, 1.f) },
            { sf::Vector3f(1.f, 0.f, 1.f), sf::Vector3f(0.f, 1.f, 1.f), sf::Vector3f(0.f, 0.f, 1.f) },

            { sf::Vector3f(0.f, 0.f, 1.f), sf::Vector3f(0.f, 1.f, 1.f), sf::Vector3f(0.f, 1.f, 0.f) },
            { sf::Vector3f(0.f, 0.f, 1.f), sf::Vector3f(1.f, 1.f, 1.f), sf::Vector3f(0.f, 0.f, 0.f) },

            { sf::Vector3f(0.f, 1.f, 0.f), sf::Vector3f(0.f, 1.f, 1.f), sf::Vector3f(1.f, 1.f, 1.f) },
            { sf::Vector3f(0.f, 1.f, 0.f), sf::Vector3f(1.f, 1.f, 1.f), sf::Vector3f(1.f, 1.f, 0.f) },

            { sf::Vector3f(1.f, 0.f, 1.f), sf::Vector3f(0.f, 0.f, 1.f), sf::Vector3f(0.f, 0.f, 0.f) },
            { sf::Vector3f(1.f, 0.f, 1.f), sf::Vector3f(0.f, 0.f, 0.f), sf::Vector3f(1.f, 0.f, 0.f) },
        }
    };

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.f;


    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        fTheta += .01f;
        matRotX.m[0][0] = 1.f;
        matRotX.m[1][1] = cosf(fTheta*0.5f);
        matRotX.m[1][2] = sinf(fTheta*0.5f);
        matRotX.m[2][1] = -sinf(fTheta*0.5f);
        matRotX.m[2][2] = cosf(fTheta*0.5f);
        matRotX.m[3][3] = 1.f;
        DrawCube(cube, window);
        window.display();
    }

    return 0;
}
