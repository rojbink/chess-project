#pragma once

#include "Utilities.hpp"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

class ChessPiece {
public:
    ChessPiece(Color color, const std::string& type);
    virtual ~ChessPiece() = default;
    
    // Getters
    Color getColor() const { return color; }
    std::string getType() const { return type; }
    bool hasMoved() const { return moved; }
    
    // Mark piece as moved
    void setMoved() { moved = true; }
    
    // Movement validation
    virtual bool canMoveTo(const Position& from, const Position& to, 
                          const class ChessBoard& board) const = 0;
    
    // Get symbol for display
    virtual std::string getSymbol() const = 0;
    
    // Special abilities
    bool hasSpecialAbility(const std::string& ability) const;
    int getAbilityValue(const std::string& ability) const;
    void setSpecialAbility(const std::string& ability, int value = 1);
    
    // Factory method to create piece from type string
    static std::unique_ptr<ChessPiece> createPiece(const std::string& type, Color color,
                                              const std::unordered_map<std::string, int>& movement,
                                              const std::unordered_map<std::string, int>& abilities);

protected:
    Color color;
    std::string type;
    bool moved;
    std::unordered_map<std::string, int> specialAbilities;
    std::unordered_map<std::string, int> movementProperties;
    
    // Helper methods for movement validation
    bool isValidForwardMove(const Position& from, const Position& to, const ChessBoard& board) const;
    bool isValidSidewaysMove(const Position& from, const Position& to, const ChessBoard& board) const;
    bool isValidDiagonalMove(const Position& from, const Position& to, const ChessBoard& board) const;
    bool isValidLShapeMove(const Position& from, const Position& to) const;
    bool isPathClear(const Position& from, const Position& to, const ChessBoard& board) const;
};

// Standard Chess Pieces
class King : public ChessPiece {
public:
    King(Color color);
    bool canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const override;
    std::string getSymbol() const override;
};

class Queen : public ChessPiece {
public:
    Queen(Color color);
    bool canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const override;
    std::string getSymbol() const override;
};

class Rook : public ChessPiece {
public:
    Rook(Color color);
    bool canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const override;
    std::string getSymbol() const override;
};

class Bishop : public ChessPiece {
public:
    Bishop(Color color);
    bool canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const override;
    std::string getSymbol() const override;
};

class Knight : public ChessPiece {
public:
    Knight(Color color);
    bool canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const override;
    std::string getSymbol() const override;
};

class Pawn : public ChessPiece {
public:
    Pawn(Color color);
    bool canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const override;
    std::string getSymbol() const override;
};

// Custom piece implementation with configurable movement
class CustomPiece : public ChessPiece {
public:
    CustomPiece(Color color, const std::string& type, 
                const std::unordered_map<std::string, int>& movement,
                const std::unordered_map<std::string, int>& abilities);
    bool canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const override;
    std::string getSymbol() const override;
};