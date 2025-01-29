#ifndef GRAPH_NODE_HPP
#define GRAPH_NODE_HPP

#include <SDL3/SDL_rect.h>
#include <iostream>

namespace GV {

/// A class representing nodes in visualized graphs.
class Node {
public:
    Node(float x, float y) : _position({x, y}) {}
    Node() : Node(0, 0) {}

    float& x() { return _position.x; }
    float& y() { return _position.y; }

    float x() const { return _position.x; }
    float y() const { return _position.y; }

private:
    SDL_FPoint _position;
};

} // namespace GV

#endif