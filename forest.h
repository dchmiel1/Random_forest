#include <fstream>
#include <string>
#include <cstdlib>
#include <array>
#include "DecisionTree.h"

/**
*ilość drzew decyzyjnych w lesie losowym
*/
#define NUM_OF_TREE 200

class Forest {

public:
	/**
	* konstruktor tworzący las losowy używając podane dane jako dane treningowe, gdzie test.first to zbiór wejść, a test.second wynik
	*/
	Forest(std::pair<std::vector<std::vector<float>>, std::vector<float>> test);
	/**
	* metoda wylicza i zwraca wartość dla zbioru argumnetów wejściowych podanych jako argument,
	*
	* wynik jest średnią arytmetyczną cząstkowych wyników zwracanych przez drzewa decyzyjne
	*/
	float predict(std::vector<float> X);

private:
	/**
	* tablica trzymająca referencje na drzewa decyzyjne
	*/
	std::array<DecisionTree, NUM_OF_TREE> forest_;
};
