#include "forest.h"
#include <iostream>

Forest::Forest(std::pair<std::vector<std::vector<float>>, std::vector<float>> test) {
	//tworzenie poszczególnych drzew decyzyjnych z losowymi cechami i danymi treningowymi
	for(int i=0; i<NUM_OF_TREE; ++i){
		std::vector<int> D = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
		std::vector<std::vector<float>> X = test.first;
		std::vector<float> Y = test.second;
		std::vector<int> DTree;
		std::vector<std::vector<float>> XTree;
		std::vector<float> YTree;

		//losuje ilość cech dla danego drzewa
		int countD = rand()%D.size() + 1;
		for(int j=0; j<countD; ++j){
			//losuje kolejną cechę dla danego drzewa
			int count = rand()%D.size();
			DTree.push_back(D[count]);
			D.erase(D.begin()+count);
		}
		//losuje ilość danych treningowych
		int countTest = rand()%X.size() + 1;
		for(int j=0; j<countTest; ++j){
			//losuje konkretny zbiór danych
			int count = rand()%X.size();
			XTree.push_back(X[count]);
			YTree.push_back(Y[count]);
			X.erase(X.begin()+count);
			Y.erase(Y.begin()+count);
		}
		//zbudowanie i wytrenowanie drzewa decyzyjnego
		forest_[i].buildTree(DTree, XTree, YTree);
	}
}

float Forest::predict(std::vector<float> X) {
	float score = 0;
	//obliczanie średniej arytmetycznej
	for(int i=0; i<NUM_OF_TREE; ++i)
		score += forest_[i].predict(X);
	score = score/NUM_OF_TREE;
	return score;
}
