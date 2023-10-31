#include "Classes.h"
#include "ChessPieces.h"
#include "Helpers.h"
#include <iostream>
#include <sstream> // Include this header for stringstream
using namespace std;

int main() {
    // Initialize the chess board
    Board chessBoard;
    // Print the initial setup of the board
    cout << "Initial Chess Board Setup: " << std::endl;
    chessBoard.printBoard();
    cout << "\n";

    // Game loop variables
    bool gameOver = false;
    Colors currentPlayer = Colors::White;

    while (!gameOver) {
        // Print the current player's turn
        cout << (currentPlayer == Colors::White ? "White's Turn" : "Black's Turn") << endl;

        // Prompt the current player for a move input
        cout << "Enter your move (e.g., 'e2 to e4'): ";
        string moveInput;
        getline(cin, moveInput);

       // Parse the move input and identify the piece
        Position startPosition, endPosition;
        Piece* pieceToMove = parseMoveAndGetPiece(moveInput, currentPlayer, chessBoard, startPosition, endPosition);

        if (!pieceToMove) {
            cout << "Invalid move or piece. Try again." << endl;
            continue;
        }
        if (pieceToMove) {
            // Check if the move is valid for the identified piece
            if (!pieceToMove->isValidMove(startPosition, endPosition, chessBoard)) {
                if (typeid(*pieceToMove) == typeid(Bishop)) {
                    cout << "Bishops can only move diagonally. " << endl;
                }
                else if (typeid(*pieceToMove) == typeid(Knight)) {
                    cout << "Knights move in an L-shape: two squares in one direction and one square perpendicular, or vice versa." << endl;
                }
                else if (typeid(*pieceToMove) == typeid(Rook)) {
                    cout << "Rooks can only move in straight lines either horizontally or vertically." << endl;
                }
                else if (typeid(*pieceToMove) == typeid(Queen)) {
                    cout << "Queens can move in any straight line: horizontally, vertically, or diagonally." << endl;
                }
                else if (typeid(*pieceToMove) == typeid(King)) {
                    cout << "Kings can move one square in any direction: horizontally, vertically, or diagonally." << endl;
                }
                else if (typeid(*pieceToMove) == typeid(Pawn)) {
                    cout << "Pawns move forward one square but capture diagonally. They have a unique double move from their starting position." << endl;
                }
                continue;
            }

            // Store original pieces before making the move
            Piece* originalPieceAtStart = chessBoard.getPieceAt(startPosition);
            Piece* originalPieceAtEnd = chessBoard.getPieceAt(endPosition);

            // Apply the valid move to the chessboard
            chessBoard.movePiece(startPosition, endPosition);
           
            // Check if move puts own king in check
            if (chessBoard.isInCheck(currentPlayer)) {
                cout << "Invalid move. Your King is in check. Try another move." << endl;
                // Cancel the move if this move make my own king in check. Restore the board to its original state
                chessBoard.movePiece(endPosition, startPosition);
                if (originalPieceAtEnd) {
                    // Place back any captured piece. Note that this assumes the 'board' member of chessBoard is public.
                    // If it's private, you'd need to either make it public, use a friend function, or add a helper method in the Board class.
                    chessBoard.placePieceAt(endPosition, originalPieceAtEnd);
                    originalPieceAtEnd->setPosition(endPosition); // Update its position                 
                }
                continue;
            }

            // Check if move puts opponent's king in check
            Colors opponentColor = (currentPlayer == Colors::White) ? Colors::Black : Colors::White;
            if (chessBoard.isInCheck(opponentColor)) {
                cout << (currentPlayer == Colors::White ? "Black's King is in check!" : "White's King is in check!") << endl;
            }


            // Check for checkmate or draw conditions
            if (chessBoard.isCheckMate(opponentColor)) {
                cout << (currentPlayer == Colors::White ? "Black" : "White") << " wins by checkmate!" << endl;
                gameOver = true;
            }
            else if (chessBoard.isDraw(currentPlayer)) {
                cout << "The game is a draw." << endl;
                gameOver = true;
            }

            // Print the updated board
            cout << "Updated Chess Board: " << endl;
            chessBoard.printBoard();

            // Switch to the other player for the next turn
            currentPlayer = (currentPlayer == Colors::White) ? Colors::Black : Colors::White;

            cout << endl;
        }
    }

    return 0;
}


