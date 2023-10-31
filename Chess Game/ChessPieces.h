/*
 * File: ChessPieces.h
 * Author: Omri Shalev
 * Date: September 16, 2023
 * Description: Header file containing declarations for chess piece classes.
 */

#pragma once 

#include "Classes.h"

class Pawn : public Piece {
public:
    Pawn(Colors color, Position pos);
    bool isValidMove(Position start, Position end, const Board& board) const override;
    string getName() const override; 
    Pieces getType() const override;
    char getSymbol() const override;
};

class Rook : public Piece {
public:
    Rook(Colors color, Position pos);
    bool isValidMove(Position start, Position end, const Board& board) const override;
    string getName() const override;
    Pieces getType() const;
    char getSymbol() const;
};

class Knight : public Piece {
public:
    Knight(Colors color, Position pos);
    bool isValidMove(Position start, Position end, const Board& board) const override;
    string getName() const;
    Pieces getType() const;
    char getSymbol() const;
};

class Bishop : public Piece {
public:
    Bishop(Colors color, Position pos);
    bool isValidMove(Position start, Position end, const Board& board) const override;
    string getName() const;
    Pieces getType() const;
    char getSymbol() const;
};

class Queen : public Piece {
public:
    Queen(Colors color, Position pos);
    bool isValidMove(Position start, Position end, const Board& board) const override;
    string getName() const;
    Pieces getType() const;
    char getSymbol() const;  
};

class King : public Piece {
public:
    King(Colors color, Position pos);
    bool isValidMove(Position start, Position end, const Board& board) const override;
    string getName() const;
    Pieces getType() const;
    char getSymbol() const;
};
