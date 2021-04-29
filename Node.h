#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_DEPTH 7
#define D_SIZE 13

class Node {

public:
    Node* left;
    Node* right;
    int d_indx;
    float d_value;
    bool is_leaf;
    float y;

    Node(std::vector<int> D, std::vector<std::vector<float>> X, std::vector<float> Y, int depth);

private:
    void findBestSplit(std::vector<int> D, std::vector<std::vector<float>> X, std::vector<float> Y);
    std::pair<float, float> getMinMSE(std::vector<float> d_data, std::vector<float> Y);
    float getMSE(std::vector<float> Y);
};
