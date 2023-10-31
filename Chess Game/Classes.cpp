/*
 * File: Classes.cpp
 * Author: Omri Shalev
 * Date: September 16, 2023
 * Description: Implementation of class methods for the chess game.
 */

#include "Classes.h"
#include "ChessPieces.h"
#include <iostream>
#include <cassert>

// Constructor of Piece classs
Piece::Piece(Colors color, Pieces name) : pieceColor(color), pieceName(name), hasMoved(false) {}
Piece::Piece(Colors color, Pieces name, Position pos) : pieceColor(color), pieceName(name), position(pos), hasMoved(false) {}

// Implemetation of the == operator
bool Position::operator==(const Position& other) const {
	return row == other.row && col == other.col;
}

// Implemetation of the != operator
bool Position::operator!=(const Position& other) const {
	return !(*this == other);
}

// Get the detail about a Piece if it made its first move
bool Piece::getHasMoved() {
	return Piece::hasMoved;
}

// Set that the Piece made its first move
void Piece::setHasMoved(bool moved) {
	hasMoved = moved;
}

// Get the color of the Piece
Colors Piece::getColor() const {
	return pieceColor;
}

// Define the operator<< overload for Colors
ostream& operator<<(ostream& os, const Colors& color) {
	switch (color) {
	case Colors::Empty: return os << "Empty";
	case Colors::Black: return os << "Black";
	case Colors::White: return os << "White";
	default: return os << "Unknown";
	}
}

//Get the position of the Piece on the board
Position Piece::getPosition() const {
	return position;
}

void Piece::setPosition(const Position& newPosition) {
    position = newPosition;
}

// Constructor - Initialize the board with random color assignments
Board::Board() {
    
    // Initialize the board with nullptrs for empty spots
    board = new Piece * *[8];
    for (int i = 0; i < 8; i++) {
        board[i] = new Piece * [8];
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }

    // Set the white color to start at the bottom
    Colors rowColors[2] = { Colors::Black, Colors::White };

    // Setting up pawns and other pieces based on rowColors
    for (int i = 0; i < 8; i++) {
        board[1][i] = new Pawn(rowColors[1], { 1, i });
        board[6][i] = new Pawn(rowColors[0], { 6, i });
    }

    // Setting up Rooks
    board[0][0] = new Rook(rowColors[1], { 0, 0 });
    board[0][7] = new Rook(rowColors[1], { 0, 7 });
       
    board[7][0] = new Rook(rowColors[0], { 7, 0 });
    board[7][7] = new Rook(rowColors[0], { 7, 7 });

    // Setting up Knights
    board[0][1] = new Knight(rowColors[1], { 0, 1 });
    board[0][6] = new Knight(rowColors[1], { 0, 6 });
    
    board[7][1] = new Knight(rowColors[0], { 7, 1 });
    board[7][6] = new Knight(rowColors[0], { 7, 6 });

    // Setting up Bishops 
    board[0][2] = new Bishop(rowColors[1], { 0, 2 });
    board[0][5] = new Bishop(rowColors[1], { 0, 5 });

    board[7][2] = new Bishop(rowColors[0], { 7, 2 });
    board[7][5] = new Bishop(rowColors[0], { 7, 5 });

    // Setting up Queens
    board[0][3] = new Queen(rowColors[1], { 0, 3 });

    board[7][3] = new Queen(rowColors[0], { 7, 3 });

    // Setting up Kings
    board[0][4] = new King(rowColors[1], { 0, 4 });
    board[7][4] = new King(rowColors[0], { 7, 4 });
    whiteKingPosition = { 0, 4 };
    blackKingPosition = { 7, 4 };

}

// Destructor
Board::~Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j];
            board[i][j] = nullptr;  // Set pointer to nullptr
        }
        delete[] board[i];
        board[i] = nullptr;  // Set pointer to nullptr
    }
    delete[] board;  // Deallocate the primary board array
}


// Print the board to the console
void Board::printBoard() const {
    // Define an array to store the labels for rows (1 to 8)
    char rowLabels[8] = { '8', '7', '6', '5', '4', '3', '2', '1' };

    for (int i = 7; i >= 0; i--) { // Start from 7 and go to 0
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) { // Check if not nullptr
                cout << "[" << board[i][j]->getSymbol() << ", " << static_cast<char>('a' + j) << rowLabels[i] << "] ";
            }
            else {
                cout << "[   " << static_cast<char>('a' + j) << rowLabels[i] << "] ";
            }
        }
        cout << endl; // Row label (again)
    }
}


// Get the Piece at the position - pos if exist 
Piece* Board::getPieceAt(Position pos) const {
	if (pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8) {
		return board[pos.row][pos.col];
	}
	return nullptr;  // Position is out of bounds
}

// Place a piece at a specific location without checking anything
void Board::placePieceAt(const Position& position, Piece* piece) {
    board[position.row][position.col] = piece;
}

// Check if there is an opponent on the position pos
bool Board::isOpponentAt(Position pos, Colors color) const {
	Piece* pieceAtPos = getPieceAt(pos);
	if (pieceAtPos == nullptr) {
		return false; // No piece at the given position
	}
	Colors pieceColor = pieceAtPos->getColor();
	// Return true if the colors are different and neither is Colors::Empty
	return pieceColor != color && pieceColor != Colors::Empty;
}

bool Board::movePiece(const Position& start, const Position& end, bool isSimulation) {
    Piece* piece = getPieceAt(start);
    if (!piece)
        return false; // If there's no piece at the start position, can't move.

    // If the move is valid
    if (piece->isValidMove(start, end, *this)) {
        // Handle castling move
        if (piece->getType() == Pieces::King && abs(start.col - end.col) == 2) { // try to move the king 2 steps - castling
            if (end.col == 6) { // kingside
                Piece* kingsideRook = getPieceAt({ start.row, 7 });
                board[start.row][5] = kingsideRook;  // Move rook to f1/f8
                board[start.row][7] = nullptr;       // Clear original rook position
                kingsideRook->setPosition({ start.row, 5 });
            }
            else { // queenside
                Piece* queensideRook = getPieceAt({ start.row, 0 });
                board[start.row][3] = queensideRook;  // Move rook to d1/d8
                board[start.row][0] = nullptr;        // Clear original rook position
                queensideRook->setPosition({ start.row, 3 });
            }
        }

        Piece* destinationPiece = getPieceAt(end);
        if (destinationPiece && !isSimulation) {
            delete destinationPiece;  // Ensure any captured piece is deleted only if it's not a simulation.
            movesWithoutPawnOrCapture = 0;
        }
        else if (typeid(*piece) == typeid(Pawn)) {
            movesWithoutPawnOrCapture = 0;
        }
        else {
            movesWithoutPawnOrCapture++;
        }

        // Execute the move
        board[end.row][end.col] = piece;
        board[start.row][start.col] = nullptr;
        piece->setPosition(end);

        // Update king position if a king is moved
        if (piece->getType() == Pieces::King) {
            if (piece->getColor() == Colors::White) {
                whiteKingPosition = end;
            }
            else {
                blackKingPosition = end;
            }
        }

        // Update the first move of the piece
        if (!piece->getHasMoved()) {
            piece->setHasMoved(true);
        }

        return true;
    }
    return false;
}



// Check if a king of a kingColor is in check
bool Board::isInCheck(Colors kingColor) const {
    // Find the position of the king of the specified color
    Position kingPosition;

    if (kingColor == Colors::White) {
        kingPosition = whiteKingPosition;
    }
    else {
        kingPosition = blackKingPosition;
    }
    // Iterate through all opponent pieces
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col];
            if (piece && piece->getColor() != kingColor) {
                // Check if this piece can attack the king's position
                if (piece->isValidMove(piece->getPosition(), kingPosition, *this)) {
                    // King is in check
                    cout << piece->getName() << " at [" << piece->getPosition().row << ", " << piece->getPosition().col
                        << "] threatens King at [" << kingPosition.row << ", " << kingPosition.col << "]" << endl;
                    return true;
                }
            }
        }
    }
    // King is not in check
    return false;
}

bool Board::isCheckMate(Colors playerColor) {
    // Step 1: Check if the player's king is in check
    if (!isInCheck(playerColor)) {
        return false;  // The king is not in check, so it's not checkmate
    }

    // Get the king's position based on the color
    Position kingPosition = (playerColor == Colors::White) ? whiteKingPosition : blackKingPosition;

    // Step 2: Check if King can move to a safe square
    static const Position directions[8] = { {1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,-1}, {1,-1}, {-1,1} };
    for (const Position& dir : directions) {
        Position newPos = { kingPosition.row + dir.row, kingPosition.col + dir.col };
        if (newPos.row >= 0 && newPos.row < 8 && newPos.col >= 0 && newPos.col < 8) {
            Piece* temp = board[newPos.row][newPos.col];
            movePiece(kingPosition, newPos, true);
            bool stillInCheck = isInCheck(playerColor);
            movePiece(newPos, kingPosition, true); // Revert the move
            board[newPos.row][newPos.col] = temp; // Restore any captured piece
            if (!stillInCheck) {
                return false; // Found a safe square for the king
            }
        }
    }

    // Step 3 & 4: Check other pieces if they can block the check or capture the threatening piece
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Position start = { i, j };
            Piece* piece = getPieceAt(start);
            if (piece && piece->getColor() == playerColor) {
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        Position end = { x, y };
                        Piece* originalPieceAtStart = getPieceAt(start);
                        Piece* originalPieceAtEnd = getPieceAt(end);  // Store the piece at the destination

                        if (piece->isValidMove(start, end, *this)) {
                            movePiece(start, end, true); // Simulated move
                            bool stillInCheck = isInCheck(playerColor);
                            movePiece(end, start, true); // Revert the move

                            // Restore the board to its original state
                            board[start.row][start.col] = originalPieceAtStart;
                            board[end.row][end.col] = originalPieceAtEnd;

                            if (!stillInCheck) {
                                return false; // Found a move that breaks the check
                            }
                        }


                    }
                }
            }
        }
    }
    // If no moves found that can break the check, it's checkmate
    return true;
}

        
    bool Board::isDraw(Colors currentPlayer) const {
        // Check for conditions that can lead to a draw
        if (!isInCheck(currentPlayer)) {
            // Check for stalemate (no legal moves available)
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    Piece* piece = board[row][col];
                    if (piece && piece->getColor() == currentPlayer) {
                        for (int newRow = 0; newRow < 8; newRow++) {
                            for (int newCol = 0; newCol < 8; newCol++) {
                                Position start = { row, col };
                                Position end = { newRow, newCol };
                                if (piece->isValidMove(start, end, *this)) {
                                    // There's a legal move for the current player
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
            // No legal moves available, it's a stalemate
            return true;
        }
       
        // Fifty-Move Rule:
        if (movesWithoutPawnOrCapture >= 50) {
            return true;
        }

        // Insufficient Material:
        // Check the board for specific combinations like K vs K, K vs KB, K vs KN
        int numWhiteBishops = 0, numWhiteKnights = 0, numBlackBishops = 0, numBlackKnights = 0;
        int numWhiteBishopsLightSquare = 0, numWhiteBishopsDarkSquare = 0;
        int numBlackBishopsLightSquare = 0, numBlackBishopsDarkSquare = 0;

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Piece* piece = board[row][col];
                if (piece) {
                    if (piece->getColor() == Colors::White) {
                        if (typeid(*piece) == typeid(Bishop)) {
                            (row + col) % 2 == 0 ? numWhiteBishopsLightSquare++ : numWhiteBishopsDarkSquare++;
                        }
                    }
                    else if (piece->getColor() == Colors::Black) {
                        if (typeid(*piece) == typeid(Bishop)) {
                            (row + col) % 2 == 0 ? numBlackBishopsLightSquare++ : numBlackBishopsDarkSquare++;
                        }
                    }
                }
            }
        }

        // Handle the four cases of insufficient material
        if (
            (numWhiteBishopsLightSquare + numWhiteBishopsDarkSquare + numBlackBishopsLightSquare + numBlackBishopsDarkSquare + numWhiteKnights + numBlackKnights == 0) || // K vs K
            (numWhiteBishopsLightSquare + numWhiteBishopsDarkSquare + numBlackBishopsLightSquare + numBlackKnights + numWhiteKnights == 1 && numBlackBishopsDarkSquare == 0) || // K vs K and (B or N)
            (numBlackBishopsLightSquare + numBlackBishopsDarkSquare + numWhiteBishopsLightSquare + numWhiteKnights + numBlackKnights == 1 && numWhiteBishopsDarkSquare == 0) || // K vs K and (B or N)
            (numWhiteBishopsLightSquare == 1 && numBlackBishopsLightSquare == 1) || // K and B vs K and B (both on light square)
            (numWhiteBishopsDarkSquare == 1 && numBlackBishopsDarkSquare == 1) // K and B vs K and B (both on dark square)
            ) {
            return true;
        }

        return false; // Default case, not a draw
    }


    // Check if specific position in the board is uncer attack.
    bool Board::isUnderAttack(Colors color, Position position) const {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Piece* piece = board[row][col];
                if (piece && piece->getColor() != color) {
                    if (piece->isValidMove({ row,col }, position, *this)) {
                        return true;  // The position is under attack.
                    }
                }
            }
        }
        return false;  // The position is not under attack.
    }


    bool Board::canCastle(const Position& kingStart, const Position& kingEnd) const {
        // Identify if this is a white or black king based on the starting row
        Colors kingColor = (kingStart.row == 0) ? Colors::White : Colors::Black;

        // Get the king and rook pieces based on the start positions
        Piece* king = getPieceAt(kingStart);
        Piece* rook = nullptr;

        // Ensure the king is actually King
        if (!king || king->getType() != Pieces::King) {
            return false;
        }

        // Check direction of castling (kingside or queenside) based on column movement
        if (kingEnd.col == 6) { // Kingside
            rook = getPieceAt({ kingStart.row, 7 });
        }
        else if (kingEnd.col == 2) { // Queenside
            rook = getPieceAt({ kingStart.row, 0 });
        }
        else {
            return false; // Invalid column for castling
        }

        // Ensure the king and rook are the correct types and colors
        if (!king || king->getType() != Pieces::King || king->getColor() != kingColor ||
            !rook || rook->getType() != Pieces::Rook || rook->getColor() != kingColor) {
            return false;
        }

        // Check if the king or rook has moved before
        if (king->getHasMoved() || rook->getHasMoved()) {
            return false;
        }

        // Ensure all squares between the king and rook are empty
        int startCol = min(kingStart.col, rook->getPosition().col);
        int endCol = max(kingStart.col, rook->getPosition().col);
        for (int col = startCol + 1; col < endCol; col++) {
            if (getPieceAt({ kingStart.row, col })) {
                return false;
            }
        }

        // Ensure the squares the king moves across are not under attack
        int checkStartCol = kingStart.col;
        int checkEndCol = kingEnd.col;
        for (int col = checkStartCol; col != checkEndCol; col += (checkEndCol > checkStartCol) ? 1 : -1) {
            if (isUnderAttack(kingColor, { kingStart.row, col })) {
                return false;
            }
        }

        // If all conditions are met, castling is possible
        return true;
    }