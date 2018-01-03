#include <thread>
#include <sys/time.h>
#include "Chess.hpp"

bool debug = false;

int whiteWin[8];
int blackWin[8];

void printBoard(const Board& board, const char *reason)
{
    std::string boardStr;
    board.toString(boardStr);
    printf("board %s:\n%s\n", reason, boardStr.c_str());
}

void printCheckMateBoard(const Board& board, Side sideWin, Side sideLose)
{
    std::string boardStr;
    board.toString(boardStr);
    std::string attacksStr;
    board.toStringAttacks(sideWin, attacksStr);
    printf("check mate board:\n%s\n"
           "attacks:\n%s\n",
           boardStr.c_str(), attacksStr.c_str());
    std::string movesStr;
    board.toStringMoves(movesStr);
    printf("game moves:\n%s\n\n", movesStr.c_str());
}

void playGame(int idx, int loops, int plays)
{
    for (int g = 0; g < loops; ++g) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        unsigned seed = (unsigned)tv.tv_usec;
        Board board(seed, White, true);
        for (int i = 0; i < plays; ++i) {
            bool checkMate; bool draw;
            board.move(checkMate, draw);
            if (debug && board.wasPromotion()) {
                printBoard(board, "promotion");
            }
            if (debug && board.wasCastle()) {
                printBoard(board, "castle");
            }
            if (debug && board.wasEnpassant()) {
                printBoard(board, "enpassant");
            }
            if (checkMate) {
                Turn turn = board.getTurn();
                if (turn == White) {
                    printCheckMateBoard(board, Black, White);
                    ++blackWin[idx];
                } else {
                    printCheckMateBoard(board, White, Black);
                    ++whiteWin[idx];
                } 
                break;
            } else if (draw) {
                break;
            }
        }
    }
}

int sumWins(int *wins, int winsLen)
{
    int sum = 0;
    for (int i = 0; i < winsLen; ++i) {
        sum += wins[i];
    }
    return sum;
}

int main(int argc, char *argv[])
{
    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);
    int loops = (argc > 2) ? atoi(argv[2]) : 100;
    int plays = (argc > 1) ? atoi(argv[1]) : 30;
    int numThreads = 4;

    std::thread thread1(playGame, 0, loops, plays);
    std::thread thread2(playGame, 1, loops, plays);
    std::thread thread3(playGame, 2, loops, plays);
    std::thread thread4(playGame, 3, loops, plays);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);
    unsigned long start = ((unsigned long)tv_start.tv_sec) * 1000 * 1000 + tv_start.tv_usec; 
    unsigned long end = ((unsigned long)tv_end.tv_sec) * 1000 * 1000 + tv_end.tv_usec; 
    printf("time for %d loops of %d plays is %lu in %d threads\n", loops, plays, end - start, numThreads);
    int whiteWins = sumWins(whiteWin, numThreads);
    int blackWins = sumWins(blackWin, numThreads);
    printf("whiteWin(%d) blackWin(%d) draw(%d)\n", whiteWins, blackWins, (1 * loops - (whiteWins + blackWins)));

    return 0;
}
