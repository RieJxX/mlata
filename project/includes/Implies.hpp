#include "Formula.hpp"

#ifndef IMPLIES_HPP
#define IMPLIES_HPP

class Implies : public Formula {
    public:
        Implies(Formula* left, Formula* right)
            : left(std::move(left)), right(std::move(right)) {}

        void print(std::ostream& os) const override {
            os << "(";
            if (left == nullptr) os<<" ";
            else left->print(os);
            os << " > ";
            if (right == nullptr) os<<" ";
            else right->print(os);
            os << ")";
        }

        ~Implies() {
            delete left;
            delete right;
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

        bool equals(const Formula& other) const override {
            const Implies* derived = dynamic_cast<const Implies*>(&other);
            return derived && ((left && derived->left && left->equals(*derived->left)) || (!left && !derived->left))
                && ((right && derived->right && right->equals(*derived->right)) || (!right && !derived->right));
        }

        Implies* clone() const override{
            return new Implies(left ? left->clone() : nullptr, right ? right->clone() : nullptr);
        }

    private:
        Formula* left;
        Formula* right;
};

#endif
