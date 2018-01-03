# chess
A header file only implementation of a chess game class.

# design
This class was created to play a random chess game and for re-enforcement learning of a Monte Carlo Tree Search.
This class is 100% re-entrant and can be used in parallel in many threads.  See tst/main.cpp as an example.

# building the tst code
cd tst

g++ -Wall -I../src --std=c++11 main.cpp -o chess

# running the tst code
to run chess games for a total of 80 moves and 1000 games in each thread

./chess 80 1000 > log.out

vim log.out 

