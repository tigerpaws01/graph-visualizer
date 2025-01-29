#include <drawer.hpp>
#include <graph/node.hpp>
#include <cmath>

#define NUM_SPHERE_NODES 10
#define PI 3.1415926f

namespace GV {

void Drawer::drawNode(const Node& node, const Camera& camera) const {
    int WINDOW_WIDTH = 640;
    int WINDOW_HEIGHT = 480;    
    float size = 25.0f * camera.size();

    // relative point position
    SDL_FPoint relPosition = camera.toScreenSpace(node);

    SDL_Vertex vertices[NUM_SPHERE_NODES + 1];
    vertices[0].position.x = relPosition.x;
    vertices[0].position.y = relPosition.y;
    vertices[0].color.r = 1.0f;
    vertices[0].color.g = 1.0f;
    vertices[0].color.b = 1.0f;
    vertices[0].color.a = 1.0f;

    for (int i = 1; i <= NUM_SPHERE_NODES; i++) {
        float radian = 2 * PI / NUM_SPHERE_NODES * i;
        vertices[i].position.x = relPosition.x + std::cos(radian) * size;
        vertices[i].position.y = relPosition.y + std::sin(radian) * size;
        vertices[i].color.b = 0;
        vertices[i].color.g = 0;
        vertices[i].color.r = 1.0f;
        vertices[i].color.a = 1.0f;
    }

    int indices[NUM_SPHERE_NODES * 3] = {0};
    for (int i = 0; i < NUM_SPHERE_NODES; i++) {
        indices[i * 3 + 0] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = (i + 2 > NUM_SPHERE_NODES) ? 1 : i + 2;
    }

    SDL_RenderGeometry(_renderer, nullptr, vertices, NUM_SPHERE_NODES + 1, indices, NUM_SPHERE_NODES * 3);
}

} // namespace GV