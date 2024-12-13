#include "./CameraFollow.hpp"

System::CameraFollow::CameraFollow() {}

System::CameraFollow::~CameraFollow() {}

void System::CameraFollow::update(sparse_array<Position>& positions, sparse_array<Camera>& cameras, sf::RenderWindow& window) {
    for (uint32_t i = 0; i < positions.size() && i < cameras.size(); i++) {
        if (positions[i].has_value() && cameras[i].has_value()) {
            cameras[i].value().update(positions[i].value().x, positions[i].value().y);
            window.setView(cameras[i].value().getView());
        }
    }
}
