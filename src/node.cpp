#include <graph/node.hpp>

namespace GV {

void
Node::appendFanout(Node& node) {
    // TODO: directed & undirected graphs
    // TODO: deduplication & no-self-loops
    _fanouts.push_back(&node);
}

} // namespace GV
