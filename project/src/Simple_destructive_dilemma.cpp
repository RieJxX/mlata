#include "../includes/Simple_destructive_dilemma.hpp"

template<typename T1, typename T2, typename T3> Formula* Simple_destructive_dilemma(T1 formula1, T2 formula2, T3 formula3) {
    // Проверка, что левая часть formula1 равна левой части formula2
    // и что formula3 является дизъюнкцией отрицаний правых частей formula1 и formula2
    if ((formula1->get_left()->equals(*formula2->get_left())) &&
        ((formula3->get_left()->equals(*new Not(formula1->get_right())) && formula3->get_right()->equals(*new Not(formula2->get_right()))) ||
         (formula3->get_left()->equals(*new Not(formula2->get_right())) && formula3->get_right()->equals(*new Not(formula1->get_right()))))) {
        // Возвращаем отрицание левой части formula1 (или formula2, так как они равны)
        return new Not(formula1->get_left());
    } else {
        return nullptr;
    }
}