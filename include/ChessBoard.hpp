#pragma once

#include "ChessPiece.hpp"
#include "Utilities.hpp"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <optional>

class ChessBoard {
public:
    ChessBoard(int size = 8);
    ~ChessBoard() = default;
    
    // Board state management
    bool placePiece(std::unique_ptr<ChessPiece> piece, const Position& pos);
    std::unique_ptr<ChessPiece> removePiece(const Position& pos);
    const ChessPiece* getPieceAt(const Position& pos) const;
    
    // Movement
    bool movePiece(const Position& from, const Position& to);
    bool isMoveValid(const Position& from, const Position& to) const;
    bool isPositionEmpty(const Position& pos) const;
    
    // Board properties
    int getSize() const { return size; }
    bool isWithinBounds(const Position& pos) const;
    
    // Get all pieces of a specific color
    std::vector<std::pair<Position, const ChessPiece*>> getPiecesByColor(Color color) const;
    
    // Find a specific piece type
    std::optional<Position> findPiece(const std::string& type, Color color) const;
    
private:
    // The board is represented as a hashmap from position to piece
    // This provides O(1) lookups for any position
    std::unordered_map<Position, std::unique_ptr<ChessPiece>> board;
    int size;
};