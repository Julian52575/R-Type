#include <cstddef>
#include <memory>
#include <vector>

#include "UserInputManager.hpp"
#include "Vector.hpp"

namespace Rengine {
    namespace Graphics {

        UserInputManager::UserInputManager(std::size_t maxInputPerFrame)
        {
            this->_maxInputPerFrame = maxInputPerFrame;
            this->_inputVector.reserve(maxInputPerFrame);
        }
        UserInputManager::const_iterator UserInputManager::begin(void) const
        {
            return this->_inputVector.begin();
        }
        UserInputManager::const_iterator UserInputManager::end(void) const
        {
            return this->_inputVector.end();
        }

        void UserInputManager::addInput(const Rengine::Graphics::UserInput& input)
        {
            if (this->_inputVector.size() >= this->_maxInputPerFrame) {
                return;
            }
            this->_inputVector.push_back(input);
        }
        void UserInputManager::reset(void)
        {
            this->_inputVector.clear();
        }
        UserInputManager::size_type UserInputManager::size(void) const noexcept
        {
            return this->_inputVector.size();
        }
    }  // namespace Graphic
}  // namespace Rengine
