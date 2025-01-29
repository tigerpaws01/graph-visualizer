#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SDL3/SDL.h>
#include <graph/node.hpp>

namespace GV {

class Camera {
public:
    Camera(unsigned int width, unsigned int height) 
      : _center({0, 0}), _size(1), _width(width), _height(height)
      , _halfWidth(width >> 1), _halfHeight(height >> 1) {}

    SDL_FPoint& center() { return _center; }
    SDL_FPoint center() const { return _center; }
    float& size() { return _size; }
    float size() const { return _size; }

    SDL_FPoint toScreenSpace(const Node& node) const;

private:
    SDL_FPoint _center;
    float _size;
    unsigned int _width;
    unsigned int _height;
    unsigned int _halfWidth;
    unsigned int _halfHeight;
};

} // namespace GV

#endif
