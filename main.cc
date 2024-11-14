#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "util.h"
using namespace sf;
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;

struct vec3d {
    float x, y , z;
};

struct triangle {
    vec3d p[3];
};

struct mesh {
    vector<triangle> tris;
};

struct mat4x4 {
    float m[4][4] = { 0 };
};

void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m) {
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];

    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
    if (w != 0.0f) {
        o.x /= w; o.y /= w; o.z /= w;
    }
}

int main() {
    mesh meshCube;
    meshCube.tris = {
        // SOUTH
        { 0.0f, 0.0f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f},
        { 0.0f, 0.0f, 0.0f,     1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f},

        // EAST
        { 1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f},
        { 1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f},
        
        //NORTH
        { 1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f},
        { 1.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f},

        //WEST
        { 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f},
        { 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f},

        //TOP
        { 0.0f, 1.0f, 0.0f,     0.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f},
        { 0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f},
        
        //BOTTOM
        { 1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f},
        { 1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f},
    };

    float fAspectRatio = (float)HEIGHT / (float)WIDTH;
    float fFov = 90.f;
    float fFovRad = 1.0f / tanf(fFov*0.5f/180.f*3.14159f);
    float fNear = .1f;
    float fFar = 1000.f;

    mat4x4 projMatrix;
    projMatrix.m[0][0] = fAspectRatio * fFovRad;
    projMatrix.m[1][1] = fFovRad;
    projMatrix.m[2][2] = fFar / (fFar - fNear);
    projMatrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    projMatrix.m[2][3] = 1.0f;

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Game");
    Clock clock;

    mat4x4 matRotZ, matRotX;
    float fTheta = 0.0f;

    while (window.isOpen()) {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed || ev.key.code == Keyboard::Key::Escape) {
                window.close();
            }
        }

        window.clear();
        matRotZ.m[0][0] = cosf(fTheta);
        matRotZ.m[0][1] = sinf(fTheta);
        matRotZ.m[1][0] = -sinf(fTheta);
        matRotZ.m[1][1] = cosf(fTheta);
        matRotZ.m[2][2] = 1;
        matRotZ.m[3][3] = 1;

        matRotX.m[0][0] = 1;
        matRotX.m[1][1] = cosf(fTheta*0.5f);
        matRotX.m[1][2] = sinf(fTheta*0.5f);
        matRotX.m[2][1] = -sinf(fTheta*0.5f);
        matRotX.m[2][2] = cosf(fTheta*0.5f);
        matRotX.m[3][3] = 1;
        for(auto tri : meshCube.tris) {
            triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;           
            MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
            MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
            MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

            MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
            MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
            MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

            triTranslated = triRotatedZX;
        
            triTranslated.p[0].z = tri.p[0].z + 3.f;
            triTranslated.p[1].z = tri.p[1].z + 3.f;
            triTranslated.p[2].z = tri.p[2].z + 3.f;

            MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], projMatrix);
            MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], projMatrix);
            MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], projMatrix);

            triProjected.p[0].x += 1.f; triProjected.p[0].y += 1.f;
            triProjected.p[1].x += 1.f; triProjected.p[1].y += 1.f;
            triProjected.p[2].x += 1.f; triProjected.p[2].y += 1.f;

            triProjected.p[0].x *= 0.5f * WIDTH;
            triProjected.p[1].x *= 0.5f * WIDTH;
            triProjected.p[2].x *= 0.5f * WIDTH;

            triProjected.p[0].y *= 0.5f * HEIGHT;
            triProjected.p[1].y *= 0.5f * HEIGHT;
            triProjected.p[2].y *= 0.5f * HEIGHT;

            DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                    triProjected.p[1].x, triProjected.p[1].y,
                    triProjected.p[2].x, triProjected.p[2].y,
                    window);
        }
        fTheta += .01f;
        window.display();
    }
    return 0;
}
