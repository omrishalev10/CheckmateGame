/*
 * File: Helpers.h
 * Author: Omri Shalev
 * Date: September 16, 2023
 * Description: Header file containing help functions declarations.
 */
#pragma once

#include <string>
#include "Classes.h" // Include the necessary headers

Piece* parseMoveAndGetPiece(const string& moveInput, Colors currentPlayer, const Board& board, Position& startPosition, Position& endPosition);
bool parsePosition(const string& positionStr, Position& position);
