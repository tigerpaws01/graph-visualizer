#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <vector>
#include <graph/node.hpp>
#include <camera.hpp>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace GV {

/// A class used to draw various components on visualized graphs.
class Drawer {
public:
    // TODO: smart pointers for renderer?
    Drawer() {}
    Drawer(SDL_Renderer* renderer) : _renderer(renderer) {}

    void drawNode(const GV::Node& node, const Camera& camera) const;
    void drawConnections(const std::vector<GV::Node>& nodes, const Camera& camera) const;

private:
    SDL_Renderer* _renderer;
};

} // namespace GV

#endif