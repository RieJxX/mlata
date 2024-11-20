#include "Formula.hpp"

#ifndef NOT_HPP
#define NOT_HPP

class Not : public Formula {
    public:
        Not(Formula* operand) : operand(std::move(operand)) {}

        void print(std::ostream& os) const override {
            os << "!";
            if (operand == nullptr) os << " ";
            else operand->print(os);
        }

        bool equals(const Formula& other) const override {
            const Not* derived = dynamic_cast<const Not*>(&other);
            return derived && ((operand && derived->operand && operand->equals(*derived->operand)) || (!operand && !derived->operand));
        }

        Not* clone() const override{
            return new Not(operand? operand->clone() : nullptr);
        }

        void set_operand(Formula* new_oper){
            operand = new_oper;
        }

        Formula* get_operand(){
            return operand;
        }

        ~Not() {
            delete operand;
        }
    private:
        Formula* operand;
};

#endif