#include <camera.hpp>

namespace GV {

SDL_FPoint
Camera::toScreenSpace(const Node& node) const {
    return SDL_FPoint {
        (node.x() - center().x) * size() + _halfWidth,
        (node.y() - center().y) * size() + _halfHeight
    };
}

} // namespace GV
