#include "Core/Graphical/GraphicalCore.hpp"
#include "Component/Liste.hpp"


int main(void) {
    GraphicalCore core;
    // core.MakeBackground();
    core.MakeParallaxBackground("Jungle Asset Pack/Jungle Asset Pack/parallax background/plx-1.png", 0.0f);
    core.MakeParallaxBackground("Jungle Asset Pack/Jungle Asset Pack/parallax background/plx-2.png", -15.0f);
    core.MakeParallaxBackground("Jungle Asset Pack/Jungle Asset Pack/parallax background/plx-3.png", -30.0f);
    core.MakeParallaxBackground("Jungle Asset Pack/Jungle Asset Pack/parallax background/plx-4.png", -45.0f);
    core.MakeParallaxBackground("Jungle Asset Pack/Jungle Asset Pack/parallax background/plx-5.png", -60.0f);
    core.MakePlayer();
    core.run();
    return 0;
}
