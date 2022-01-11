//
// Created by Utilisateur on 10/01/2022.
//

#ifndef DP_PROBSOLVING_NQUEENS_HPP
#define DP_PROBSOLVING_NQUEENS_HPP

#include <vector>
#include <mutex>
#include <thread>

typedef std::vector<std::vector<int>> Board;
typedef std::vector<int> State;

class NQueens
{
public:

    struct Node
    {
        std::vector<int> state;
        int score;
        Node *parent;
        Node *next;
    };

    enum Strat : int
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
    
    void threadAlgo(std::vector<std::vector<int>> *grid, int *solvingMethod, int *strat);

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
    void SetNeighbor(State &nState);
    int ComputeOptimalGoal(State &state);

    // uninformed strat
    bool IsBoardAttacked(const Board &board, int row, int col);
    void UninformedStrategy();
    bool SolveBoard(Board &board, int col);
    State ConvertBoardToState(const Board &board);
    Board ConvertStateToBoard(const State &state);
    void SetInputGrid(std::vector<std::vector<int>> *grid);

    long long GetIterations() const;
    long long GetTime() const;

private:
    int _solvingMethod;
    int _size;

    long long _iterations;
    long long _timer;

    State _state;
    State _initState;

    Board *_grid;
    Board _initGrid;

    std::mutex _algoMutex;
};

#endif //DP_PROBSOLVING_NQUEENS_HPP
