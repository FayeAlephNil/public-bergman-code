//
//  zeckGame.cpp
//  Zeckendorf
//
//  Created by Henry Fleischmann on 6/20/21.
//

#include "zeckGame.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <math.h>
#include <assert.h>

using namespace std;

void ZeckGame::playGame(){
    // merge() || split () R L seems to be optimal. About n*phi moves.
    while (split() || merge()){
		if (gameState[start] == 0) {
			startHasZero.push_back(moves);
		} else if (!startHasZero.empty() && gameState[start] >= 1) {
			startHasNonZero.push_back(moves);
		}

	}
    return;
}

void ZeckGame::playGameDumbSplit(){
    // merge() || split () R L seems to be optimal. About n*phi moves.
    while (gameState[start] > -wellUpto && (dumbSplit() || merge())) {
		if (gameState[start] == 0) {
			startHasZero.push_back(moves);
		} else if (!startHasZero.empty() && gameState[start] >= 1) {
			startHasNonZero.push_back(moves);
		}
	}
    return;
}

void ZeckGame::playGameWeird(){
    // merge() || split () R L seems to be optimal. About n*phi moves.
    while (weirdSplit()){}
    return;
}

void ZeckGame::playGameRandomly(){
    // merge() || split () R L seems to be optimal. About n*phi moves.
    while (performRandomMove()){}
    return;
}

void ZeckGame::playGameRandomlySplits(){
    // merge() || split () R L seems to be optimal. About n*phi moves.
    while (performRandomMoveSplit()){}
    return;
}

void ZeckGame::playGameRandomlyWeirdSplits(){
    // merge() || split () R L seems to be optimal. About n*phi moves.
    while (performRandomWeirdSplit()){}
    return;
}

bool ZeckGame::split(){
//    for (int i = rightMost; i >= leftMost; --i){
    for (int i = leftMost; i <= rightMost; ++i){
		if (performSplit(i)) {
			if (i == start) {
				splitAtStart.push_back(moves);
				if (gameState[start] < 2) {
					startExhausted.push_back(moves);
				}
			} else if (i == start + 1) {
				splitAtOne.push_back(moves);
			} else if (i == start + 2) {
				splitAtTwo.push_back(moves);
			}
			return 1;
		}
    }
    return 0;
}


bool ZeckGame::dumbSplit(){
//    for (int i = rightMost; i >= leftMost; --i){
    for (int i = leftMost; i <= rightMost; ++i){
		if (performDumbSplit(i)) {
			if (i == start) {
				splitAtStart.push_back(moves);
			//	if (gameState[start] < 2) {
			//		startExhausted.push_back(moves);
			//	}
				exhaustWell.push_back(moves);
				exhaustWell.push_back(moves);
			} else if (i == start + 1) {
				splitAtOne.push_back(moves);
			} else if (i == start + 2) {
				splitAtTwo.push_back(moves);
			}
			return 1;
		}
    }
    return 0;
}

bool ZeckGame::performDumbSplit(int i) {
	if ((i - 2) >= 0 && ((gameState[i] >= 2) || (i == start))){
        ++gameState[i - 2];
        if (leftMost > i - 2){
            leftMost = i - 2;
            leftInGame = min(leftMost, leftInGame);
        }

		if (i < (start - 1)) {
        	++gameState[i + 1];
        	// can make more efficient here later;
        	if (rightMost < i + 1){
            	rightMost = i + 1;
            	rightInGame = max(rightMost, rightInGame);
        	}
		}
        gameState[i] -= 2;
        ++moves;
        ++numSplit;
        return 1;
    }
	return 0;
}

bool ZeckGame::weirdSplit() {
	WeirdSplit theSplit;

	if (gameState[start] >= 2 && start - 2 >= 0 && start + 2 < gameState.size()) {
		theSplit = WeirdSplit {.indexBack = start - 2, .index = start, .indexForward = start + 2};
	} else {
		for (int i = leftMost; i < start; ++i) {
			if (i - 2 >= 0 && gameState[i] >= 2) {
				theSplit = WeirdSplit {.indexBack = i - 2, .index = i, .indexForward = start};
				break;
			}
		}

		for (int j = start + 2; j < rightMost; ++j) {
			if (j + 1 < gameState.size() && gameState[j] >= 2) {
				theSplit = WeirdSplit {.indexBack = start, .index = j, .indexForward = j + 1};
				break;
			}
		}
	}

	//if (theSplit) {
		assert(performWeirdSplit(theSplit));
		return 1;
	//}
	return performRandomWeirdSplit();
}

bool ZeckGame::performSplit(int i) {
	if ((i - 2) >= 0 && gameState[i] >= 2){
        ++gameState[i - 2];
        if (leftMost > i - 2){
            leftMost = i - 2;
            leftInGame = min(leftMost, leftInGame);
        }
        ++gameState[i + 1];

        if (rightMost < i + 1){
            rightMost = i + 1;
            rightInGame = max(rightMost, rightInGame);
        }
        gameState[i] -= 2;
        ++moves;
        ++numSplit;
        return 1;
    }
	return 0;
}

bool ZeckGame::merge(){
    for (int i = rightMost; i >= leftMost; --i){
    // for (int i = rightMost; i >= leftMost; --i){
		if (performCombine(i)) {
			return 1;
		}
	// }
	}
    return 0;
}

bool ZeckGame::performCombine(int i) {
	if (i >= 0 && (gameState[i] >= 1) && (gameState[i+1] >= 1)){
    	++gameState[i + 2];
		if (rightMost < i + 2){
    		rightMost = i + 2;
        	rightInGame = max(rightMost, rightInGame);
       		}
       	--gameState[i];
       	--gameState[i+1];
       	if (leftMost == i && gameState[i] == 0){
        	leftMost = (gameState[i + 1] == 0) ? i + 2 : i + 1;
           	leftInGame = min(leftMost, leftInGame);
       	}
       	++moves;
       	++numMerge;
//      just to print out
//      for (int i = start/2; i <= start + ceil(log10(num)/log10(FIB)); ++i){
//          if (i == start){
//              cout << "*";
//          }
//          cout << gameState[i];
//          if (i == start){
//              cout << "*";
//          }
//      }
//      cout << "\n";
       	return 1;
   	}
	return 0;
}

bool ZeckGame::performMove(Move move) {
	if (move.type == MOVE_TYPE::SPLIT) {
		return performSplit(move.index);
	} else if (move.type == MOVE_TYPE::COMBINE) {
		return performCombine(move.index);
	} else {
		return 0;
	}
}


// want to implement "studyGame" which marks player turn and who eventually wins

std::tuple<std::vector<Move>, std::vector<Move>> ZeckGame::possibleMovesTuple() {
	std::vector<Move> theCombineMoves;
	std::vector<Move> theSplitMoves;
    for (int i = leftMost; i <= rightMost; ++i){
		if ((i - 2) >= 0 && (i + 1) < gameState.size() && gameState[i] >= 2){
			Move theSplit = {.type = MOVE_TYPE::SPLIT, .index=i};
			theSplitMoves.push_back(theSplit);
		}

		if (i >= 0 && (i + 2) < gameState.size() && gameState[i] >= 1 && gameState[i + 1] >= 1) {
			Move theCombine = {.type = MOVE_TYPE::COMBINE, .index=i};
			theCombineMoves.push_back(theCombine);
		}
	}
	return std::tuple<std::vector<Move>, std::vector<Move>>(theCombineMoves, theSplitMoves);
}

std::vector<Move> ZeckGame::possibleMoves() {
	std::tuple<std::vector<Move>, std::vector<Move>> tupleMoves = possibleMovesTuple();
	std::vector<Move> theMoves = std::get<0>(tupleMoves);
	std::vector<Move> theSplitMoves = std::get<1>(tupleMoves);

	theMoves.reserve(theMoves.size() + theSplitMoves.size());
	theMoves.insert(theMoves.end(), theSplitMoves.begin(), theSplitMoves.end());

	return theMoves;
}

bool ZeckGame::performRandomMove() {
	std::vector<Move> theMoves = possibleMoves();
	if (theMoves.size() == 0) {
		return 0;
	}
	int randomIdx = rand() % theMoves.size();

	assert(performMove(theMoves[randomIdx]));

	return 1;
}

bool ZeckGame::performRandomMoveSplit() {
	std::tuple<std::vector<Move>, std::vector<Move>> tupleMoves = possibleMovesTuple();
	std::vector<Move> theCombineMoves = std::get<0>(tupleMoves);
	std::vector<Move> theSplitMoves = std::get<1>(tupleMoves);

	if (theSplitMoves.size() == 0) {
		if (theCombineMoves.size() == 0) {
			return 0;
		}
		int randomIdx = rand() % theCombineMoves.size();
		assert(performMove(theCombineMoves[randomIdx]));

		return 1;
	} else {
		int randomIdx = rand() % theSplitMoves.size();

		assert(performMove(theSplitMoves[randomIdx]));
		return 1;
	}
}

std::vector<WeirdSplit> ZeckGame::possibleWeirdSplits() {
	std::vector<WeirdSplit> theSplits;
	for (int i = leftMost; i <= rightMost; ++i){
		if (gameState[i] >= 2) {
			for (int j = 0; j <= (i - 2); ++j) {
				for (int k = (i + 1); k < gameState.size(); ++k) {
					WeirdSplit theMove = {.indexBack = j, .index = i, .indexForward=k};
					theSplits.push_back(theMove);
					// Prioritize certain types of moves because God is Dead
					if (gameState[j] > 0) {
						theSplits.push_back(theMove);
						theSplits.push_back(theMove);
					}
					if (gameState[k] > 0) {
						theSplits.push_back(theMove);
						theSplits.push_back(theMove);
					}
				}
			}
		}
	}
	return theSplits;
}

bool ZeckGame::performWeirdSplit(WeirdSplit theSplit) {
	int idxBack = theSplit.indexBack;
	int idx = theSplit.index;
	int idxForward = theSplit.indexForward;

	if (idxBack >= 0 && gameState[idx] >= 2 && idxForward < gameState.size()){
        ++gameState[idxBack];
        if (leftMost > idxBack){
            leftMost = idxBack;
            leftInGame = min(leftMost, leftInGame);
        }
        ++gameState[idxForward];
        // can make more efficient here later;
        if (rightMost < idxForward){
            rightMost = idxForward;
            rightInGame = max(rightMost, rightInGame);
        }
        gameState[idx] -= 2;
        ++moves;
        ++numSplit;
        // just to print out
//      for (int i = start/2; i <= start + ceil(log10(num)/log10(FIB)); ++i){
//          if (i == start){
//              cout << "*";
//          }
//          cout << gameState[i];
//          if (i == start){
//              cout << "*";
//          }
//      }
//      cout << "\n";
        return 1;
    }
	return 0;
}

bool ZeckGame::performRandomWeirdSplit() {
	std::vector<WeirdSplit> theSplits = possibleWeirdSplits();
	if (theSplits.size() == 0) {
		return 0;
	}

	int randomIdx = rand() % theSplits.size();
	assert(performWeirdSplit(theSplits[randomIdx]));
	return 1;
}
