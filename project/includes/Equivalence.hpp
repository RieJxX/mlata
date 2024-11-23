#include "Formula.hpp"

#ifndef EQUIVALENCE_HPP
#define EQUIVALENCE_HPP

class Equivalence : public Formula {
public:
    Equivalence(Formula* left, Formula* right)
        : left(std::move(left)), right(std::move(right)) {}

    bool equals(const Formula& other) const override {
        const Equivalence* derived = dynamic_cast<const Equivalence*>(&other);
        return derived && ((left && derived->left && left->equals(*derived->left)) || (!left && !derived->left))
            && ((right && derived->right && right->equals(*derived->right)) || (!right && !derived->right));
    }

    Equivalence* clone() const override {
        return new Equivalence(left ? left->clone() : nullptr, right ? right->clone() : nullptr);
    }

    void set_left(Formula* left_in) { left = left_in; }
    void set_right(Formula* right_in) { right = right_in; }

    Formula* get_left() { return left; }
    Formula* get_right() { return right; }

    void print(std::ostream& os) const override {
        os << "(";
        if (left) left->print(os);
        os << "=";
        if (right) right->print(os);
        os << ")";
    }

    ~Equivalence() {
            delete left;
            delete right;
        }

private:
    Formula* left;
    Formula* right;
};

#endif