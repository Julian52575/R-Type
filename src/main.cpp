#include "ECS.hpp"
#include "Entity.hpp"
#include "SparseArray.hpp"
#include <exception>

void destroy(Rengine::Entity& e)
{
    std::cout << "Removed " << int(e) << "." << std::endl;
    e.removeComponent<int>();
}

void printAllInt(Rengine::ECS &ecs)
{
    Rengine::SparseArray<int> sp = ecs.registerComponent<int>();

    auto i = 0;
    for (auto &it : sp) {
        std::cout << i << ": ";
        if (it.has_value() == false) {
            std::cout << "NA";
        } else {
            std::cout << it.value();
        }
        std::cout << std::endl;
        i++;
    }
}

int main(void)
{
    Rengine::ECS ecs = Rengine::ECS(5);

    ecs.registerComponent<int>();
    printAllInt(ecs);

    Rengine::Entity& e0 = ecs.addEntity();
    e0.addComponent<int>(0);
    e0.setDestroyFunction(destroy);
    printAllInt(ecs);

    Rengine::Entity& e1 = ecs.addEntity();
    e1.addComponent<int>(1);
    e1.setDestroyFunction(destroy);

    Rengine::Entity& e2 = ecs.addEntity();
    e2.addComponent<int>(2);
    e2.setDestroyFunction(destroy);

    Rengine::Entity& e3 = ecs.addEntity();
    e3.addComponent<int>(3);
    e3.setDestroyFunction(destroy);

    Rengine::Entity& e4 = ecs.addEntity();
    e4.addComponent<int>(4);
    e4.setDestroyFunction(destroy);

    try {
        Rengine::Entity& e5 = ecs.addEntity();
        e5.addComponent<int>(5);
    } catch (std::exception &e) {
        std::cout << "Caught exc when adding e5: " << e.what() << std::endl;
    }
    printAllInt(ecs);
    ecs.removeEntity(e4);
    std::cout << "After removing e4:" << std::endl;
    printAllInt(ecs);
    try {
        ecs.removeEntity(e4);
    } catch (std::exception &e) {
        std::cout << "Caught exc when removing e4: " << e.what() << std::endl;
    }
    Rengine::Entity& e4new = ecs.addEntity();
    e4new.addComponent<int>(84);
    std::cout << "After adding back e4:" << std::endl;
    printAllInt(ecs);
    return 0;
}
