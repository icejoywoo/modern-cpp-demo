//
// Created by icejoywoo on 2022/7/17.
//

#ifndef MODERN_CPP_DEMO_STACK1_HPP
#define MODERN_CPP_DEMO_STACK1_HPP

#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
class Stack {
private:
    std::vector<T> elems;
public:
    void push(T const&);
    void pop();
    T top() const;
    bool empty() const {
        return elems.empty();
    }
};

template <typename T>
void Stack<T>::push(T const& elem) {
    elems.push_back(elem);
}

template <typename T>
void Stack<T>::pop() {
    if (elems.empty()) {
        throw std::out_of_range("Stack<T>::pop(): empty stack");
    }
    elems.pop_back();
}

template <typename T>
T Stack<T>::top() const {
    if (elems.empty()) {
        throw std::out_of_range("Stack<T>::top(): empty stack");
    }
    return elems.back();
}

#endif //MODERN_CPP_DEMO_STACK1_HPP
