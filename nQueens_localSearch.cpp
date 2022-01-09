#include <vector>
#include <iostream>
#include <algorithm>

#define ERROR 84

typedef std::vector<std::vector<int>> Board;

void printBoard(Board board)
{
    std::for_each(begin(board), end(board), [] (const auto &line) {
        std::for_each(begin(line), end(line), [] (const auto &c) {
            std::cout << c << " ";
        });
        std::cout << std::endl;
    });
}

Board createBoard(int size, int value = 0)
{
    std::vector<int> line(size, value);
    Board board(size, line);

    return board;
}

Board generateBoardFromState(std::vector<int> nState)
{
    int size = nState.size();
    Board nBoard = createBoard(size);

    for (int i = 0; i < size; ++i) {
        nBoard[nState[i]][i] = 1;
    }

    return nBoard;
}

int computeOptimumGoal(Board &oBoard, std::vector<int> &oState)
{
    int dangerCount = 0;
    int row, col;
    int size = oBoard.size();

    for (int i = 0; i < size; ++i) {
        row = oState[i];
        col = i - 1;

        // check left position
        while (col >= 0 && oBoard[row][col] != 1)
            col--;
        if (col >= 0 && oBoard[row][col] == 1) // couldn't find a safe spot while parsing going Left
            dangerCount++;

        row = oState[i];
        col = i + 1;
        // check right position
        while (col < size && oBoard[row][col] != 1)
            col++;
        // couldn't find a safe spot checking right side
        if (col < size && oBoard[row][col] == 1)
            dangerCount++;

        row = oState[i] - 1;
        col = i - 1;
        // Diag left up
        while (col >= 0 && row >= 0 && oBoard[row][col] != 1) {
            col--;
            row--;
        }
        if (col >= 0 && row >= 0 && oBoard[row][col] == 1)
            dangerCount++;

        row = oState[i] + 1;
        col = i - 1;
        // Diag left down
        while (col >= 0 && row < size && oBoard[row][col] != 1) {
            col--;
            row++;
        }
        if (col >= 0 && row < size && oBoard[row][col] == 1)
            dangerCount++;


        row = oState[i] - 1;
        col = i + 1;
        // Diag right up
        while (col < size && row >= 0 && oBoard[row][col] != 1) {
            col++;
            row--;
        }
        if (col < size && row >= 0 && oBoard[row][col] == 1)
            dangerCount++;


        row = oState[i] + 1;
        col = i + 1;
        // Diag right down
        while (col < size && row < size && oBoard[row][col] != 1) {
            col++;
            row++;
        }
        if (col < size && row < size && oBoard[row][col] == 1)
            dangerCount++;
    }

    return (int)(dangerCount / 2);
}

void getNeighbor(Board &nBoard, std::vector<int> &nState)
{
    int size = nBoard.size();
    std::vector<int> optiState = nState;
    Board optiBoard = generateBoardFromState(optiState);
    int optiGoal = computeOptimumGoal(optiBoard, optiState);

    std::vector<int> nbState = nState;
    Board nbBoard = generateBoardFromState(nbState);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {

            if (j != nState[i]) {
                nbState[i] = j;
                nbBoard[nbState[i]][i] = 1;
                nbBoard[nState[i]][i] = 0;

                int tmp = computeOptimumGoal(nbBoard, nbState);

                if (tmp <= optiGoal) {
                    optiGoal = tmp;
                    optiState = nbState;
                    optiBoard = generateBoardFromState(optiState);
                }

                // reset to get to the start config
                nbBoard[nbState[i]][i] = 0;
                nbState[i] = nState[i];
                nbBoard[nState[i]][i] = 1;
            }
        }
    }

    nState = optiState;
    nBoard = generateBoardFromState(nState);
}

void solveNQueens(Board &board, std::vector<int> state)
{
    std::vector<int> nState = state;
    Board nBoard = generateBoardFromState(nState);

    while (true) {
        state = nState;
        board = generateBoardFromState(state);

        getNeighbor(nBoard, nState);
        if (state == nState) {
            break; // END LOGIC
        } else if (computeOptimumGoal(board, state) == computeOptimumGoal(nBoard, nState)) {
            nState[rand() % nState.size()] = rand() % nState.size();
            nBoard = generateBoardFromState(nState);
        }
    }
}

void randomizeBoard(Board &board, std::vector<int> &state)
{
    int size = board.size();

    srand(time(0));

    for (int i = 0; i < size; i++) {
        state[i] = rand() % size;

        board[state[i]][i] = 1;
    }
}

int main(int ac, char **av)
{
    int size = 0;
    if (ac != 2)
        return ERROR;

    // check if the argument is valid
    std::string s_arg(av[1]);
    try {
        size = std::stoi(s_arg, nullptr);
    } catch (std::exception e) {
        std::cerr << "Error" << std::endl;
        return 84;
    }

    if (size == 2 || size == 3) {
        std::cerr << "Invalid size" << std::endl;
        return ERROR;
    }

    // create an empty board & state
    Board board = createBoard(size);
    std::vector<int> state(size, 0);
    
    // gives a random start state to the board
    randomizeBoard(board, state);

    // solve NQueens and start the hill climbing algo
    solveNQueens(board, state);
    
    //display the result with queens on the board
    printBoard(board);

    return 0;
}
