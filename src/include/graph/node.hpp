#ifndef GRAPH_NODE_HPP
#define GRAPH_NODE_HPP

#include <SDL3/SDL_rect.h>
#include <vector>

namespace GV {

/// A class representing nodes in visualized graphs.
class Node {
public:
    typedef std::vector<Node*>::const_iterator FanoutIterator;

    Node(size_t id, float x, float y) : _id(id), _position({x, y}) {}
    Node() : Node(0, 0, 0) {} // TODO: ID Manager to generate an unused id, or ban this default constructor.

    float& x() { return _position.x; }
    float& y() { return _position.y; }

    float x() const { return _position.x; }
    float y() const { return _position.y; }

    size_t id() const { return _id; }

    // TODO: smart pointers
    void appendFanout(Node& node);

    FanoutIterator beginFanout() const { return _fanouts.cbegin(); }
    FanoutIterator endFanout() const { return _fanouts.cend(); }

private:
    SDL_FPoint _position;
    size_t _id;
    std::vector<Node*> _fanouts;
};

} // namespace GV

#endif