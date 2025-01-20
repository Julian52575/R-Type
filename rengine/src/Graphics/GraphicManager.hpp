//
#ifndef SRC_GRAPHICS_GRAPHICMANAGER_HPP_
#define SRC_GRAPHICS_GRAPHICMANAGER_HPP_
#include <memory>
#include <string>

// SFML Includes if define
#ifdef __RENGINEGRAPHICS_USELIB_SFML
    #include "./SFML/SFMLWindow.hpp"
    #include "./SFML/SFMLSprite.hpp"
    #include "./SFML/SFMLSound.hpp"
#endif
// SFL Includes if define
#ifdef __RENGINEGRAPHICS_USELIB_SDL
    // #warning "SDL not implemented yet."
    #include "./SDL/SDLWindow.hpp"
#endif

#include "Vector.hpp"
#include "AWindow.hpp"
#include "ASprite.hpp"
#include "ASound.hpp"
#include "SpriteSpecs.hpp"
#include "WindowSpecs.hpp"
#include "SoundSpecs.hpp"
#include "TextSpecs.hpp"

namespace Rengine {
    namespace Graphics {

        class GraphicManagerException : public std::exception {
            public:
                explicit GraphicManagerException(const std::string& msg) : _msg("Rengine::Graphics::GraphicManager: " + msg) {}
                const char *what() const noexcept
                {
                    return this->_msg.c_str();
                };

            private:
                std::string _msg;
        };
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class GraphicManager
        * @brief The entrypoint class for managing graphics in the rengine.
        */
        class GraphicManager {
            public:
                /**
                * @fn GraphicManager
                * @brief Create the graphic manager and the window.
                */
                GraphicManager();
                ~GraphicManager(void) = default;
                /**
                * @fn createWindow
                * @param windowSpecs The specs of the to-be-created window
                * @brief Create the window following the specs.
                */
                void createWindow(const WindowSpecs& windowSpecs = WindowSpecs());
                /**
                * @fn getWindow
                * @brief Return a reference to the window.
                * If the window wasn't created previously, creates it with default settings
                */
                std::shared_ptr<AWindow>& getWindow(void) noexcept;
                /**
                * @fn createSprite
                * @param spriteSpecs The specs of the new sprite.
                * @return std::shared_ptr<ASprite> A pointer to a sprite object renderable by the window.
                * @brief Create a new sprite following the spriteSpecs,
                * this sprite is sure to be usable with the window returned by this->getWindow().
                * Note: this sprite is not stored in the GraphicManager.
                * Note: If a window wasn't created previously, creates it with default settings
                */
                std::shared_ptr<ASprite> createSprite(const SpriteSpecs& spriteSpecs);
                /**
                * @fn createText
                * @param textSpecs The specs of the new text.
                * @return std::shared_ptr<AText> A pointer to a text object renderable by the window.
                * @brief Create a new text following the textSpecs,
                * Note: this text is not stored in the GraphicManager.
                */
                std::shared_ptr<AText> createText(const TextSpecs& textSpecs);
                /**
                * @fn createSound
                * @param soundSpecs The specs of the new sound / music.
                * @return std::shared_ptr<ASound> A pointer to a sound object.
                * @brief Create a new sound / music following the soundSpecs,
                * Note: this sound is not stored in the GraphicManager.
                */
                std::shared_ptr<ASound> createSound(const SoundSpecs& soundSpecs);
                /**
                * @fn addToRender
                * @param sprite The sprite to render.
                * @param position The window position to render the sprite.
                * @param updateAnimationFrame Whether or not the window should update the sprite's animation before rendering. True by default.
                * @brief Add a sprite to the render.
                */
                void addToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite,
                    const Rengine::Graphics::vector2D<float>& position, bool updateAnimationFrame = true);
                /**
                * @fn addToRender
                * @param text The sprite to render.
                * @param position The window position to render the text.
                * @brief Add a text to the render.
                */
                void addToRender(const std::shared_ptr<Rengine::Graphics::AText>& text,
                    const Rengine::Graphics::vector2D<float>& position);

            private:
                std::shared_ptr<AWindow> _window = nullptr;
        };  // class GraphicManager

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class GraphicManagerSingletone
        * @brief The singletone class for GraphicManager.
        */
        class GraphicManagerSingletone {
            public:
                GraphicManagerSingletone(void) = default;
                ~GraphicManagerSingletone(void) = default;
                /**
                * @fn getSingletone
                * @return A static reference to the same GraphicManager object.
                * @brief Return a reference to the static GraphicManager
                */
                static GraphicManager& get(void) noexcept {
                    static GraphicManager manager;

                    return manager;
                }
        };  // class GraphicManagerSingletone

    }  // namespace Graphics
}  // namespace Rengine
#endif  // SRC_GRAPHICS_GRAPHICMANAGER_HPP_
