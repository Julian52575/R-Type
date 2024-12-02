/**
 * @file OwnedMessage.hpp
 * @brief Defines the template class OwnedMessage to associate a message with its owner.
 */

#pragma once

#include "User/User.hpp"
#include "Messages/Message.hpp"

/**
 * @class OwnedMessage
 * @brief Template class representing a message owned by a user.
 *
 * This class associates a user (`User`) with a message (`Message<T>`), allowing
 * tracking of the sender or owner of the message.
 *
 * @tparam T The type of data contained in the message.
 */
template <typename T>
class OwnedMessage {
private:
    User m_owner; ///< The owner of the message (sender).
    Message<T> m_message; ///< Message associated with the owner.

public:
    /**
     * @brief Constructor.
     *
     * @param m_owner The owner of the message.
     * @param msg The message associated with the owner.
     */
    OwnedMessage(User m_owner, const Message<T> &msg);

    /**
     * @brief Retrieves the associated message.
     *
     * @return A reference to the message.
     */
    Message<T> &GetMessage();

    /**
     * @brief Retrieves the owner of the message.
     *
     * @return A reference to the owner (user).
     */
    User &GetOwner();

    /**
     * @brief Destructor.
     */
    ~OwnedMessage();
};

/**
 * @brief Constructor for the OwnedMessage class.
 *
 * @tparam T The type of data contained in the message.
 * @param m_owner The owner of the message.
 * @param msg The message associated with the owner.
 */
template <typename T>
OwnedMessage<T>::OwnedMessage(User m_owner, const Message<T> &msg) 
    : m_owner(m_owner), m_message(msg) {
}

/**
 * @brief Retrieves the associated message.
 *
 * @tparam T The type of data contained in the message.
 * @return A reference to the message.
 */
template <typename T>
Message<T> &OwnedMessage<T>::GetMessage() {
    return m_message;
}

/**
 * @brief Retrieves the owner of the message.
 *
 * @tparam T The type of data contained in the message.
 * @return A reference to the owner (user).
 */
template <typename T>
User &OwnedMessage<T>::GetOwner() {
    return m_owner;
}

/**
 * @brief Destructor for the OwnedMessage class.
 *
 * @tparam T The type of data contained in the message.
 */
template <typename T>
OwnedMessage<T>::~OwnedMessage() {
}
