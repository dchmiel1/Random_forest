#include "DecisionTree.h"
#include <iostream>

int DecisionTree::buildTree(std::vector<int> D, std::vector<std::vector<float>> X, std::vector<float> Y) {
    if (D.size() == 0 || X.size() != Y.size()) {
        std::cout << "Wrong data provided" << std::endl;
        return -1;
    }
    this->root = new Node(D, X, Y, 0);
    return 0;
}

float DecisionTree::predict(std::vector<float> X) {
    if (X.size() != D_SIZE) {
        std::cout << "Wrong data provided";
        return -1;
    }
    Node* tmp = root;
    while (tmp->is_leaf == false) {
        if (X[tmp->d_indx] < tmp->d_value)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    return tmp->y;
}
