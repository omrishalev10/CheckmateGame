/*
 * File: Classes.h
 * Author: Omri Shalev
 * Date: September 16, 2023
 * Description: Header file containing class declarations and prototypes for the chess game.
 */

#pragma once

#include <string>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

// Define enums
enum class Colors { Empty, Black, White };
enum class Pieces { None, Pawn, Knight, Bishop, Rook, Queen, King };

// Declare the operator<< overload (without defining it here)
ostream& operator<<(ostream& os, const Colors& color);

struct Position {
    int row;
    int col;

    Position() : row(0), col(0) {}
    Position(int r, int c) : row(r), col(c) {}

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
};

// Forward declaration
class Board;

class Piece {
protected:
    Colors pieceColor;
    Pieces pieceName;
    Position position;
    bool hasMoved; // To know if the Pawn made his first move. 
    string symbol;

public:
    Piece(Colors color, Pieces name, Position pos);
    Piece(Colors color, Pieces name);
    virtual ~Piece() {}
    bool getHasMoved();
    void setHasMoved(bool moved);
    Colors getColor() const; 
    virtual string getName() const = 0;
    virtual bool isValidMove(Position start, Position end, const Board& board) const = 0;
    Position getPosition() const;
    void setPosition(const Position& newPosition);
    virtual Pieces getType() const = 0;
    virtual char getSymbol() const = 0;
};

class Board {
private:
    Piece*** board;
    Position whiteKingPosition;
    Position blackKingPosition;
    std::map<std::string, int> boardHistory;
    int movesWithoutPawnOrCapture = 0;

public:
    Board();
    ~Board();
    Piece* getPieceAt(Position pos) const;
    void printBoard() const;
    bool isOpponentAt(Position pos, Colors color) const;
    bool isInCheck(Colors kingColor) const;
    bool isCheckMate(Colors kingColor);
    bool movePiece(const Position& start, const Position& end, bool isSimulation = false);
    bool isDraw(Colors currentPlayer) const;
    void placePieceAt(const Position& position, Piece* piece);
    bool isUnderAttack(Colors opponentColor, Position position) const;
    bool canCastle(const Position& kingStart, const Position& kingEnd) const;
};

// Include chess piece headers here
#include "ChessPieces.h"
