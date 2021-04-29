#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include "forest.h"
#include <opencv2/ml/ml.hpp>

//ilość danych treningowych
#define NUM_OF_TRAINING_DATA 400
//plik z danymi
#define DATA_FILE_NAME "boston_housing.txt"
//plik do zapisu wyników
#define WRITE_DATA_FILE "wyniki2.txt"

std::pair<std::vector<std::vector<float>>, std::vector<float>> getTrainingData() {
    std::ifstream data_file;
    float tmp;
    data_file.open(DATA_FILE_NAME);
    std::string s;
    std::vector<std::vector<float>> X;
    std::vector<float> x;
    std::vector<float> Y;

    for (int i = 0; i < NUM_OF_TRAINING_DATA; ++i) {
        for (int j = 0; j < D_SIZE; ++j) {
            data_file >> tmp;
            x.push_back(tmp);
        }
        data_file >> tmp;
        Y.push_back(tmp);
        X.push_back(x);
        x.clear();
        std::getline(data_file, s);
    }
    data_file.close();

    std::pair<std::vector<std::vector<float>>, std::vector<float>> training_data;
    training_data.first = X;
    training_data.second = Y;

    return training_data;
}


std::pair<std::vector<std::vector<float>>, std::vector<float>> testing_data() {
    std::ifstream data_file;
    data_file.open(DATA_FILE_NAME);
    std::vector<std::vector<float>> X;
    std::string s;
    std::vector<float> x;
    std::vector<float> Y;
    float tmp;
    for (int i = 0; i < NUM_OF_TRAINING_DATA * 2; i++)
        std::getline(data_file, s);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < D_SIZE; j++) {
            data_file >> tmp;
            x.push_back(tmp);
        }
        data_file >> tmp;
        Y.push_back(tmp);
        X.push_back(x);
        x.clear();
        std::getline(data_file, s);
    }
    data_file.close();

    std::pair<std::vector<std::vector<float>>, std::vector<float>> training_data;
    training_data.first = X;
    training_data.second = Y;
    return training_data;
}
//zapisywanie danych do pliku
void writeData(std::vector<float> Y, std::vector<float> Y1, std::vector<float> Y2) {
    std::fstream data_file;
    data_file.open(WRITE_DATA_FILE, std::ios::out);
    data_file << "Y:\tY1:\tbłąd1:\tY2:\tbłąd2:\n";
    data_file << std::fixed;
    data_file.precision(4);
    for(int i=0; i<Y.size(); ++i){
	data_file << Y[i] <<"\t" << Y1[i] <<"\t" << abs(Y1[i] - Y[i]) << "\t" << Y2[i] <<"\t" << abs(Y2[i] - Y[i]) << std::endl;
    }
    data_file.close();
    return;
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    //uzyskanie danych treningowych
    std::pair<std::vector<std::vector<float>>, std::vector<float>> U;
    U = getTrainingData();

    //stworzenie i trening lasu losowego
    Forest forest(U);
    //przekształcenie danych treningowych do odpowiedniego formatu dla lasu losowego openCV
    cv::Mat UX = cv::Mat(NUM_OF_TRAINING_DATA, D_SIZE, CV_32F);
    cv::Mat UY = cv::Mat(NUM_OF_TRAINING_DATA, 1, CV_32F);
    for(int y=0; y<NUM_OF_TRAINING_DATA; ++y){
	for(int x=0; x<D_SIZE; ++x){
	    UX.at<float>(cv::Point(x, y)) = U.first.at(y).at(x);
	}
	    UY.at<float>(cv::Point(0, y)) = U.second.at(y);
    }
    CvRTrees trees;
cv::Mat var_type = cv::Mat(D_SIZE+1, 1, CV_8U);
var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL));
    //trening lasu losowego openCV
    trees.train(UX, CV_ROW_SAMPLE, UY, cv::Mat(), cv::Mat(), var_type);
    //uzyskanie danych testowych
    std::pair<std::vector<std::vector<float>>, std::vector<float>> test;
    test = testing_data();
    std::vector<std::vector<float>> X = test.first;
    std::vector<float> Y = test.second;

    float max_error1 = 0;
    float error1 = 0;

    float max_error2 = 0;
    float error2 = 0;

    cv::Mat TX = cv::Mat(1, D_SIZE, CV_32F);
    std::vector<float> Y1, Y2;
    for (int i = 0; i < Y.size(); i++) {
        //uzyskanie wartości dla jednego ze zbiorów testowych z lasu losowego
        Y1.push_back(forest.predict(X[i]));
	//przetworzenie danych do odpowiedniego formatu
	for(int x=0; x<D_SIZE; ++x){
	    TX.at<float>(cv::Point(x, 0)) = X.at(i).at(x);
	}
	//uzyskanie wartości dla jednego ze zbiorów testowych z lasu losowego openCV
	Y2.push_back(trees.predict(TX));
	//korekcja maksymalnego błędu i obliczenie średniego błędu dla lasów losowych
        if(abs(Y1[i] - Y[i]) > max_error1)
            max_error1 = abs(Y1[i] - Y[i]);
        error1 += abs(Y1[i] - Y[i]);
        if(abs(Y2[i] - Y[i]) > max_error2)
            max_error2 = abs(Y2[i] - Y[i]);
        error2 += abs(Y2[i] - Y[i]);
    }
    //zapisuje dane do pliku
    writeData(Y, Y1, Y2);
    //wyświetlenie informacji końcowych: średni i maksymalny błąd
    std::cout << "Number of predictions: " << Y.size() << std::endl <<std::endl;
    std::cout << "Zaimplementowana wersja:" << std::endl;
    std::cout << "Average error: " << error1 / Y.size() << std::endl;
    std::cout << "Max error: " << max_error1 << std::endl;
    std::cout << std::endl;
    std::cout << "Wersja openCV:" << std::endl;
    std::cout << "Average error: " << error2 / Y.size() << std::endl;
    std::cout << "Max error: " << max_error2 << std::endl;
    return 0;
}
