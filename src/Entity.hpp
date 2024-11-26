/*
 *
*/
#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <cstddef>

#define GOOMBA  (size_t) 0
#define KOOPA   (size_t) 1
#define BOMBOMB (size_t) 2

class entity {
    public:
        entity(void)
        {
        }
        ~entity() = default;

        operator int() const
        {
            return this->_id;
        }
    private:
        size_t _id = (size_t) -1;
};

#endif
