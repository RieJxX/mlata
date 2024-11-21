#include "../includes/Hypothetical_syllogism.hpp"

template<typename T1, typename T2> Formula* Hypothetical_syllogism(T1 formula1, T2 formula2) {
    // Проверка, является ли правая часть formula1 левой частью formula2
    if (formula1->get_right()->equals(*formula2->get_left())) {
        // Возвращаем новую импликацию с левой частью formula1 и правой частью formula2
        return new Implies(formula1->get_left(), formula2->get_right());
    } else {
        return nullptr;
    }
}