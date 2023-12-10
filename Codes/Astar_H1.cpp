#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

struct PuzzleState {
    vector<vector<int>> board;
    int heuristic;
    int moves;
    vector<vector<int>> path;

    PuzzleState(const vector<vector<int>>& b, int h, int m) : board(b), heuristic(h), moves(m) {}
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

void AStar_h1(const vector<vector<int>>& initial, const vector<vector<int>>& goal) {
    auto comparator = [](const PuzzleState& a, const PuzzleState& b) {
        return (a.moves + a.heuristic) > (b.moves + b.heuristic);
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
            return;
        }

        string currentBoardString = boardToString(current.board);
        visited.insert(currentBoardString);

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

        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto& dir : directions) {
            int newX = zeroX + dir.first;
            int newY = zeroY + dir.second;

            if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
                vector<vector<int>> newBoard = current.board;
                swap(newBoard[zeroX][zeroY], newBoard[newX][newY]);
                string newBoardString = boardToString(newBoard);
                if (visited.find(newBoardString) == visited.end()) {
                    int newHeuristic = calculateHeuristic(newBoard, goal);
                    PuzzleState nextState(newBoard, newHeuristic, current.moves + 1);
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

    cout << "\nSolving the Puzzle...\n" << endl;
    AStar_h1(initial, goal);

    return 0;
}