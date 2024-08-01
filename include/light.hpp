#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "model.hpp"

class Light : public Model
{
    public:

        Light ()
            : Model("models/small_box.tris", glm::vec3(1.0, 1.0, 1.0))
        { }
};

#endif