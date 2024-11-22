#include "../includes/Disjunctive_syllogism.hpp"

template<typename T1, typename T2> Formula* Disjunctive_syllogism(T1 formula1, T2 formula2) {
    // Проверка, является ли formula1 отрицанием левой части formula2
    if (formula1->equals(*formula2->get_left())) {
        // Возвращаем правую часть formula2
        return formula2->get_right();
    } else {
        return nullptr;
    }
}
