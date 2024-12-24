#include "ECS.hpp"
#include "Entity.hpp"
#include "SparseArray.hpp"
#include <exception>

void intRunnerFunction(const Rengine::ECS&, int& i, Rengine::Entity& e)
{
    std::cout << "intRunnerFunction with [i = " << i << "] [e = " << int(e) << "]." << std::endl;
}

int main(void)
{
    Rengine::ECS ecs = Rengine::ECS(5);
    Rengine::Entity& e0 = ecs.addEntity();
    Rengine::Entity& e1 = ecs.addEntity();

    ecs.registerComponent<int>();
    e0.addComponent<int>(0);
    e1.addComponent<int>(42);
    ecs.setComponentFunction<int>(intRunnerFunction);
    ecs.runComponentFunction<int>();
    std::cout << "Done!" << std::endl;
    return 0;
}
