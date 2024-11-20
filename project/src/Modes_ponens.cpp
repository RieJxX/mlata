#include "../includes/Modes_ponens.hpp"

template<typename T1, typename T2> Formula* Modes_ponens(T1 formula1, T2 formula2) {
    if (formula1->equals(*formula2->get_left())) {
        //std::cout << "hello\n";
        return formula2->get_right();
    } else {
        // обработка??
        return nullptr;
    }
}
