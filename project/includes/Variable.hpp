#include "Formula.hpp"

#ifndef VARIABLE_HPP
#define VARIABLE_HPP

class Variable : public Formula{
    public:
        Variable(const char name): name(name){}
        void print(std::ostream& os) const override{
            os << name;
        }

        char get_name(){
            return name;
        }

        bool equals(const Formula& other) const override {
            const Variable* var = dynamic_cast<const Variable*>(&other);
            return var && name == var->name;
        }

        Formula* clone() const override{
            return new Variable(name);
        }

    private:
        const char name;
};

#endif