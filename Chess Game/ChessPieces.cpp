/*
 * File: ChessPieces.cpp
 * Author: Omri Shalev
 * Date: September 16, 2023
 * Description: Implementation of chess piece classes.
 */

#include "ChessPieces.h"
#include "Classes.h"

// Forawad declarations 
class Board;
class Piece;

/* ---------------------------------- Pawn ----------------------------------  */
Pawn::Pawn(Colors color, Position pos) : Piece(color, Pieces::Pawn, pos) {}

string Pawn::getName() const {
    return "Pawn";
}

Pieces Pawn::getType() const {
    return Pieces::Pawn;
}

char Pawn::getSymbol() const {
    return (pieceColor == Colors::White) ? 'P' : 'p'; // U+2659 for white pawn, U+265F for black pawn
}

bool Pawn::isValidMove(Position start, Position end, const Board& board) const{
    // debug
    cout << "Checking move for " << getName()
        << " from (" << start.row << "," << start.col << ")"
        << " to (" << end.row << "," << end.col << ")" << endl;
    
    // Check if the destination is within the bounds of the board
    if (end.row < 0 || end.row >= 8 || end.col < 0 || end.col >= 8) {
        return false;
    }
    /* if Color = white so direction is to move forward on the board
       else if the color = black so move backward on the board. */
    int direction = (pieceColor == Colors::White) ? 1 : -1;
    // Forward move
    if (start.col == end.col) {
        if (start.row + direction == end.row) 
            return true; // Move one forward
        if (!hasMoved && start.row + 2 * direction == end.row) 
            return true; // First move, two forward
    }
    // Diagonal capture
    if (start.row + direction == end.row && abs(start.col - end.col) == 1) {
        if (board.isOpponentAt(end, pieceColor)) {
            return true;
        }
    }
    return false;
}


/* ---------------------------------- Rook ----------------------------------  */
Rook::Rook(Colors color, Position pos) : Piece(color, Pieces::Rook, pos) {}

string Rook::getName() const {
    return "Rook";
}

Pieces Rook::getType() const {
    return Pieces::Rook;
}

char Rook::getSymbol() const {
    return (pieceColor == Colors::White) ? 'R' : 'r'; // White Rook: ♖, Black Rook: ♜
}

bool Rook::isValidMove(Position start, Position end, const Board& board) const {
    // debug
    cout << "Checking move for " << getName()
        << " from (" << start.row << "," << start.col << ")"
        << " to (" << end.row << "," << end.col << ")" << endl;
    
    // Rooks can move either horizontally or vertically.
    if (start.row == end.row || start.col == end.col) {
        // Check if there are no pieces in the way
        int rowStep = (start.row == end.row) ? 0 : ((end.row > start.row) ? 1 : -1);
        int colStep = (start.col == end.col) ? 0 : ((end.col > start.col) ? 1 : -1);
       
        int currentRow = start.row + rowStep;
        int currentCol = start.col + colStep;

        while (currentRow != end.row || currentCol != end.col) {
            // Check if there's a piece at the current position
            Piece* pieceAtCurrent = board.getPieceAt({ currentRow, currentCol });

            if (pieceAtCurrent) {
                return false;
            }
            
            currentRow += rowStep;
            currentCol += colStep;
        }
        // No pieces in the way, so it's a valid non-capture move
        return true;
    }
    return false; // Rooks can only move horizontally or vertically
}

/* ---------------------------------- Knight ----------------------------------  */

Knight::Knight(Colors color, Position pos) : Piece(color, Pieces::Knight, pos) {}

string Knight::getName() const {
    return "Knight";
}

Pieces Knight::getType() const {
    return Pieces::Knight;
}

char Knight::getSymbol() const {
    return (pieceColor == Colors::White) ? 'N' : 'n'; // White Knight: ♘, Black Knight: ♞
}

bool Knight::isValidMove(Position start, Position end, const Board& board) const {
    // debug
    cout << "Checking move for " << getName()
        << " from (" << start.row << "," << start.col << ")"
        << " to (" << end.row << "," << end.col << ")" << endl;

    int dx = abs(start.row - end.row);
    int dy = abs(start.col - end.col);

    // Knights move in an "L" shape, so they can jump over other pieces.
    // Check if the move is valid for a knight.
    if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) {
        Piece* pieceAtEnd = board.getPieceAt(end);

        // If there's no piece at the destination or it's an opponent's piece, it's a valid move.
        if (!pieceAtEnd || pieceAtEnd->getColor() != pieceColor) {
            return true;
        }
    }
    return false; // Invalid move for a knight
}



/* ---------------------------------- Bishop ----------------------------------  */
Bishop::Bishop(Colors color, Position pos) : Piece(color, Pieces::Bishop, pos) {}

string Bishop::getName() const {
    return "Bishop";
}

Pieces Bishop::getType() const {
    return Pieces::Bishop;
}

char Bishop::getSymbol() const {
    return (pieceColor == Colors::White) ? 'B' : 'b'; // White Bishop: ♗, Black Bishop: ♝
}

bool Bishop::isValidMove(Position start, Position end, const Board& board) const {
    // debug
    cout << "Checking move for " << getName()
        << " from (" << start.row << "," << start.col << ")"
        << " to (" << end.row << "," << end.col << ")" << endl;
    
    // Check if the move is diagonal (both row and column movements have the same absolute value)
    int dx = abs(start.row - end.row);
    int dy = abs(start.col - end.col);

    if (dx != dy) {
        return false; // Not a diagonal move
    }

    // Determine the direction of movement (up-right, up-left, down-right, or down-left)
    int rowStep = (end.row > start.row) ? 1 : -1;
    int colStep = (end.col > start.col) ? 1 : -1;

    int currentRow = start.row + rowStep;
    int currentCol = start.col + colStep;

    while (currentRow != end.row || currentCol != end.col) {
        Piece* pieceAtCurrent = board.getPieceAt({ currentRow, currentCol });
        if (pieceAtCurrent) {
            return false;
        }

        currentRow += rowStep;
        currentCol += colStep;         
    }

    // Check for a piece at the destination
    Piece* pieceAtEnd = board.getPieceAt(end);
    if (pieceAtEnd && pieceAtEnd->getColor() == pieceColor) {
        return false; // Cannot capture own piece
    }

    // No pieces in the way, so it's a valid move
    return true;
}


/* ---------------------------------- Queen ----------------------------------  */
Queen::Queen(Colors color, Position pos) : Piece(color, Pieces::Queen, pos) {}

string Queen::getName() const {
    return "Queen";
}

Pieces Queen::getType() const {
    return Pieces::Queen;
}

char Queen::getSymbol() const {
    return (pieceColor == Colors::White) ? 'Q' : 'q'; // White Queen: ♕, Black Queen: ♛
}

//bool Queen::isValidMove(Position start, Position end, const Board& board) const {
//    int dx = abs(start.row - end.row);
//    int dy = abs(start.col - end.col);
//    if (start.row == end.row || start.col == end.col || dx == dy) {
//        // Determine the direction of movement
//        int rowStep = (end.row > start.row) ? 1 : (end.row < start.row) ? -1 : 0;
//        int colStep = (end.col > start.col) ? 1 : (end.col < start.col) ? -1 : 0;
//
//        int currentRow = start.row + rowStep;
//        int currentCol = start.col + colStep;
//
//        while (currentRow != end.row && currentCol != end.col) {
//            Piece* pieceAtCurrent = board.getPieceAt({ currentRow, currentCol });
//            if (pieceAtCurrent->getColor() == pieceColor) {
//                return false; // There's a piece blocking the Queen's path
//            }
//
//            currentRow += rowStep;
//            currentCol += colStep;
//        }
//    }
//    //// Check for a piece at the destination
//    //Piece* pieceAtEnd = board.getPieceAt(end);
//    //if (pieceAtEnd && pieceAtEnd->getColor() == pieceColor) {
//    //    return false; // Cannot capture own piece
//    //}
//
//    return true; // No pieces blocking the way, so it's a valid move
//}

bool Queen::isValidMove(Position start, Position end, const Board& board) const {
    // debug
    cout << "Checking move for " << getName()
        << " from (" << start.row << "," << start.col << ")"
        << " to (" << end.row << "," << end.col << ")" << endl;
    
    // Check for horizontal or vertical movement (like a Rook)
    if (start.row == end.row || start.col == end.col) {
        int rowStep = (start.row == end.row) ? 0 : ((end.row > start.row) ? 1 : -1);
        int colStep = (start.col == end.col) ? 0 : ((end.col > start.col) ? 1 : -1);

        int currentRow = start.row + rowStep;
        int currentCol = start.col + colStep;

        while (currentRow != end.row || currentCol != end.col) {
            if (board.getPieceAt({ currentRow, currentCol })) {
                return false; // There's a piece blocking the path.
            }

            currentRow += rowStep;
            currentCol += colStep;
        }
        // No pieces in the way, so it's a valid move
        Piece* pieceAtEnd = board.getPieceAt(end);
        if (pieceAtEnd && pieceAtEnd->getColor() == pieceColor) {
            return false;  // Trying to capture a piece of the same color
        }
        return true;
    }
    // Check for diagonal movement (like a Bishop)
    int dx = abs(start.row - end.row);
    int dy = abs(start.col - end.col);
    if (dx == dy) {
        int rowStep = (end.row > start.row) ? 1 : -1;
        int colStep = (end.col > start.col) ? 1 : -1;

        int currentRow = start.row + rowStep;
        int currentCol = start.col + colStep;

        while (currentRow != end.row && currentCol != end.col) {
            if (board.getPieceAt({ currentRow, currentCol })) {
                return false; // There's a piece blocking the path.
            }
            currentRow += rowStep;
            currentCol += colStep;
        }
        Piece* pieceAtEnd = board.getPieceAt(end);
        if (pieceAtEnd && pieceAtEnd->getColor() == pieceColor) {
            return false;  // Trying to capture a piece of the same color
        }
        return true; // Path is clear
    }

    // Not a valid Rook or Bishop move, hence invalid for a Queen.
    return false;
}

/* ---------------------------------- King ----------------------------------  */
King::King(Colors color, Position pos) : Piece(color, Pieces::King, pos) {}

string King::getName() const {
    return "King";
}

Pieces King::getType() const {
    return Pieces::King;
}

char King::getSymbol() const {
    return (pieceColor == Colors::White) ? 'K' : 'k'; // White King: ♔, Black King: ♚
}

bool King::isValidMove(Position start, Position end, const Board& board) const {
    cout << "Checking move for " << getName()
        << " from (" << start.row << "," << start.col << ")"
        << " to (" << end.row << "," << end.col << ")" << endl;

    int dx = abs(start.row - end.row);
    int dy = abs(start.col - end.col);

    // Check if the move is within one square in any direction
    if (dx <= 1 && dy <= 1) {
        Piece* pieceAtEnd = board.getPieceAt(end);
        if (pieceAtEnd && pieceAtEnd->getColor() == pieceColor) {
            return false; // Cannot capture own piece
        }
        return true;
    }

    // Check for castling
    if (dx == 0 && dy == 2) {
        // Assuming the board class has a method to validate castling.
        if (board.canCastle(start, end)) {
            return true;
        }
    }

    return false;
}







