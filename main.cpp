// This module defines the entry point for the program. It
// instanciate a Game object and then runs a simple REPL 
// (read-eval-print-loop). Most commands are evaluted by calling a 
// corresponding method on the Game object.

#include <iostream>
#include <sstream>
#include <cassert>
#include <map>
#include "game.h"
#include "move.h"
#include "piece.h"

bool isFinished(Game &g) {
    return g.getAllLegalMoves().size() == 0;
}

// We need to parse a line, construct a Move, and make sure 
// the move is valid in the current Game.
// What we do instead is to get all the valid moves, and see
// if line is equal to the string representation of one of these moves. 
Move *parseAndValidate(Game &g, const std::string &line) {
    std::vector<Move *> moves = g.getAllLegalMoves();  
    for (auto x : moves) {
    std::string s = x->toBasicNotation();
    if (line == s) {
    return x;
    }
    }
    return NULL;
}

// Transforms a string s into a vector of words (substrings not containing 
// spaces)
void tokenize(const std::string &s, std::vector<std::string> &tokens) {
    std::istringstream f(s);
    std::string word;    
    while (f >> word) {
        tokens.push_back(word);
    }
}

// Asks the computer what next move to play. 
void computerPlay(Game &g, int strength) {
    if (isFinished(g)) {
       std::cout << "Nothing to play !" << std::endl;
       return; 
    }
    Move *m = g.computerSuggestion(strength);
    // should not be null as there is always something to play if the game is not
    // finished
    assert(m != NULL); 
    g.play(m);
    std::cout << "Computer played " << m->toBasicNotation() << std::endl; 
    g.display();
}

void evaluateCommand(Game &g, const std::string &line) {
        std::vector<std::string> commands;
        tokenize(line, commands);
        if (commands.size() == 0) {
            return;
        }
        std::string &command = commands[0];
        if (command == "d") {
            g.display();
        } else if (command == "?") {
            std::vector<Move *> moves = g.getAllLegalMoves();
            for (auto x : moves) {
                std::cout << x->toBasicNotation() << " ";
            }
            std::cout << std::endl; 
        } else if (command == "dead" || command == "e") {
            g.displayCaptured();
        } else if (command == "score" || command == "o") {
            g.displayValueHeuristic();
        } else if (command == "help" || command == "h") {
            std::cout << "*move*: play *move* (type '?' for list of possible moves)" << std::endl;
            std::cout << "play s, p s, p: computer plays next move, s = strength" << std::endl;
            std::cout << "display, d: display current state of the game" << std::endl;
            std::cout << "display dead pieces, e: display all eated piece during the game" << std::endl;
            std::cout << "display board score, o: display the current score on the board" << std::endl;
            std::cout << "undo, u: cancel last move" << std::endl;
            std::cout << "?: print all possible moves" << std::endl;
            std::cout << "quit, q: quit game" << std::endl;
            std::cout << "help, h: this message" << std::endl;
        } else if (command == "quit" || command == "q") {
            std::cout << "bye bye" << std::endl;
            exit(0);
        } else if (command == "undo" || command == "u") {
            bool b = g.undo();
	    if(b)
		std::cout << "undo last move" << std::endl;
	    else
		std::cout << "no move to undo" << std::endl;
        } else if (command == "play" || command == "p") {
            computerPlay(g, 1);
            return;
        } else {
            Move *m = parseAndValidate(g, line);
            if (m == NULL) {
               std::cout << "I didn't understand your move, try '?' for list of moves or 'help'" << std::endl; 
            } else {
               g.play(m);
               std::cout << m->toBasicNotation() << std::endl; 
               g.display();
            }
        }
    }

int main() { 
    Game g;
    std::string line;
    while(true) {
        std::cout << "> ";
        getline(std::cin, line);
        evaluateCommand(g, line); 
    }
    return 0;
}
