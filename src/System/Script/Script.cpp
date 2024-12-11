#include "Script.hpp"

System::Script::Script() {}

System::Script::~Script() {}

void System::Script::updateMovement(sparse_array<Scripting>& scripting, sparse_array<Velocity>& velo, sparse_array<Position>& pos) {
    for (uint32_t i = 0; i < scripting.size() && i < velo.size() && i < pos.size(); i++) {
        if (scripting[i].has_value() && velo[i].has_value() && pos[i].has_value()) {
            // execute le script ici
            std::vector<std::any> rep = scripting[i].value().callFunction("movement",pos[i].value().x, pos[i].value().y, velo[i].value().x, velo[i].value().y);
            if (rep.size() == 4) {
                pos[i].value().x = std::any_cast<double>(rep[0]);
                pos[i].value().y = std::any_cast<double>(rep[1]);
                velo[i].value().x = std::any_cast<double>(rep[2]);
                velo[i].value().y = std::any_cast<double>(rep[3]);
            }
        }
    }
}

void System::Script::updateAttack(sparse_array<Scripting>& scripting, sparse_array<Attack>& attack, sparse_array<Position>& pos, float deltaTime, std::function<void(float, float)> spawnProjectile) {
    for (uint32_t i = 0; i < scripting.size() && i < attack.size() && i < pos.size(); i++) {
        if (scripting[i].has_value() && attack[i].has_value() && pos[i].has_value()) {
            attack[i].value().update(deltaTime);
            std::vector<std::any> rep = scripting[i].value().callFunction("attack",attack[i].value().canAttack(),pos[i].value().x, pos[i].value().y);
            if (rep.size() == 1) {
                if (std::any_cast<bool>(rep[0])) {
                    attack[i].value().reset();
                    spawnProjectile(pos[i].value().x, pos[i].value().y);
                }
            }
        }
    }
}