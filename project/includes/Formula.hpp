#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

#ifndef FORMULA_HPP
#define FORMULA_HPP

//определение классов
//Базовый класс
class Formula{
    public:
        virtual ~Formula() = default;
        virtual void print(std::ostream& os) const=0;
        virtual bool equals(const Formula& other) const = 0;
        virtual Formula* clone() const = 0;
};

#endif