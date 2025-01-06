#include "WindowParser.hpp"
#include "Window.hpp"

bsf::WindowParser::WindowParser() {
    window = nullptr;
}


bsf::WindowParser::~WindowParser() {
}

void bsf::WindowParser::setWindow(bsf::RenderWindow& window) {
    this->window = &window;
}

void bsf::WindowParser::parse(const std::string& path) {
    if (window == nullptr)
        throw std::runtime_error("Window not set");

    file.open(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);
    
    try {
        file >> json;
    } catch (nlohmann::json::parse_error& e) {
        throw std::runtime_error("Could not parse json file: " + path);
    }

    this->parseTitle();
    try{
        this->parseResolution();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << ": Using default resolution" << std::endl;
    }
    this->parseFramerateLimit();

    try {
        this->parseBackgroundColor();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << ": Using default color" << std::endl;
    }
    try {
        this->parseIcon();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << ": Using no icon" << std::endl;
    }
    this->parseScreenMode();
    this->parseVsync();
    this->parseCursor();
    try {
        this->parseView();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << ": Using default view" << std::endl;
    }
    parseInput();
    file.close();
}

void bsf::WindowParser::parseTitle() {
    if (json.contains("title"))
        window->setTitle(std::string(json["title"]));
}

void bsf::WindowParser::parseResolution() {
    if (json.contains("resolution")) {
        if (!json["resolution"].contains("width") || !json["resolution"].contains("height"))
            throw std::runtime_error("resolution must contain width and height");
        if (json["resolution"]["width"] <= 0 || json["resolution"]["height"] <= 0)
            throw std::runtime_error("height and width must be positive and non-zero");
        window->setSize(sf::Vector2u(json["resolution"]["width"], json["resolution"]["height"]));
    }
}


void bsf::WindowParser::parseFramerateLimit() {
    if (json.contains("framerate_limit"))
        window->setFramerateLimit(json["framerate_limit"]);
}

void bsf::WindowParser::parseBackgroundColor() {
    if (json.contains("background_color")) {
        if (!json["background_color"].contains("r") || !json["background_color"].contains("g") || !json["background_color"].contains("b"))
            throw std::runtime_error("Invalid color format");
        if (json["background_color"]["r"] < 0 || json["background_color"]["r"] > 255 || json["background_color"]["g"] < 0 || json["background_color"]["g"] > 255 || json["background_color"]["b"] < 0 || json["background_color"]["b"] > 255)
            throw std::runtime_error("Invalid color value");
        window->setBackgroundColor(sf::Color(json["background_color"]["r"], json["background_color"]["g"], json["background_color"]["b"]));
    }
}

void bsf::WindowParser::parseIcon() {
    if (json.contains("icon")) {
        sf::Image icon;
        if (!icon.loadFromFile(json["icon"]))
            throw std::runtime_error("Could not load icon");

        window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
}

void bsf::WindowParser::parseScreenMode() {
    unsigned int style = sf::Style::Titlebar;

    if (json.contains("fullscreen") && (json["fullscreen"]))
            style = sf::Style::Fullscreen;
    else if (json.contains("borderless") && (json["borderless"]))
        style = sf::Style::None;
    else{
        if ((json.contains("resizable") && json["resizable"]) || !(json.contains("resizable"))) 
            style |= sf::Style::Resize;
        if ((json.contains("closable") && json["closable"]) || !(json.contains("closable")))
            style |= sf::Style::Close;
    }

    window->create(
        (style == sf::Style::Fullscreen) ? sf::VideoMode::getDesktopMode() : sf::VideoMode(window->getSize().x, window->getSize().y),
        window->getTitle(),
        style
    );
}

void bsf::WindowParser::parseVsync() {
    if (json.contains("vsync"))
        window->setVerticalSyncEnabled(json["vsync"]);
}

void bsf::WindowParser::parseCursor() {
    if (json.contains("cursor")) {
        window->setMouseCursorVisible(json["cursor"]);
    }
}

void bsf::WindowParser::parseView() {
    if (json.contains("view")) {
        if (!json["view"].contains("center") || !json["view"].contains("size"))
            throw std::runtime_error("View must contain center and size");
        if (!json["view"]["center"].contains("x") || !json["view"]["center"].contains("y"))
            throw std::runtime_error("Center must contain x and y");
        if (!json["view"]["size"].contains("width") || !json["view"]["size"].contains("height"))
            throw std::runtime_error("Size must contain width and height");
        window->setView(sf::View(sf::Vector2f(json["view"]["center"]["x"], json["view"]["center"]["y"]), sf::Vector2f(json["view"]["size"]["width"], json["view"]["size"]["height"]))); 
    }
}

void bsf::WindowParser::parseInput() {

    std::unordered_map<std::string,sf::Keyboard::Key> dict = {
        {"A", sf::Keyboard::A},
        {"B", sf::Keyboard::B},
        {"C", sf::Keyboard::C},
        {"D", sf::Keyboard::D},
        {"E", sf::Keyboard::E},
        {"F", sf::Keyboard::F},
        {"G", sf::Keyboard::G},
        {"H", sf::Keyboard::H},
        {"I", sf::Keyboard::I},
        {"J", sf::Keyboard::J},
        {"K", sf::Keyboard::K},
        {"L", sf::Keyboard::L},
        {"M", sf::Keyboard::M},
        {"N", sf::Keyboard::N},
        {"O", sf::Keyboard::O},
        {"P", sf::Keyboard::P},
        {"Q", sf::Keyboard::Q},
        {"R", sf::Keyboard::R},
        {"S", sf::Keyboard::S},
        {"T", sf::Keyboard::T},
        {"U", sf::Keyboard::U},
        {"V", sf::Keyboard::V},
        {"W", sf::Keyboard::W},
        {"X", sf::Keyboard::X},
        {"Y", sf::Keyboard::Y},
        {"Z", sf::Keyboard::Z},
        {"Num0", sf::Keyboard::Num0},
        {"Num1", sf::Keyboard::Num1},
        {"Num2", sf::Keyboard::Num2},
        {"Num3", sf::Keyboard::Num3},
        {"Num4", sf::Keyboard::Num4},
        {"Num5", sf::Keyboard::Num5},
        {"Num6", sf::Keyboard::Num6},
        {"Num7", sf::Keyboard::Num7},
        {"Num8", sf::Keyboard::Num8},
        {"Num9", sf::Keyboard::Num9},
        {"Escape", sf::Keyboard::Escape},
        {"LControl", sf::Keyboard::LControl},
        {"LShift", sf::Keyboard::LShift},
        {"LAlt", sf::Keyboard::LAlt},
        {"LSystem", sf::Keyboard::LSystem},
        {"RControl", sf::Keyboard::RControl},
        {"RShift", sf::Keyboard::RShift},
        {"RAlt", sf::Keyboard::RAlt},
        {"RSystem", sf::Keyboard::RSystem},
        {"Menu", sf::Keyboard::Menu},
        {"LBracket", sf::Keyboard::LBracket},
        {"RBracket", sf::Keyboard::RBracket},
        {"SemiColon", sf::Keyboard::SemiColon},
        {"Comma", sf::Keyboard::Comma},
        {"Period", sf::Keyboard::Period},
        {"Quote", sf::Keyboard::Quote},
        {"Slash", sf::Keyboard::Slash},
        {"BackSlash", sf::Keyboard::BackSlash},
        {"Tilde", sf::Keyboard::Tilde},
        {"Equal", sf::Keyboard::Equal},
        {"Dash", sf::Keyboard::Dash},
        {"Space", sf::Keyboard::Space},
        {"Return", sf::Keyboard::Return},
        {"BackSpace", sf::Keyboard::BackSpace},
        {"Tab", sf::Keyboard::Tab},
        {"PageUp", sf::Keyboard::PageUp},
        {"PageDown", sf::Keyboard::PageDown},
        {"End", sf::Keyboard::End},
        {"Home", sf::Keyboard::Home},
        {"Insert", sf::Keyboard::Insert},
        {"Delete", sf::Keyboard::Delete},
        {"Add", sf::Keyboard::Add},
        {"Subtract", sf::Keyboard::Subtract},
        {"Multiply", sf::Keyboard::Multiply},
        {"Divide", sf::Keyboard::Divide},
        {"Left", sf::Keyboard::Left},
        {"Right", sf::Keyboard::Right},
        {"Up", sf::Keyboard::Up},
        {"Down", sf::Keyboard::Down},
        {"Numpad0", sf::Keyboard::Numpad0},
        {"Numpad1", sf::Keyboard::Numpad1},
        {"Numpad2", sf::Keyboard::Numpad2},
        {"Numpad3", sf::Keyboard::Numpad3},
        {"Numpad4", sf::Keyboard::Numpad4},
        {"Numpad5", sf::Keyboard::Numpad5},
        {"Numpad6", sf::Keyboard::Numpad6},
        {"Numpad7", sf::Keyboard::Numpad7},
        {"Numpad8", sf::Keyboard::Numpad8},
        {"Numpad9", sf::Keyboard::Numpad9},
        {"F1", sf::Keyboard::F1},
        {"F2", sf::Keyboard::F2},
        {"F3", sf::Keyboard::F3},
        {"F4", sf::Keyboard::F4},
        {"F5", sf::Keyboard::F5},
        {"F6", sf::Keyboard::F6},
        {"F7", sf::Keyboard::F7},
        {"F8", sf::Keyboard::F8},
        {"F9", sf::Keyboard::F9},
        {"F10", sf::Keyboard::F10},
        {"F11", sf::Keyboard::F11},
        {"F12", sf::Keyboard::F12},
        {"F13", sf::Keyboard::F13},
        {"F14", sf::Keyboard::F14},
        {"F15", sf::Keyboard::F15},
        {"Pause", sf::Keyboard::Pause},
        {"Unknown", sf::Keyboard::Unknown}
    };


    if (json.contains("inputs")) {
        if (json["inputs"].contains("close"))
            window->closeInput = dict.find(std::string(json["inputs"]["close"]))->second;
        if (json["inputs"].contains("fullscreen"))
            window->fullscreenInput = dict.find(std::string(json["inputs"]["fullscreen"]))->second;
    }
}