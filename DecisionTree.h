#include "Node.h"

class DecisionTree {

public:
    int buildTree(std::vector<int> D, std::vector<std::vector<float>> X, std::vector<float> Y);
    float predict(std::vector<float> X);

private:
    Node* root;
};
