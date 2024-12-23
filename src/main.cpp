#include "ECS.hpp"
#include "SparseArray.hpp"

void destroy(Rengine::Entity& e)
{
    std::cout << "Removed " << int(e) << "." << std::endl;
}

int main(void)
{
    Rengine::ECS ecs = Rengine::ECS(90);
    Rengine::SparseArray<int> sp = ecs.registerComponent<int>();

    sp.emplaceAt(5, 5);
    sp.emplaceAt(12, 12);
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
    return sp.size();
}
