#ifndef GRAPH_NODE_HPP
#define GRAPH_NODE_HPP

#include <SDL3/SDL_rect.h>

namespace GV {

/// A class representing nodes in visualized graphs.
class Node {
public:
    Node(int x, int y) : _position({x, y}) {}
    Node() : Node(0, 0) {}

private:
    SDL_Point _position;
};

}

#endif