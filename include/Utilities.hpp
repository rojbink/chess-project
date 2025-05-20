#pragma once

#include <string>
#include <utility>
#include <tuple>

// Position represents a coordinate on the chess board
struct Position {
    int x;
    int y;
    
    Position() : x(0), y(0) {}
    Position(int x, int y) : x(x), y(y) {}
    
    // Convert chess notation (e.g., "e4") to Position
    static Position fromChessNotation(const std::string& notation) {
        if (notation.length() < 2) return Position();
        
        char file = notation[0];
        char rank = notation[1];
        
        int x = file - 'a';
        int y = rank - '1';
        
        return Position(x, y);
    }
    
    // Convert Position to chess notation (e.g., Position(4, 3) -> "e4")
    std::string toChessNotation() const {
        char file = 'a' + x;
        char rank = '1' + y;
        
        return std::string(1, file) + std::string(1, rank);
    }
    
    // Convert Position to string format "x,y"
    std::string toString() const {
        return std::to_string(x) + "," + std::to_string(y);
    }
    
    // Equality operator for position comparison
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    // Less than operator for map/set usage
    bool operator<(const Position& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

// Hash function for Position to enable it as a key in unordered_map
namespace std {
    template<>
    struct hash<Position> {
        std::size_t operator()(const Position& pos) const {
            return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
        }
    };
}

// Color enumeration for chess pieces
enum class Color {
    WHITE,
    BLACK
};

// Represents a move from one position to another
struct Move {
    Position from;
    Position to;
    bool usedPortal;
    std::string portalId;
    
    Move() : usedPortal(false) {}
    Move(Position from, Position to) : from(from), to(to), usedPortal(false) {}
    Move(Position from, Position to, bool usedPortal, std::string portalId = "") 
        : from(from), to(to), usedPortal(usedPortal), portalId(portalId) {}
};

// Extended move record for the history stack
struct MoveRecord {
    Move move;
    std::string capturedPieceType;
    Color capturedPieceColor;
    bool hasCaptured;
    
    MoveRecord() : hasCaptured(false) {}
    MoveRecord(Move move) : move(move), hasCaptured(false) {}
    MoveRecord(Move move, std::string capturedType, Color capturedColor) 
        : move(move), capturedPieceType(capturedType), capturedPieceColor(capturedColor), hasCaptured(true) {}
};