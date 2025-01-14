// a test
#include <rengine/src/Graphics/ASound.hpp>
#include <rengine/src/Graphics/ASprite.hpp>
#include <rengine/src/Graphics/AText.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/SpriteSpecs.hpp>
#include <rengine/src/Graphics/TextSpecs.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/WindowSpecs.hpp>
#include <unistd.h>
#include <memory>
#include <iostream>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>

static void input(void)
{
    Rengine::Graphics::UserInputManager& manager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();

    for (auto it : manager) {
        std::cout << it << std::endl;
        if (it.type == Rengine::Graphics::UserInputTypeWindowClosed) {
            Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->close();
            std::cout << "Close" << std::endl;
        }
    }
}

static std::shared_ptr<Rengine::Graphics::ASprite> initCircle(void)
{
    Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
    Rengine::Graphics::SpriteSpecs specs;

    specs.type = Rengine::Graphics::SpriteTypeCircle;
    //specs.shapeData.specifics.circleRadius = 150;
    specs.color = {255, 100, 100};
    specs.shapeData.outlineColor = {255, 0, 0};
    specs.opacity = 0;
    return manager.createSprite(specs);
}

static std::shared_ptr<Rengine::Graphics::ASprite> initRectangle(void)
{
    Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
    Rengine::Graphics::SpriteSpecs specs;

    specs.type = Rengine::Graphics::SpriteTypeRectangle;
    //specs.shapeData.specifics.rectangleSize = {150, 150};
    specs.color = {100, 100, 255};
    specs.shapeData.outlineColor = {0, 0, 255};
    return manager.createSprite(specs);
}

static std::shared_ptr<Rengine::Graphics::ASprite> initSprite(void)
{
    Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
    Rengine::Graphics::SpriteSpecs specs;

    specs.texturePath = "marioSheet.png";
    specs.type = Rengine::Graphics::SpriteTypeSprite;
    specs.originOffset.x = 50;
    specs.originOffset.y = 50;
    specs.textureScale = {5, 5};
    specs.animation = Rengine::Graphics::SpriteSpecs::SpriteAnimationSpecs();
    specs.animation->frameCount = 7;
    specs.animation->frameDisplacement = {15, 0};
    specs.animation->frameDuration = 0.08;
    specs.animation->frameRectWidthHeight = {25, 23};
    specs.animation->frameRectXY = {5, 0};
    return manager.createSprite(specs);
}

static std::shared_ptr<Rengine::Graphics::AText> initText(void)
{
    Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
    Rengine::Graphics::TextSpecs specs;

    specs.fontPath = "Mario-Kart-DS.ttf";
    specs.color = {255, 255, 255};
    specs.outlineColor = {0, 0, 0};
    return manager.createText(specs);
}

static std::shared_ptr<Rengine::Graphics::AText> initFancyText(void)
{
    Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
    Rengine::Graphics::TextSpecs specs;

    specs.message = "Sussy baka sfml\nNewline.";
    specs.fontPath = "arial_narrow_7.ttf";
    specs.style |= Rengine::Graphics::TextStyle::TextStyleBold
        | Rengine::Graphics::TextStyleItalic
        | Rengine::Graphics::TextStyleStrikeThrough
        | Rengine::Graphics::TextStyleUnderline
        ;
    return manager.createText(specs);
}

static std::shared_ptr<Rengine::Graphics::ASound> initMusic(void)
{
    Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
    Rengine::Graphics::SoundSpecs specs;

    specs.soundPath = "starforx64_training_mode.mp3";
    //specs.soundPath = "peproll1.wav";
    specs.loop = true;
    specs.loopPointsMicroseconds.x = 0;
    specs.loopPointsMicroseconds.y = -1;
    specs.playingOffsetMicroseconds =150000;
    specs.pitch = 1;
    specs.volume = 100;
    return manager.createSound(specs);
}

static void controlSound(std::shared_ptr<Rengine::Graphics::ASound> sound)
{
    Rengine::Graphics::UserInputManager& manager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
    static float deltatime = 0;

    deltatime = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getDeltaTimeSeconds();
    if (deltatime < 1.0f) {
        return;
    }
    if (manager.receivedInput({Rengine::Graphics::UserInputTypeKeyboardChar, ' '})) {
        if (sound->isPlaying()) {
            sound->pause();
        } else {
            sound->play();
        }
        std::cout << "Play" << std::endl;
        Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->resetDeltatime();
        return;
    }
    if (manager.receivedInput({Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialENTER})) {
        sound->reset();
        std::cout << "Reset" << std::endl;
        Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->resetDeltatime();
        return;
    }
}

static void inputFlipSprite(std::shared_ptr<Rengine::Graphics::ASprite> sprite)
{
    Rengine::Graphics::UserInputManager& manager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();

    for (auto it : manager) {
        std::cout << it.type << std::endl;
        if (it.type == Rengine::Graphics::UserInputTypeMouseLeftClick) {
            sprite->flip();
            std::cout << "Flip !" << std::endl;
        }
    }
}

static std::shared_ptr<Rengine::Graphics::AText> initTextBox(void)
{
    Rengine::Graphics::TextSpecs specs;

    specs.color = {0, 0, 255};
    specs.message = "";
    specs.fontPath = "arial_narrow_7.ttf";
    return Rengine::Graphics::GraphicManagerSingletone::get().createText(specs);
}

static void inputTextbox(std::shared_ptr<Rengine::Graphics::AText> &textBox)
{
    Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
    std::string currentText = textBox->getText();

    for (auto it : inputManager) {
        if (it.type != Rengine::Graphics::UserInputTypeKeyboardChar && it.type != Rengine::Graphics::UserInputTypeKeyboardCharPressed) {
            continue;
        }
        currentText += it.data.keyboardChar;
    }
    textBox->setText(currentText);
}

int main(void)
{
    Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
    Rengine::Graphics::WindowSpecs specs;
    auto circle = initCircle();
    auto rectangle = initRectangle();
    auto sprite = initSprite();
    auto text = initText();
    auto fancyText = initFancyText();
    auto music = initMusic();
    auto textBox = initTextBox();

    music->play();
    specs.resolution = {800, 800};
    specs.options.isFullscreen = false;
    specs.options.isBorderless = false;
    manager.createWindow(specs);
    while (manager.getWindow()->isOpen()) {
        manager.getWindow()->pollInput();
        controlSound(music);
        input();
        inputFlipSprite(sprite);
        inputTextbox(textBox);
        manager.getWindow()->getInputManager().clear();
        manager.addToRender(circle, {0, 0});
        manager.addToRender(sprite, {300, 300});
        manager.addToRender(rectangle, {600, 600});
        manager.addToRender(text, {100, 0});
        manager.addToRender(fancyText, {100, 100});
        manager.addToRender(textBox, {0, 500});
        manager.getWindow()->render();
    }
}
