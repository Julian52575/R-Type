/*
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
*/

#include "ComponentRegistry.hpp"
#include "Entity.hpp"

void destroy(Rengine::Entity& e)
{
    std::cout << "Removed " << int(e) << "." << std::endl;
}

int main(void)
{
    Rengine::ComponentRegistry reg;
    reg.registerComponent<int>();
    Rengine::Entity en = Rengine::Entity(reg, 42);

    en.setDestroyFunction(destroy);
    en.addComponent<int>(42);
    en.removeComponent<int>();
    en.destroyAllComponents();
    return int(en);
}
