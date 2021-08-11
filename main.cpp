//
//  main.cpp
//  Zeckendorf
//
//  Created by Henry Fleischmann on 6/20/21.
//

#include <iostream>
#include <fstream>
#include <string>
#include "zeckGame.hpp"

using namespace std;

int playGameStrat() {
    auto G = ZeckGame(10000);
    cout << "Start index: " << G.start << "\n";
    G.playGame();
    cout << "Leftmost index: " << G.start - G.leftInGame << "\n";
    cout << "Rightmost index: " << G.rightInGame - G.start << "\n";
    cout << "Number of moves: " << G.moves << "\n";
    cout << "Number of Splits: " << G.numSplit << "\n";
    cout << "Number of Merges: " << G.numMerge << "\n";
    cout << "Decomposition: " << "\n";
    for (int i = G.leftMost; i <= G.rightMost; ++i){
        if (i == G.start){
            cout << "*";
        }
        cout << G.gameState[i];
        if (i == G.start){
            cout << "*";
        }
    }
    cout << "\n";
	return 0;
}

 void writeVec(std::vector<unsigned int> vec, ofstream& output) {
	if (vec.empty()) {
		output << "None" << '\n';
		return;
	}

	unsigned int sz = vec.size();

	for (unsigned int i = 0; i < sz - 1; i++) {
		output << vec[i];
		if ((i + 1) == sz) {
			output << '\n';
		} else {
			output << ',';
		}
	}
	output << vec.back() << '\n';
}

int slcr_split_at_zero(unsigned int n, string fileName) {
	ofstream outputFile;
	outputFile.open(fileName);
	auto G = ZeckGame(n);
	cout << "meow";
	G.playGame();
	cout << G.splitAtStart.front();
	writeVec(G.splitAtStart, outputFile);
	writeVec(G.splitAtOne, outputFile);
	writeVec(G.splitAtTwo, outputFile);
	writeVec(G.startHasZero, outputFile);
	writeVec(G.startHasNonZero, outputFile);

	outputFile.close();
	return 0;
}

int slcr_exhaust(unsigned int start, unsigned int upto, string fileName) {
	ofstream outputFile;
	outputFile.open(fileName);
	for (unsigned int n = start; n < (upto + 1); n++) {
		auto G = ZeckGame(n);
		G.playGame();
		cout << "Ran on: " << n;
		if (G.startExhausted.empty()) {
			outputFile << "None";
		} else {
			outputFile << G.startExhausted[0];
		}

		if (n == upto) {
			outputFile << '\n';
		} else {
			outputFile << ',';
		}
	}
	outputFile.close();
	return 0;
}

int slcr_smart_exhaust(unsigned int upto, string fileName) {
	ofstream outputFile;
	outputFile.open(fileName);

	auto G = ZeckGame(upto);
	G.wellUpto = upto;
	G.gameState[G.start] = 0;
	cout << G.wellUpto << '\n';
	G.playGameDumbSplit();
	cout << G.moves << '\n';

	writeVec(G.exhaustWell, outputFile);
	outputFile.close();
	return 0;
}

float averageLength(unsigned int trials, unsigned int n) {
	float result = 0;
	for (unsigned int i = 0; i < trials; i++) {
		auto G = ZeckGame(n);
		G.playGameRandomly();
		result += ((float) G.moves) / ((float) trials);
	}
	return result;
}

int collectRandomDataAvg(unsigned int trials, unsigned int upto, string fileName) {
    srand(time(NULL));
	ofstream outputFile;
	outputFile.open(fileName);

	for (unsigned int i = 1; i < upto + 1; i++) {
		cout << "Running on: " << i << "\n\n";
		float avgMoves = averageLength(trials, i);

		outputFile << avgMoves;
		if (i != upto) {
			outputFile << ",";
		}
	}

	return 0;
}

int collectRandomData(unsigned int trials, unsigned int n, string fileName) {
    srand(time(NULL));
	ofstream outputFile;
	outputFile.open(fileName);

	for (unsigned int i = 0; i < trials; i++) {
		cout << "Running trial: " << i << "\n";
    	auto G = ZeckGame(n);
		G.playGameRandomly();
		outputFile << G.moves;
		if (i != (trials - 1)) {
			outputFile << ",";
		}
	}

	return 0;
}

int collectRandomWeirdData(unsigned int trials, unsigned int n, unsigned int m, string fileName) {
    srand(time(NULL));
	ofstream outputFile;
	outputFile.open(fileName);

	for (unsigned int i = 0; i < trials; i++) {
		cout << "Running trial: " << i << "\n";
    	auto G = ZeckGame(n, m);
		G.playGameRandomlyWeirdSplits();
		outputFile << G.moves;
		if (i != (trials - 1)) {
			outputFile << ",";
		}
	}
	outputFile.close();

	return 0;
}

int main(int argc, const char * argv[]) {
 	string fileName;

	//int start;
//	int upto;
//	sscanf(argv[1], "%d", &start);
// 	sscanf(argv[1], "%d", &upto);
// 	fileName.assign(argv[2]);
// 	slcr_smart_exhaust(upto, fileName);
	int trials;
	int upto;
	sscanf(argv[1], "%d", &trials);
 	sscanf(argv[2], "%d", &upto);
 	fileName.assign(argv[3]);
	collectRandomDataAvg(trials, upto, fileName);
}
