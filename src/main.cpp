#include "ComponentRegistry.hpp"

int main(void)
{
    Rengine::ComponentRegistry a;
    Rengine::SparseArray<int>& s = a.registerComponent<int>();
    Rengine::SparseArray<int>& s2 = a.getComponents<int>();

    s.emplaceAt(15, 15);
    s.insertAt(42, 42);
    s.insertAt(84, 84);
    s.erase(15);
    s.erase(42);
    std::cout << s[15].has_value() << std::endl;
    std::cout << s[42].has_value() << std::endl;
    std::cout << s[84].value() << std::endl;
    return 84;
}
