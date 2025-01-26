#include <drawer.hpp>
#include <graph/node.hpp>
#include <cmath>

#define NUM_SPHERE_NODES 10
#define PI 3.1415926f

namespace GV {

void Drawer::drawNode(const Node& node) const {
    int WINDOW_WIDTH = 640;
    int WINDOW_HEIGHT = 480;    
    float size = 25.0f;

    SDL_Vertex vertices[NUM_SPHERE_NODES + 1];
    vertices[0].position.x = node.x();
    vertices[0].position.y = node.y();
    vertices[0].color.r = 1.0f;
    vertices[0].color.g = 1.0f;
    vertices[0].color.b = 1.0f;
    vertices[0].color.a = 1.0f;

    for (int i = 1; i <= NUM_SPHERE_NODES; i++) {
        float radian = 2 * PI / NUM_SPHERE_NODES * i;
        vertices[i].position.x = node.x() + std::cos(radian) * size;
        vertices[i].position.y = node.y() + std::sin(radian) * size;
        vertices[i].color.b = 0;
        vertices[i].color.g = 0;
        vertices[i].color.r = 1.0f;
        vertices[i].color.a = 1.0f;
    }

    /*vertices[1].position.x = node.x();
    vertices[1].position.y = node.y() - size;
    vertices[1].color.b = 0;
    vertices[1].color.g = 0;
    vertices[1].color.r = 1.0f;
    vertices[1].color.a = 1.0f;

    vertices[2].position.x = node.x() - size;
    vertices[2].position.y = node.y() + (size / 2.0f);
    vertices[2].color.b = 0;
    vertices[2].color.r = 1.0f;
    vertices[2].color.g = 0;
    vertices[2].color.a = 1.0f;

    vertices[3].position.x = node.x() + size;
    vertices[3].position.y = node.y() + (size / 2.0f);
    vertices[3].color.r = 1.0f;
    vertices[3].color.g = 0;
    vertices[3].color.b = 0;
    vertices[3].color.a = 1.0f;*/

    int indices[NUM_SPHERE_NODES * 3] = {0};
    for (int i = 0; i < NUM_SPHERE_NODES; i++) {
        indices[i * 3 + 0] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = (i + 2 > NUM_SPHERE_NODES) ? 1 : i + 2;
    }

    SDL_RenderGeometry(_renderer, nullptr, vertices, NUM_SPHERE_NODES + 1, indices, NUM_SPHERE_NODES * 3);

    /*SDL_SetRenderDrawColor(_renderer, 255.0, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = 0; i <= NUM_SPHERE_NODES; i++) {
        if (i == 0) SDL_SetRenderDrawColor(_renderer, 255.0, 255.0, 255.0, SDL_ALPHA_OPAQUE);
        if (i == 1) SDL_SetRenderDrawColor(_renderer, 255.0, 0, 0, SDL_ALPHA_OPAQUE);
        if (i == 2) SDL_SetRenderDrawColor(_renderer, 0, 255.0, 0, SDL_ALPHA_OPAQUE);
        if (i == 3) SDL_SetRenderDrawColor(_renderer, 0, 0, 255.0, SDL_ALPHA_OPAQUE);

        SDL_RenderPoint(_renderer, vertices[i].position.x, vertices[i].position.y);
    }*/
}

}