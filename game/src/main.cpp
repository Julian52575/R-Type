#include <rengine/Rengine.hpp>
#include <SFML/Main.hpp>

int main(void) {
    GraphicalCore core;
    core.MakeEntity("entities/player.json");
    core.MakeEntity("entities/projectile.json");
    core.MakeEntity("entities/Message.json");
    // // core.MakeBackground();
    // core.MakeParallaxBackground("assets/images/parallax background/plx-1.png", 0.0f);
    // core.MakeParallaxBackground("assets/images/parallax background/plx-2.png", -15.f);
    // core.MakeParallaxBackground("assets/images/parallax background/plx-3.png", -30.f);
    // core.MakeParallaxBackground("assets/images/parallax background/plx-4.png", -45.f);
    // core.MakeParallaxBackground("assets/images/parallax background/plx-5.png", -60.f);
    core.run();
    return 0;
}