#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

// ���������� ���������
const int BOARD_SIZE_8 = 3;
const int BOARD_SIZE_15 = 4;

// ��������� ������� �����
struct Board {
    vector<vector<int>> cells;
    int size;
};

// ��������������� ������� ��� ����������� ������� �����
void printBoard(const Board& board) {
    for (const auto& row : board.cells) {
        for (const auto& cell : row) {
            if (cell == 0) {
                cout << "   ";
            }
            else {
                cout << " " << cell << " ";
            }
        }
        cout << endl;
    }
}

// ��������������� ������� ��� ��������, �������� �� ������ ����������� �������� ��������
bool isSolvable(const vector<int>& tiles) {
    int inversions = 0;
    int size = tiles.size();
    for (int i = 0; i < size - 1; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (tiles[i] > tiles[j] && tiles[i] != 0 && tiles[j] != 0) {
                ++inversions;
            }
        }
    }
    if (size % 2 == 0) {
        int blankRow = size - 1 - (find(tiles.begin(), tiles.end(), 0) - tiles.begin()) / size;
        return (inversions + blankRow) % 2 == 0;
    }
    else {
        return inversions % 2 == 0;
    }
}

// ������� ��� �������� � ������������� ����� ������� �����
Board createBoard(int size) {
    Board board;
    board.size = size;
    board.cells.resize(size, vector<int>(size));

    vector<int> tiles(size * size);
    for (int i = 0; i < size * size; ++i) {
        tiles[i] = i;
    }

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(tiles.begin(), tiles.end(), default_random_engine(seed));

    while (!isSolvable(tiles)) {
        shuffle(tiles.begin(), tiles.end(), default_random_engine(seed));
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board.cells[i][j] = tiles[i * size + j];
        }
    }

    return board;
}

// ������� ��� ����������� �������� �� ������� �����
void moveTile(Board& board, int tile) {
    int size = board.size;
    int blankRow = -1, blankCol = -1;

    // ����� ������ ������ �� �����
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board.cells[i][j] == 0) {
                blankRow = i;
                blankCol = j;
                break;
            }
        }
        if (blankRow != -1) {
            break;
        }
    }

    // ��������, �������� �� ��������� �������� �������� � ������ ������
    if (blankRow != -1) {
        bool validMove = false;
        if (blankRow > 0 && board.cells[blankRow - 1][blankCol] == tile) {  // �����
            swap(board.cells[blankRow][blankCol], board.cells[blankRow - 1][blankCol]);
            validMove = true;
        }
        else if (blankRow < size - 1 && board.cells[blankRow + 1][blankCol] == tile) {  // ����
            swap(board.cells[blankRow][blankCol], board.cells[blankRow + 1][blankCol]);
            validMove = true;
        }
        else if (blankCol > 0 && board.cells[blankRow][blankCol - 1] == tile) {  // �����
            swap(board.cells[blankRow][blankCol], board.cells[blankRow][blankCol - 1]);
            validMove = true;
        }
        else if (blankCol < size - 1 && board.cells[blankRow][blankCol + 1] == tile) {  // ������
            swap(board.cells[blankRow][blankCol], board.cells[blankRow][blankCol + 1]);
            validMove = true;
        }

        if (!validMove) {
            cout << "��������� �������� �� ����� ���� ����������. ���������� �����." << endl;
        }
    }
}

// ������� ��� ��������, �������� �� ������� ����������� �������� ���������
bool isBoardSolved(const Board& board) {
    int size = board.size;
    int expectedValue = 1;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board.cells[i][j] != expectedValue && expectedValue != size * size) {
                return false;
            }
            expectedValue++;
        }
    }
    return true;
}

// ������� ��� ������� ����
void playGame(int mode) {
    int size = (mode == 1) ? BOARD_SIZE_8 : BOARD_SIZE_15;
    Board board = createBoard(size);

    cout << "���� \"��������\" ����������!" << endl;
    cout << "���� ���� - ����������� ����� �� 1 �� " << size * size - 1 << " �� �����������." << endl;
    cout << "������� ����� ��������, ����� �� �����������, ��� 0 ��� ������." << endl;

    int moves = 0;
    auto startTime = chrono::steady_clock::now();

    while (true) {
        cout << endl;
        printBoard(board);

        if (isBoardSolved(board)) {
            auto endTime = chrono::steady_clock::now();
            auto elapsedTime = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();

            cout << "�����������! �� ������ �����������!" << endl;
            cout << "���������� �����������: " << moves << endl;
            cout << "�����, ����������� �� ����: " << elapsedTime << " ������" << endl;
            break;
        }

        int tile;
        cout << "������� ����� �������� ��� ����������� (��� 0 ��� ������): ";
        cin >> tile;

        if (tile == 0) {
            cout << "���� ���������. �����..." << endl;
            break;
        }

        moveTile(board, tile);
        ++moves;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int mode;
    cout << "�������� ����� ����:" << endl;
    cout << "1. ����� 8 (3x3)" << endl;
    cout << "2. ����� 15 (4x4)" << endl;
    cout << "��� �����: ";
    cin >> mode;

    if (mode != 1 && mode != 2) {
        cout << "�������� ����� ����. ���������� ���������." << endl;
        return 0;
    }

    playGame(mode);

    return 0;
}