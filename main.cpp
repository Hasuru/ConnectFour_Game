#include <iostream>
#include <vector>
#include <unistd.h>

#define COL 7
#define ROW 6
#define LEN 42

#define P1 'X'
#define P2 'O'
#define EMPTY ' '

char winner = EMPTY;
int mode;
bool turn;
char current_player = EMPTY;

int choose_mode() {
    printf("MODE SELECTION:\n");

    printf("\t[1] -> PLAYER VS PLAYER\n");
    printf("\t[2] -> PLAYER VS AI\n");
    printf("\t[3] -> AI VS AI\n");

    printf("SELECT THE MODE: ");
    scanf("%d", &mode);
    return mode;
}

void choose_turn() {
    int temp;
    printf("CHOOSE WHO STARTS FIRST:\n");

    printf("\t[1] -> PLAYER 1 STARTS FIRST\n");
    printf("\t[2] -> PLAYER 2 STARTS FIRST\n");

    printf("SELECT TURN: ");
    scanf("%d", &temp);

    if (temp == 1)
        turn = true;
    else
        turn = false;
}

void show_board(std::vector<char> board) {
    for (int r = 0; r <= ROW; r++)
        printf("| %d ", r+1);
    printf("|\n");

    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) 
            printf("| %c ", board[r*COL+c]);
        printf("|\n");
    }
}

bool are_four_connected(std::vector<char> board, char player) {
    // check horizontally
    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL - 3; c++) {
            if ((board[r*COL+c] == board[r*COL+c+1]) && (board[r*COL+c] == board[r*COL+c+2]) && 
                (board[r*COL+c] == board[r*COL+c+3]) && (board[r*COL+c] == player)) {
                return true;
            }
        }
    }

    // check vertically
    for (int r = 0; r < ROW - 3; r++) {
        for (int c = 0; c < COL; c++) {
            if ((board[r*COL+c] == board[(r+1)*COL+c]) && (board[r*COL+c] == board[(r+2)*COL+c]) && 
                (board[r*COL+c] == board[(r+3)*COL+c]) && (board[r*COL+c] == player))
                return true;
        }
    }

    // check diagonally
    for (int r = 3; r < ROW; r++) {
        for (int c = 0; c < COL - 3; c++) {
            if ((board[r*COL+c] == board[(r-1)*COL+c+1]) && (board[r*COL+c] == board[(r-2)*COL+c+2]) && 
                (board[r*COL+c] == board[(r-3)*COL+c+3]) && (board[r*COL+c] == player))
                return true;
        }
    }

    return false;
}

bool game_ended(std::vector<char> board, std::vector<int> stacks) {
    if (are_four_connected(board, P1)) {
        winner = P1;
        return true;
    }

    if (are_four_connected(board, P2)) {
        winner = P2;
        return true;
    }
    
    // board is not empty
    for (int c = 0; c < COL; c++) {
        if (stacks[c] > 0)
            return false;
    }
    return true;
}

bool valid_play(std::vector<int> stacks, int col) {
    if (col >= COL || col < 0)
        return false;
    if (stacks[col] <= 0) 
        return false;
    return true;
}

int randomized_ai(std::vector<int> stacks) {
    int range = COL;
    int col = rand() % range + 1;

    while (col <= 0 || col > COL || stacks[col-1] <= 0) {
        col = rand() % range + 1;
    }

    return col;
}

int read_play(std::vector<int> stacks) {
    int col;

    if (turn) {
        current_player = P1;
        printf("PLAYER'S 1 TURN: ");
    } else {
        current_player = P2;
        printf("PLAYER'S 2 TURN: ");
    }

    switch (mode) {
        case 1:
            scanf("%d", &col);
            break;
        case 2:
            if (turn) {
                scanf("%d", &col);
            } else {
                col = randomized_ai(stacks);
                sleep(2);
            }   
            break;
        case 3:
            col = randomized_ai(stacks);
            sleep(2);
            break;
    }
    printf("COL SELECTED: %d\n", col);

    return col;
}

int main() {
    std::vector<char> board(LEN, EMPTY);
    std::vector<int> stacks(COL, ROW);

    choose_mode();
    putchar('\n');
    choose_turn();
    putchar('\n');

    int col;

    while (!game_ended(board, stacks)) {
        show_board(board);
        putchar('\n');
        col = read_play(stacks);

        col--;

        if (valid_play(stacks, col)) {
            board[(stacks[col]-1)*COL + col] = current_player;
            stacks[col]--;
        } else {
            printf("INVALID COLLUMN!\n");
        }

        putchar('\n');

        turn = !turn;
    }

    printf("FINAL RESULT:\n");
    show_board(board);

    switch (winner) {
        case P1:
            printf("PLAYER 1 WON!\n");
            break;
        case P2:
            printf("PLAYER 2 WON!\n");
            break;
        default:
            printf("GAME TIED!\n");
            break;
    }

    putchar('\n');

    return 0;
}