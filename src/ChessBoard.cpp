#include "../include/ChessBoard.hpp"
#include <iostream>

ChessBoard::ChessBoard(int size) : size(size) {}

bool ChessBoard::placePiece(std::unique_ptr<ChessPiece> piece, const Position& pos) {
    // Check if position is within bounds
    if (!isWithinBounds(pos)) {
        return false;
    }
    
    // Check if position is already occupied
    if (!isPositionEmpty(pos)) {
        return false;
    }
    
    // Place the piece
    board[pos] = std::move(piece);
    return true;
}

std::unique_ptr<ChessPiece> ChessBoard::removePiece(const Position& pos) {
    // Check if there's a piece at the position
    auto it = board.find(pos);
    if (it == board.end()) {
        return nullptr;
    }
    
    // Remove the piece and return it
    std::unique_ptr<ChessPiece> piece = std::move(it->second);
    board.erase(it);
    return piece;
}

const ChessPiece* ChessBoard::getPieceAt(const Position& pos) const {
    auto it = board.find(pos);
    return (it != board.end()) ? it->second.get() : nullptr;
}

bool ChessBoard::movePiece(const Position& from, const Position& to) {
    // Check if there's a piece at the starting position
    auto it = board.find(from);
    if (it == board.end()) {
        return false;
    }
    
    // Check if the move is valid (should be checked by MoveValidator)
    if (!isMoveValid(from, to)) {
        return false;
    }
    
    // Remove any piece at the destination (capture)
    if (!isPositionEmpty(to)) {
        removePiece(to);
    }
    
    // Move the piece
    board[to] = std::move(it->second);
    board.erase(it);
    
    // Mark the piece as moved
    board[to]->setMoved();
    
    return true;
}

bool ChessBoard::isMoveValid(const Position& from, const Position& to) const {
    // Check if positions are within bounds
    if (!isWithinBounds(from) || !isWithinBounds(to)) {
        return false;
    }
    
    // Check if there's a piece at the starting position
    const ChessPiece* piece = getPieceAt(from);
    if (!piece) {
        return false;
    }
    
    // Check if the destination has a piece of the same color
    const ChessPiece* targetPiece = getPieceAt(to);
    if (targetPiece && targetPiece->getColor() == piece->getColor()) {
        return false;
    }
    
    // Check if the move is valid according to the piece's rules
    return piece->canMoveTo(from, to, *this);
}

bool ChessBoard::isPositionEmpty(const Position& pos) const {
    return board.find(pos) == board.end();
}

bool ChessBoard::isWithinBounds(const Position& pos) const {
    return pos.x >= 0 && pos.x < size && pos.y >= 0 && pos.y < size;
}

std::vector<std::pair<Position, const ChessPiece*>> ChessBoard::getPiecesByColor(Color color) const {
    std::vector<std::pair<Position, const ChessPiece*>> pieces;
    
    for (const auto& [pos, piece] : board) {
        if (piece->getColor() == color) {
            pieces.emplace_back(pos, piece.get());
        }
    }
    
    return pieces;
}

std::optional<Position> ChessBoard::findPiece(const std::string& type, Color color) const {
    for (const auto& [pos, piece] : board) {
        if (piece->getType() == type && piece->getColor() == color) {
            return pos;
        }
    }
    
    return std::nullopt;
}