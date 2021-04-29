#include "Node.h"
#include <iostream>

Node::Node(std::vector<int> D, std::vector<std::vector<float>> X, std::vector<float> Y, int depth) {
    if (Y.size() < 2 || getMSE(Y) < 1 || depth == MAX_DEPTH) {
        is_leaf = true;
        float mean = 0;
        for (int i = 0; i < Y.size(); i++)
            mean += Y[i];
        if (Y.size() > 0)
            y = mean / Y.size();
        else
            y = 0;
    }
    else {
        this->is_leaf = false;
        findBestSplit(D, X, Y);
        std::vector<std::vector<float>> left_X;
        std::vector<std::vector<float>> right_X;
        std::vector<float> left_Y;
        std::vector<float> right_Y;

        for (int i = 0; i < Y.size(); i++) {
            if (X[i][d_indx] < d_value) {
                left_X.push_back(X[i]);
                left_Y.push_back(Y[i]);
            }
            else {
                right_X.push_back(X[i]);
                right_Y.push_back(Y[i]);
            }
        }
        ++depth;
        this->left = new Node(D, left_X, left_Y, depth);
        this->right = new Node(D, right_X, right_Y, depth);
    }
}

//funkcja wyznacza najlepszy podział i zapisuje potrzebne informacje w strukturze węzła
void Node::findBestSplit(std::vector<int> D, std::vector<std::vector<float>> X, std::vector<float> Y) {
    std::vector<float> d_data;
    std::pair<float, float> d_mse;
    float min_mse = 999999;
    int min_mse_id = 0;
    float min_mse_value = 0;
    for (auto d : D) {
        for (int j = 0; j < X.size(); j++){
            d_data.push_back(X[j][d]);
        }
        d_mse = getMinMSE(d_data, Y);
        if (d_mse.first < min_mse) {
            min_mse = d_mse.first;
            min_mse_value = d_mse.second;
            min_mse_id = d;
        }
        d_data.clear();
    }
    this->d_indx = min_mse_id;
    this->d_value = min_mse_value;
}

//funkcja wyznaczająca najlepszy podział dla danego atrybutu
std::pair<float, float> Node::getMinMSE(std::vector<float> d_data, std::vector<float> Y) {
    std::vector<float> Y1_after_division;
    std::vector<float> Y2_after_division;
    std::vector<float> d_data_copy;
    d_data_copy = d_data;
    float min_mse = 999999;
    float min_mse_value = 0;
    float mean = 0;
    float tmp_mse = 0;
    std::sort(d_data_copy.begin(), d_data_copy.end());
    for (int i = 0; i < d_data_copy.size() - 1; i++) {
        if (d_data_copy[i] == d_data_copy[i + 1])
            continue;
        tmp_mse = 0;
        Y1_after_division.clear();
        Y2_after_division.clear();
        mean = (d_data_copy[i] + d_data_copy[i + 1]) / 2;
        for (int j = 0; j < d_data.size(); j++) {
            if (d_data[j] < mean)
                Y1_after_division.push_back(Y[j]);
            else
                Y2_after_division.push_back(Y[j]);
        }
        tmp_mse += Y1_after_division.size() * getMSE(Y1_after_division);
        tmp_mse += Y2_after_division.size() * getMSE(Y2_after_division);

        if (tmp_mse < min_mse) {
            min_mse = tmp_mse;
            min_mse_value = mean;
        }
    }

    std::pair<float, float> toReturn;
    toReturn.first = min_mse;
    toReturn.second = min_mse_value;
    return toReturn;
}

//zwraca obliczone MSE dla danego zbioru wartości
float Node::getMSE(std::vector<float> Y) {
    float mean = 0;
    float mse = 0;
    for (auto y : Y)
        mean += y;
    mean = mean / Y.size();
    for (auto y : Y)
        mse += (y - mean) * (y - mean);
    mse = mse / Y.size();
    return mse;
}
