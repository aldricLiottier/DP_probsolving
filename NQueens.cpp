//
// Created by Utilisateur on 10/01/2022.
//

#include "NQueens.hpp"
#include <iostream>

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

int NQueens::ComputeOptimumGoal(State &state)
{
    int dangerCount = 0;

    for (int i = 0; i < _size; i++) {
        if (IsAttacked(state, i))
            dangerCount++;
    }

    return dangerCount;
}


void NQueens::GetNeighbor(State &nState)
{
    State optiState = nState;
    int optiGoal = ComputeOptimumGoal(optiState);
    State nbState = nState;

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            if (j != nState[i]) {
                nbState[i] = j;

                int tmp = ComputeOptimumGoal(nbState);

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
    // std::cout << "local strategy" << std::endl;
    State nState = _state;

    while (true) {
        // std::cout << "sup" << std::endl;
        _state = nState;

        GetNeighbor(nState);
        if (_state == nState)
            break; // END LOGIC
        else if (ComputeOptimumGoal(_state) == ComputeOptimumGoal(nState))
            nState[rand() % nState.size()] = rand() % nState.size();
    }
    // std::cout << "end of local strategy" << std::endl;
}

////////////////////////

/******** UNINFORMED SEARCH ********/

bool NQueens::SolveBoard(Board &board, int col)
{
    if (col >= _size)
        return true;

    for (int row = 0; row < _size; row++) {
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
    switch (strat) {
        case LOCAL:
            RandomizeState();
            LocalStrategy();
            break;
        case INFORMED:
            std::cout << "INFORMED" << std::endl;
            break;
        case UNINFORMED:
            UninformedStrategy();
            break;
    }
}

void NQueens::Reset(int newSize)
{
    _size = newSize;
    _state = State(_size, 0);
    _initState = _state;
}

void NQueens::PrintBoard() {
    for (int row = 0; row < _size; ++row) {
//        int queenLocation = _state[row];

        for (int col = 0; col < _size; ++col) {
            if (col == _state[row])
                std::cout << "Q ";
            else
                std::cout << "_ ";
        }
        std::cout << std::endl;
    }
}

// int NQueens::CountAttacks() {
//     int attackCount = 0;

//     for (int i = 0; i < _size; ++i) {
//         if (IsAttacked(i))
//             attackCount++;
//     }

//     return attackCount;
// }

bool NQueens::Equals(const NQueens &other) const
{
    if (this == &other)
        return true;

    return other.GetState() == _state;
}

State NQueens::GetState() const
{ return _state; }