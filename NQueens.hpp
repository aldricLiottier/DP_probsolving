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

    // Logic
    void Compute(enum Strat strat);
    void threadAlgo(std::vector<std::vector<int>> *grid, int *solvingMethod, int *strat);

    // Local strat methods
    bool IsAttacked(const State &state, int row);
    bool IsAttacked(const State &state, int row, int column);
    int ComputeOptimalGoal(State &state);
    void SetNeighbor(State &nState);
    void LocalStrategy();

    // uninformed strat
    bool IsBoardAttacked(const Board &board, int row, int col);
    bool SolveBoard(Board &board, int col);
    void UninformedStrategy();

    // getters
    long long GetIterations() const;
    long long GetTime() const;
    State GetState() const;
    long long GetNodeSize() const;

    // Utility Methods
    void PrintBoard();
    State ConvertBoardToState(const Board &board);
    Board ConvertStateToBoard(const State &state);
    void RandomizeState();
    void SetInputGrid(std::vector<std::vector<int>> *grid);
    bool Equals(const NQueens &other) const;

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
