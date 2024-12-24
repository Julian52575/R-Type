#include "ECS.hpp"
#include "Entity.hpp"
#include "SparseArray.hpp"
#include <exception>

void intRunnerFunction(const Rengine::ECS& ecs, int& i, Rengine::Entity& e)
{
    std::cout << "intRunnerFunction with [i = " << i << "] [e = " << int(e) << "]." << std::endl;
    ecs.runComponentFunction<float>();
}

void floatRunnerFunction(const Rengine::ECS& ecs, float& f, Rengine::Entity& e)
{
    std::cout << "floatRunnerFunction with [f = " << f << "] [e = " << int(e) << "]." << std::endl;
}

int main(void)
{
    Rengine::ECS ecs = Rengine::ECS(5);
    Rengine::Entity& e0 = ecs.addEntity();
    Rengine::Entity& e1 = ecs.addEntity();

    ecs.registerComponent<int>();
    ecs.setComponentFunction<float>(floatRunnerFunction);
    ecs.registerComponent<float>();
    ecs.setComponentFunction<int>(intRunnerFunction);
    e0.addComponent<int>(0);
    e0.addComponent<float>(0.1);
    e1.addComponent<int>(42);
    e1.addComponent<float>(123.456);
    ecs.runComponentFunction<int>();
    std::cout << "Done!" << std::endl;
    return 0;
}
