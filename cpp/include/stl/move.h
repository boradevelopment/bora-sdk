#pragma once
namespace bora::stl {
    // Move Function for objects like strings or shared pointers
template <typename T> struct remove_reference      { using type = T; };
template <typename T> struct remove_reference<T&>  { using type = T; };
template <typename T> struct remove_reference<T&&> { using type = T; };

/// @brief Moves an object of the same type to the new object
/// @tparam T The type [Both have to be the same (sending and recieving)]
/// @param object The object that is being moved
/// @return The newly moved object of the same type.
template<typename T>
constexpr typename remove_reference<T>::type&& move(T&& object) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(object);
}
}
