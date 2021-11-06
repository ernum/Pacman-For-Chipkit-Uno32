 # Pacman For Chipkit Uno32

The purpose of this project is to develop the classic "Pacman" game. The objective of the game is to accumulate as many points as possible by eating dots, fruits, and ghosts (whenever the chance arises) as you play as the Pacman character. After the player has accumulated all the dots of a level, they advance to the next level in which the objective is increasingly harder to accomplish.

## Main Features

* 1 playable Pacman and 4 enemy AI ghosts.
* Functional menu system for start, level select, highscore.
* Highscore is saved with three letters inputted by the player. 
* Functional controls via the chipkits buttons.
* Classic Pacman game functionality.

## Run Commands
To run the game on your chipkit, use the following commands:

    . /opt/mcb32tools/environment
    make
    make install TTYDEV=/dev/ttyS4

## Technologies
    C 
    MCB32
