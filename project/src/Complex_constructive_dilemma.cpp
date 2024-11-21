#include "../includes/Complex_constructive_dilemma.hpp"

template<typename T1, typename T2, typename T3> Formula* Complex_constructive_dilemma(T1 formula1, T2 formula2, T3 formula3) {
    // Проверка, что formula3 является дизъюнкцией левой части formula1 и левой части formula2
    if ((formula3->get_left()->equals(*formula1->get_left()) || formula3->get_left()->equals(*formula2->get_left())) &&
        (formula3->get_right()->equals(*formula1->get_left()) || formula3->get_right()->equals(*formula2->get_left()))) {
        // Возвращаем дизъюнкцию правых частей formula1 и formula2
        return new Or(formula1->get_right(), formula2->get_right());
    } else {
        return nullptr;
    }
}