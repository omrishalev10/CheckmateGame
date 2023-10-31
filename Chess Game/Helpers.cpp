/*
 * File: Helpers.cpp
 * Author: Omri Shalev
 * Date: September 16, 2023
 * Description: Implementation of helps methods for the chess game.
 */

#include "Helpers.h"
#include <sstream>
#include <algorithm> // Required for transform
#include <cctype>    // Required for tolower

Piece* parseMoveAndGetPiece(const string& moveInput, Colors currentPlayer, const Board& board, Position& startPosition, Position& endPosition) {
    // Parse the move input and identify the piece
    istringstream iss(moveInput);
    string startStr, to, endStr;

    if (iss >> startStr >> to >> endStr) {
        // Parse the start and end positions
        if (parsePosition(startStr, startPosition) && parsePosition(endStr, endPosition)) {
            // Get the piece at the start position
            Piece* piece = board.getPieceAt(startPosition);

            // if this Piece is mine, so return it
            if (piece && piece->getColor() == currentPlayer) {
                return piece;
            }
        }
    }
    return nullptr; // Invalid move or piece
}




// This function gets a position as a string and returns the position for the board. returns true if succeed, else false.
bool parsePosition(const string& positionStr, Position& position) {
    if (positionStr.length() != 2) {
        return false; // Invalid position format
    }

    char colChar = positionStr[0];
    char rowChar = positionStr[1];

    // Check if the column character is a valid lowercase letter (a-h)
    if (colChar < 'a' || colChar > 'h') {
        return false;
    }

    // Check if the row character is a valid digit (1-8)
    if (rowChar < '1' || rowChar > '8') {
        return false;
    }

    // Convert the column character to a numeric index (0-7)
    int col = colChar - 'a';

    // Convert the row character to a numeric index (0-7)
    int row = '8' - rowChar;

    position.row = row;
    position.col = col;

    return true;
}




