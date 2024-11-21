#ifndef XOR_HPP
#define XOR_HPP

#include "Formula.hpp"
#include <iostream>
#include <utility>

class Xor : public Formula {
public:
    Xor(Formula* left, Formula* right)
        : left(std::move(left)), right(std::move(right)) {}

    void print(std::ostream& os) const override {
        os << "(";
        if (left == nullptr) os << " ";
        else left->print(os);
        os << " xor ";
        if (right == nullptr) os << " ";
        else right->print(os);
        os << ")";
    }

    bool equals(const Formula& other) const override {
        const Xor* derived = dynamic_cast<const Xor*>(&other);
        return derived && ((left && derived->left && left->equals(*derived->left)) || (!left && !derived->left))
            && ((right && derived->right && right->equals(*derived->right)) || (!right && !derived->right));
    }

    Xor* clone() const override {
        return new Xor(left ? left->clone() : nullptr, right ? right->clone() : nullptr);
    }

    void set_left(Formula* left_in) {
        left = left_in;
    }

    void set_right(Formula* right_in) {
        right = right_in;
    }

    Formula* get_left() {
        return left;
    }

    Formula* get_right() {
        return right;
    }

    ~Xor() override {
        delete left;
        delete right;
    }

private:
    Formula* left;
    Formula* right;
};

#endif