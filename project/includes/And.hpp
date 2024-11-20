#include "Formula.hpp"

#ifndef AND_HPP
#define AND_HPP

class And : public Formula {
    public:
        And(Formula* left, Formula* right)
            : left(std::move(left)), right(std::move(right)) {}

        void print(std::ostream& os) const override {
            os << "(";
            if (left == nullptr) os<<" ";
            else left->print(os);
            os << " * ";
            if (right == nullptr) os<<" ";
            else right->print(os);
            os << ")";
        }

        bool equals(const Formula& other) const override {
            const And* derived = dynamic_cast<const And*>(&other);
            return derived && ((left && derived->left && left->equals(*derived->left)) || (!left && !derived->left))
                && ((right && derived->right && right->equals(*derived->right)) || (!right && !derived->right));
        }

        And* clone() const override{
            return new And(left ? left->clone() : nullptr, right ? right->clone() : nullptr);
        }

        void set_left(Formula* left_in){
            left = left_in;
        }

        void set_right(Formula* right_in){
            right = right_in;
        }

        Formula* get_left(){
            return left;
        }

        Formula* get_right(){
            return right;
        }

        ~And() {
            delete left;
            delete right;
        }
        
    private:
        Formula* left;
        Formula* right;
};

#endif
