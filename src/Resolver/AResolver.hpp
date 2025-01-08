//
#ifndef SRC_RESOLVER_ARESOLVER_HPP_
#define SRC_RESOLVER_ARESOLVER_HPP_
#include <unordered_map>

namespace Rengine {

    namespace Resolver {

        class AResolverExceptionUnknowKey : public std::exception {
            public:
                const char *what() const noexcept { return "Rengine::Resolver::AResolver: Cannot find an element associated to the provided key."; };
        };
        /**
        * @addtogroup Resolver
        * @namespace Resolver
        * @class AResolver
        * @brief An abstract class to help users write their own resolver.
        * A resolver creates a value and stores it for faster access.
        * Ie: Trade memory for faster configuration file parsing.
        */
        template<class keyType, class resultType>
        class AResolver {
            public:
                ~AResolver() = default;
                /**
                * @fn get
                * @param key The key to create / get an element.
                * @return The 'resultType' binded to 'key'.
                * @brief Either create a new 'resultType' and stores it OR fetch the previously created object.
                */
                resultType& get(const keyType& key)
                {
                    auto it = this->_map.find(key);

                    if (it != this->_map.end()) {
                        return this->_map[key];
                    }
                    this->_map[key] = this->createNewElement(key);
                    return this->_map[key];
                };
                /**
                * @fn remove
                * @param key The key of the previously created element.
                * @exception AResolverExceptionUnknowKey No element has been previously registered with the provided key.
                * @brief Remove the element refered by key
                */
                void remove(const keyType& key)
                {
                    auto it = this->_map.find(key);

                    if (it == this->_map.end()) {
                        throw AResolverExceptionUnknowKey();
                    }
                    this->_map.erase(key);
                }

            protected:
                std::unordered_map<keyType, resultType> _map;

            private:
                /**
                * @fn createNewElement
                * @param key The key to create a new element.
                * @brief Create a new 'resultType' object and returns it.
                */
                inline virtual resultType createNewElement(const keyType& key) = 0;
        };  // class AResolver
    }  // namespace Resolver
}  // namespace Rengine

#endif  // SRC_RESOLVER_ARESOLVER_HPP_
