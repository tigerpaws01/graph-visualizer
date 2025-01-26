#include <drawer.hpp>
#include <graph/node.hpp>

namespace GV {

void Drawer::drawNode(const Node& node) const {
    int WINDOW_WIDTH = 640;
    int WINDOW_HEIGHT = 480;    
    float size = 40.0f;

    SDL_Vertex vertices[4];
    vertices[0].position.x = node.x();
    vertices[0].position.y = node.y();
    vertices[0].color.r = 1.0f;
    vertices[0].color.g = 1.0f;
    vertices[0].color.b = 1.0f;
    vertices[0].color.a = 1.0f;

    vertices[1].position.x = node.x();
    vertices[1].position.y = node.y() - size;
    vertices[1].color.r = 0;
    vertices[1].color.g = 0;
    vertices[1].color.b = 1.0f;
    vertices[1].color.a = 1.0f;

    vertices[2].position.x = node.x() - size;
    vertices[2].position.y = node.y() + (size / 2.0f);
    vertices[2].color.r = 0;
    vertices[2].color.g = 1.0f;
    vertices[2].color.b = 0;
    vertices[2].color.a = 1.0f;

    vertices[3].position.x = node.x() + size;
    vertices[3].position.y = node.y() + (size / 2.0f);
    vertices[3].color.r = 1.0f;
    vertices[3].color.g = 0;
    vertices[3].color.b = 0;
    vertices[3].color.a = 1.0f;

    const int indices[9] = {
        2, 1, 0,
        0, 1, 3,
        2, 0, 3,
    };

    SDL_RenderGeometry(_renderer, nullptr, vertices, 4, indices, 9);
}

}