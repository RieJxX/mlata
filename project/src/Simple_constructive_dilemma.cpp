#include "../includes/Simple_constructive_dilemma.hpp"

template<typename T1, typename T2, typename T3> Formula* Simple_constructive_dilemma(T1 formula1, T2 formula2, T3 formula3) {
    // Проверка, что правые части formula1 и formula2 равны
    // и что formula3 является дизъюнкцией левой части formula1 и левой части formula2
    if (formula1->get_right()->equals(*formula2->get_right()) &&
        (formula3->get_left()->equals(*formula1->get_left()) || formula3->get_left()->equals(*formula2->get_left())) &&
        (formula3->get_right()->equals(*formula1->get_left()) || formula3->get_right()->equals(*formula2->get_left()))) {
        // Возвращаем правую часть formula1 (или formula2, так как они равны)
        return formula1->get_right();
    } else {
        return nullptr;
    }
}