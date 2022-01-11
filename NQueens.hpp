//
// Created by Utilisateur on 10/01/2022.
//

#ifndef DP_PROBSOLVING_NQUEENS_HPP
#define DP_PROBSOLVING_NQUEENS_HPP

#include <vector>

typedef std::vector<std::vector<int>> Board;
typedef std::vector<int> State;

class NQueens
{
public:
    enum Strat
    {
        INFORMED,
        UNINFORMED,
        LOCAL
    };

    enum Tile : int
    {
        EMPTY,
        QUEEN
    };
    

    NQueens(int size);
    
    void Reset(int newSize);
    void PrintBoard();
    
    int CountAttacks();
    bool IsAttacked(const State &state, int row);
    bool IsAttacked(const State &state, int row, int column);
    
    bool Equals(const NQueens &other) const;
    
    void RandomizeState();
    State GetState() const;
    void Compute(enum Strat strat);

    // Local strat methods
    void LocalStrategy();
    void GetNeighbor(State &nState);
    int ComputeOptimumGoal(State &state);

    // uninformed strat
    bool IsBoardAttacked(const Board &board, int row, int col);
    void UninformedStrategy();
    bool SolveBoard(Board &board, int col);
    State ConvertBoardToState(const Board &board);

private:
    int _size;
    State _state;

    State _initState;
//    Board _board;
};

#endif //DP_PROBSOLVING_NQUEENS_HPP
