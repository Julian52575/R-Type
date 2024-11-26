/*
 *
 *
*/
#include "Entity.hpp"
#include "Sparse_Array.hpp"
#include "Registry.hpp"

#include <exception>
#include <memory>
#include <string>


class component {
    public:
        component() : _x(0), _y(0) //default constructor for vector resizing
        {;}

        component(int y) : _y(y) //, int x) : _y(y), _x(x)
        {;}

        component(int x, int y) : _x(x), _y(y)
        {;}
        ~component() = default;
        friend std::ostream& operator<<(std::ostream& os, const component& c);

        int gety(void) const
        {
            return this->_y;
        }
        int getx(void) const
        {
            return this->_x;
        }
    private:
        int _x = 0;
        int _y;
};

std::ostream& operator<<(std::ostream& os, const component& c)
{
    os << "(" << c._x << "," << c._y << ")" << std::endl;
    return os;
}


#ifdef DEBUG
static void test_sparse_array(void)
{
    sparse_array<component> og;
    og.emplace_at(9, 9, 9);
    std::cout << "Og:" << std::endl;
    og.dump();

    sparse_array<component> sp = std::move(og);

    sp.emplace_at(2, 42, 42);
    sp.emplace_at(0, 1, 1);
    auto& aaa = sp.insert_at(12, component(84, 42) );
    std::cout << "Address of added = " << &aaa << std::endl;
    std::cout << "Inserted " << aaa.value() << "at pos " << sp.get_index(aaa) << std::endl;
    sp.dump();
    // Erase
    std::cout << "Erasing:" << std::endl;
    size_t size = sp.size();
    for (size_t i = 0; i < size; i++) {
        std::cout << "Sparse_Array size: " << sp.size() << std::endl;
        sp.erase(0);
    }
    std::cout << "After erasure:" << std::endl;
    sp.dump();
}
#endif

static void test_registry(void)
{
    registry reg;

    reg.register_component<component>();
    auto c = reg.get_components<component>();
}

int main(int ac, char **av)
{
#ifdef DEBUG
    test_sparse_array();
#endif

    test_registry();
    return 0;
}
