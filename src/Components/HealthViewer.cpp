#include "HealthViewer.hpp"

namespace RType{

    namespace Components {

        void HealthViewer::componentFunction(Rengine::ECS& ecs, RType::Components::HealthViewer& health_view, Rengine::Entity& entity){
            std::cout << "HealthViwer::componentFunction" << std::endl;
        }

    }
}