//
//  zeckGame.hpp
//  Zeckendorf
//
//  Created by Henry Fleischmann on 6/20/21.
//

#ifndef zeckGame_hpp
#define zeckGame_hpp

#include <stdio.h>
#include <deque>
#include <vector>
#include <cmath>
#include <math.h>

constexpr double FIB = 1.618;
enum class MOVE_TYPE : short {
	SPLIT,
	COMBINE,
	BAD_MOVE
};

struct WeirdSplit {
	int indexBack;
	int index;
	int indexForward;
};

struct Move {
	MOVE_TYPE type;
	int index;
};

class ZeckGame{
public:
    std::vector<int> gameState;
    unsigned int num;
    int leftInGame;
    int rightInGame;
    int leftMost;
    int rightMost;
    int start;
    unsigned int moves;
    unsigned int numSplit;
    unsigned int numMerge;
	std::vector<unsigned int> startExhausted;
	std::vector<unsigned int> exhaustWell;
	int wellUpto;

	std::vector<unsigned int> splitAtStart;
	std::vector<unsigned int> splitAtOne;
	std::vector<unsigned int> splitAtTwo;
	std::vector<unsigned int> startHasZero;
	std::vector<unsigned int> startHasNonZero;

	bool weirdSplit();

    // applies first split possible and returns true if split, returns false otherwise
    bool split();
    bool dumbSplit();

    // applies first split possible and returns true if split, returns false otherwise (goes right)
    bool splitR();

    // applies first merge possible and returns true if split, returns false otherwise
    bool merge();
    ZeckGame(int n) : gameState(4*n + 2* ceil(log10(n)/log10(FIB) + 10), 0), num(n), leftInGame(4*n), rightInGame(4*n), leftMost(4*n), rightMost(4*n), start(4*n), moves(0), numSplit(0), numMerge(0) {
        gameState[start] = n;
    }
    ZeckGame(int n, int m) : gameState(2*m + 1, 0), num(n), leftInGame(m), rightInGame(m), leftMost(m), rightMost(m), start(m), moves(0), numSplit(0), numMerge(0) {
        gameState[start] = n;
    }
    // play game, returns how far to the left it goes.
    void playGame();
    void playGameDumbSplit();

	bool performCombine(int i);
	bool performSplit(int i);
	bool performDumbSplit(int i);

	// Performs the move if possible, returns false if the move is not possible.
	bool performMove(Move move);
	bool performWeirdSplit(WeirdSplit theSplit);

	bool performRandomMove();
	bool performRandomMoveSplit();
	bool performRandomWeirdSplit();

	std::vector<WeirdSplit> possibleWeirdSplits();
	std::vector<Move> possibleMoves();
	std::tuple<std::vector<Move>, std::vector<Move>> possibleMovesTuple();

	void playGameWeird();
	void playGameRandomly();
	void playGameRandomlySplits();
	void playGameRandomlyWeirdSplits();
};


#endif /* zeckGame_hpp */
