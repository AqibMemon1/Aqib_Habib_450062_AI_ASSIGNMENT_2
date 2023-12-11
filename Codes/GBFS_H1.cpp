#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

struct PuzzleState
{
    vector<vector<int>> board;
    int heuristic;
    int moves;
    vector<vector<vector<int>>> path;

    PuzzleState(vector<vector<int>> b,int h, int m) : board(b), heuristic(h), moves(m) {}
};

void printBoard(const vector<vector<int>>& board) {
    for (int i=0;i<3;i++) {
        for (int j=0;j<3;j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

int calculateHeuristic(const vector<vector<int>>& board, const vector<vector<int>>& goal) {
    int h1 = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int value = board[i][j];
            if (value != 0) {
                int targetX = (value - 1) / 3;
                int targetY = (value - 1) % 3;
                h1 += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return h1;
}

bool isBoardsEqual(const vector<vector<int>>& board1, const vector<vector<int>>& board2) {
    return board1 == board2;
}

// Function to convert a 2D vector to a string representation
string boardToString(const vector<vector<int>>& board) {
    string str;
    for (const auto& row : board) {
        for (int num : row) {
            str += to_string(num) + " ";
        }
        str += "\n";
    }
    return str;
}

void GBFS_h1(const vector<vector<int>>& initial, const vector<vector<int>>& goal) {
    auto comparator = [](const PuzzleState& a, const PuzzleState& b) {
        return a.heuristic > b.heuristic;
    };

    priority_queue<PuzzleState, vector<PuzzleState>, decltype(comparator)> pq(comparator);
    unordered_set<string> visited;

    int initialHeuristic = calculateHeuristic(initial, goal);
    pq.emplace(initial, initialHeuristic, 0);

    while (!pq.empty()) {
        PuzzleState current = pq.top();
        pq.pop();

        if (isBoardsEqual(current.board, goal)) {
            cout << "Goal State Found!" << endl;
            cout << "Number of moves: " << current.moves << endl;

            cout << "Path to the goal state:" << endl;
            for (const auto& pathBoard : current.path) {
                printBoard(pathBoard);
                cout << endl;
            }

            return;
        
        }

        // Convert the board to string representation for insertion
        string currentBoardString = boardToString(current.board);

        // Insert the string representation into the unordered_set
        visited.insert(currentBoardString);
     
   // Find the position of the empty space (0)
        int zeroX, zeroY;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (current.board[i][j] == 0) {
                    zeroX = i;
                    zeroY = j;
                    break;
                }
            }
        }

        // Define possible moves: up, down, left, right
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto& dir : directions) {
            int newX = zeroX + dir.first;
            int newY = zeroY + dir.second;

            if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
                vector<vector<int>> newBoard = current.board; // creates a vector newBoard with currentBoard values
                swap(newBoard[zeroX][zeroY], newBoard[newX][newY]); // swaps the value of zero
                string newBoardString = boardToString(newBoard);
                if (visited.find(newBoardString) == visited.end()) { // loop to check if the state has been visited or not
                    int newHeuristic = calculateHeuristic(newBoard, goal);
                    PuzzleState nextState(newBoard, newHeuristic, current.moves + 1);
                    nextState.path = current.path; // Copy path from the current state
                    nextState.path.push_back(newBoard); // Add new state to the path
                    pq.emplace(nextState);
                }
            }
        }
    }


    cout << "Goal State Not Reachable!" << endl;
}

int main() {
    vector<vector<int>> initial = {{8, 0, 6}, {5, 4, 7}, {2, 3, 1}};
    vector<vector<int>> goal = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

    cout << "Initial State:" << endl;
    printBoard(initial);
    cout << "\n";
    cout << "Goal State:" << endl;
    printBoard(goal);

    cout << "\nSolving the Puzzle...\n" << endl;
    GBFS_h1(initial, goal);


    return 0;
}
