#include "Formula.hpp"
#include "Implies.hpp"
#include "Not.hpp"

#ifndef MODES_TOLLENS_HPP
#define MODES_TOLLENS_HPP

template<typename T1, typename T2> Formula* Modes_tollens(T1 formula1, T2 formula2) {
    // Проверка, является ли formula2 отрицанием правой части formula1
    if ((formula2->get_operand())->equals(*formula1->get_right())) {
        // Возвращаем отрицание левой части formula1
        return new Not(formula1->get_left());
    } else {
        return nullptr;
    }
}

#endif