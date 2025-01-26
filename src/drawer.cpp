#include <drawer.hpp>
#include <graph/node.hpp>

namespace GV {

void Drawer::drawNode(const Node& node) const {
    int WINDOW_WIDTH = 640;
    int WINDOW_HEIGHT = 480;    
    float size = 400.0f;

    SDL_Vertex vertices[3];
    vertices[0].position.x = ((float) WINDOW_WIDTH) / 2.0f;
    vertices[0].position.y = (((float) WINDOW_HEIGHT) - size) / 2.0f;
    vertices[0].color.r = 1.0f;
    vertices[0].color.a = 1.0f;
    vertices[1].position.x = (((float) WINDOW_WIDTH) + size) / 2.0f;
    vertices[1].position.y = (((float) WINDOW_HEIGHT) + size) / 2.0f;
    vertices[1].color.g = 1.0f;
    vertices[1].color.a = 1.0f;
    vertices[2].position.x = (((float) WINDOW_WIDTH) - size) / 2.0f;
    vertices[2].position.y = (((float) WINDOW_HEIGHT) + size) / 2.0f;
    vertices[2].color.b = 1.0f;
    vertices[2].color.a = 1.0f;

    SDL_RenderGeometry(_renderer, nullptr, vertices, 3, nullptr, 0);
}

}