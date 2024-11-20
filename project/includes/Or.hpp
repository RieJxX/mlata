#include "Formula.hpp"

#ifndef OR_HPP
#define OR_HPP

class Or : public Formula {
    public:
        Or(Formula* left, Formula* right)
            : left(std::move(left)), right(std::move(right)) {}

        void print(std::ostream& os) const override {
            os << "(";
            if (left == nullptr) os<<" ";
            else left->print(os);
            os << " | ";
            if (right == nullptr) os<<" ";
            else right->print(os);
            os << ")";
        }
        bool equals(const Formula& other) const override {
            const Or* derived = dynamic_cast<const Or*>(&other);
            return derived && ((left && derived->left && left->equals(*derived->left)) || (!left && !derived->left))
                && ((right && derived->right && right->equals(*derived->right)) || (!right && !derived->right));
        }

        Or* clone() const override{
            return new Or(left ? left->clone() : nullptr, right ? right->clone() : nullptr);
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

        ~Or() {
            delete left;
            delete right;
        }

    private:
        Formula* left;
        Formula* right;
    };


#endif