#ifndef COMPLEX_DESTRUCTIVE_DILEMMA_HPP
#define COMPLEX_DESTRUCTIVE_DILEMMA_HPP

#include "Formula.hpp"
#include "Implies.hpp"
#include "Or.hpp"
#include "Not.hpp"

template<typename T1, typename T2, typename T3> Formula* Complex_destructive_dilemma(T1 formula1, T2 formula2, T3 formula3) {
    // Проверка, что formula3 является дизъюнкцией отрицаний правых частей formula1 и formula2
    if ((formula3->get_left()->equals(*new Not(formula1->get_right())) && formula3->get_right()->equals(*new Not(formula2->get_right()))) ||
        (formula3->get_left()->equals(*new Not(formula2->get_right())) && formula3->get_right()->equals(*new Not(formula1->get_right())))) {
        // Возвращаем дизъюнкцию отрицаний левых частей formula1 и formula2
        return new Or(new Not(formula1->get_left()), new Not(formula2->get_left()));
    } else {
        return nullptr;
    }
}

#endif
