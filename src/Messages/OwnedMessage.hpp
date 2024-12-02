#pragma once
#include "User/User.hpp"
#include "Messages/Message.hpp"

template <typename T>
class OwnedMessage {
 private:
  User m_owner;
  Message<T> m_message;

 public:
  OwnedMessage(User m_owner, const Message<T> &msg);
  Message<T> &GetMessage();
  User &GetOwner();
  ~OwnedMessage();
};


template <typename T>
OwnedMessage<T>::OwnedMessage(User m_owner, const Message<T> &msg) : m_owner(m_owner), m_message(msg) {
}

template <typename T>
Message<T> &OwnedMessage<T>::GetMessage() {
    return m_message;
}

template <typename T>
User &OwnedMessage<T>::GetOwner() {
    return m_owner;
}

template <typename T>
OwnedMessage<T>::~OwnedMessage() {
}
