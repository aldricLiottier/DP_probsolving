//
// Created by Utilisateur on 10/01/2022.
//

#include "NQueens.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

/******** ATTACK METHODS ********/

bool NQueens::IsAttacked(const State &state, int row, int col)
{
    for (int i = 0; i < _size; i++) {
        if (i != row) {
            if (state[i] == col)
                return true;
            else if ((state[i] - col) == (row - i))
                return true;
            else if ((col - state[i]) == (row - i))
                return true;
        }
    }
    
    return false;
}

bool NQueens::IsAttacked(const State &state, int row)
{
    return IsAttacked(state, row, state[row]);
}

bool NQueens::IsBoardAttacked(const Board &board, int row, int col)
{
    for (int x = 0; x < _size; x++) {
        if (x == col)
            continue;
        if (board[row][x] == QUEEN)
            return true;
    }

    // left top
    for (int x = row, y = col; x >= 0 && y >= 0; x--, y--) {
        if (board[x][y] == QUEEN)
            return true;
    }
    // left bot
    for (int x = row, y = col; x < _size && y >= 0; x++, y--) {
        if (board[x][y] == QUEEN)
            return true;
    }

    // right top
    for (int x = row, y = col; x >= 0 && y < _size; x--, y++) {
        if (board[x][y] == QUEEN)
            return true;
    }
    // right bot
    for (int x = row, y = col; x < _size && y < _size; x++, y++) {
        if (board[x][y] == QUEEN)
            return true;
    }

    return false;
}

/******* LOCAL SEARCH ********/

int NQueens::ComputeOptimalGoal(State &state)
{
    int dangerCount = 0;

    for (int i = 0; i < _size; i++) {
        if (IsAttacked(state, i))
            dangerCount++;
    }

    return dangerCount;
}


void NQueens::SetNeighbor(State &nState)
{
    State optiState = nState;
    int optiGoal = ComputeOptimalGoal(optiState);
    State nbState = nState;

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            if (j != nState[i]) {
                nbState[i] = j;

                int tmp = ComputeOptimalGoal(nbState);

                if (tmp <= optiGoal) {
                    optiGoal = tmp;
                    optiState = nbState;
                }

                nbState[i] = nState[i];
            }
        }
    }
    nState = optiState;
}

void NQueens::LocalStrategy()
{
    State nState = _state;

    while (true) {
        _iterations++;
        if (_solvingMethod == 2) {
            *_grid = ConvertStateToBoard(_state);
            std::lock_guard<std::mutex> guard(_algoMutex);
        }

        _state = nState;

        SetNeighbor(nState);
        if (_state == nState)
            break; // END LOGIC
        else if (ComputeOptimalGoal(_state) == ComputeOptimalGoal(nState))
            nState[rand() % nState.size()] = rand() % nState.size();
    }
}

////////////////////////

/******** UNINFORMED SEARCH ********/

bool NQueens::SolveBoard(Board &board, int col)
{
    if (col >= _size)
        return true;

    for (int row = 0; row < _size; row++) {
        _iterations++;
        if (_solvingMethod == 2) {
            // State s = ConvertBoardToState(board);
            // (*_grid) = ConvertStateToBoard(s);
            *_grid = board;
            std::lock_guard<std::mutex> guard(_algoMutex);
        }

        if (IsBoardAttacked(board, row, col) == false) {
            board[row][col] = QUEEN;
            if (SolveBoard(board, col + 1))
                return true;
            board[row][col] = EMPTY;
        }
    }

    return false;
}

State NQueens::ConvertBoardToState(const Board &board)
{
    State res(_size, 0);

    for (int row = 0; row < _size; row++) {
        for (int col = 0; col < _size; col++) {
            if (board[row][col] == QUEEN) {
                res[row] = col;
                break;
            }
        }
    }

    return res;
}

Board NQueens::ConvertStateToBoard(const State &state)
{
    Board b(_size, State(_size, EMPTY));

    for (int y = 0; y < _size; y++) {
        for (int x = 0; x < _size; x++) {
            if (state[y] == x)
                b[y][x] = QUEEN;
        }
    }

    return b;
}

void NQueens::UninformedStrategy()
{
    Board board(_size, State(_size, EMPTY));

    SolveBoard(board, 0);

    _state = ConvertBoardToState(board);
}

////////////////////////

NQueens::NQueens(int size) : _size(size) {
    _state = State(size, 0);
    _initState = _state;
    srand(time(0));
    _iterations = 0;
}

void NQueens::RandomizeState()
{
    for (int i = 0; i < _size; i++) {
        _state[i] = rand() % _size;
    }
    _initState = _state;
}

void NQueens::Compute(enum NQueens::Strat strat)
{
    auto time0 = std::chrono::high_resolution_clock::now();

    switch (strat) {
        case LOCAL:
            RandomizeState();
            LocalStrategy();
            break;
        case INFORMED:
            std::cout << "INFORMED IS NOT IMPLEMENTED" << std::endl;
            break;
        case UNINFORMED:
            UninformedStrategy();
            break;
    }
    auto time1 = std::chrono::high_resolution_clock::now();
    _timer = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0).count();
}

void NQueens::Reset(int newSize)
{
     _state = _initState;
}

void NQueens::SetInputGrid(Board *grid)
{
    _grid = grid;
    _initGrid = *grid;

    _state = ConvertBoardToState(*_grid);
    _initState = _state;
}

void NQueens::threadAlgo(std::vector<std::vector<int>> *grid, int *solvingMethod, int *strat)
{
    _solvingMethod = *solvingMethod;
    SetInputGrid(grid);
    enum Strat algoStrat = (*strat == 0) ? INFORMED : (*strat == 1) ? UNINFORMED : LOCAL;

    Compute(algoStrat);

    *_grid = ConvertStateToBoard(_state);
}

void NQueens::PrintBoard() {
    for (int row = 0; row < _size; ++row) {

        for (int col = 0; col < _size; ++col) {
            if (col == _state[row])
                std::cout << "Q ";
            else
                std::cout << "_ ";
        }
        std::cout << std::endl;
    }
}

long long NQueens::GetIterations() const
{ return _iterations; }

long long NQueens::GetTime() const
{ return _timer; }

bool NQueens::Equals(const NQueens &other) const
{
    if (this == &other)
        return true;

    return other.GetState() == _state;
}

State NQueens::GetState() const
{ return _state; }