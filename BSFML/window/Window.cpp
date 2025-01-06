#include "Window.hpp"

bsf::RenderWindow::RenderWindow(const std::string& configfile) : sf::RenderWindow(sf::VideoMode(800, 600), "SFML Window") {
    backgroundColor = sf::Color::Black;
    closeInput = sf::Keyboard::Key::Unknown;
    fullscreenInput = sf::Keyboard::Key::Unknown;
    parser.setWindow(*this);
    try{
        parser.parse(configfile);
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << ": Using default values" << std::endl;
    }
};


std::string &bsf::RenderWindow::getTitle() {
    return title;
};

void bsf::RenderWindow::setTitle(const std::string& title) {
    sf::RenderWindow::setTitle(title);
    this->title = title;
};

void bsf::RenderWindow::setBackgroundColor(const sf::Color& color) {
    backgroundColor = color;
};

void bsf::RenderWindow::clear() {
    sf::RenderWindow::clear(backgroundColor);
};

void bsf::RenderWindow::clear(const sf::Color& color) {
    sf::RenderWindow::clear(color);
};

bool bsf::RenderWindow::pollEvent(sf::Event& event) {
    bool hasEvent = sf::RenderWindow::pollEvent(event);
    if (!hasEvent)
        return false;
    if (event.type == sf::Event::Closed) {
        sf::RenderWindow::close();
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == this->closeInput) {
            sf::RenderWindow::close();
        }
        if (event.key.code == this->fullscreenInput) {
            sf::RenderWindow::create(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
        }
    }
    return true;
};

bsf::RessourceManager<bsf::Texture> &bsf::RenderWindow::getTextureManager() {
    return textureManager;
};

bsf::RessourceManager<sf::Font> &bsf::RenderWindow::getFontManager() {
    return fontManager;
};

bsf::RessourceManager<sf::SoundBuffer> &bsf::RenderWindow::getSoundManager() {
    return soundManager;
};

bsf::RessourceManager<sf::Music> &bsf::RenderWindow::getMusicManager() {
    return musicManager;
};

bsf::RessourceManager<sf::Shader> &bsf::RenderWindow::getShaderManager() {
    return shaderManager;
};

bsf::Vector2 bsf::RenderWindow::getMousePositionLocal() {
    return sf::Mouse::getPosition(*this);
};

bsf::Vector2 bsf::RenderWindow::getMousePositionGlobal() {
    return sf::Mouse::getPosition();
};

float bsf::RenderWindow::getDeltaTime() {
    return clock.restart().asSeconds();
};