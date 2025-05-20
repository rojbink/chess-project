#include "../include/ChessPiece.hpp"
#include "../include/ChessBoard.hpp"
#include <cmath>

// Base ChessPiece implementation
ChessPiece::ChessPiece(Color color, const std::string& type)
    : color(color), type(type), moved(false) {}

bool ChessPiece::hasSpecialAbility(const std::string& ability) const {
    auto it = specialAbilities.find(ability);
    return it != specialAbilities.end() && it->second > 0;
}

int ChessPiece::getAbilityValue(const std::string& ability) const {
    auto it = specialAbilities.find(ability);
    return (it != specialAbilities.end()) ? it->second : 0;
}

void ChessPiece::setSpecialAbility(const std::string& ability, int value) {
    specialAbilities[ability] = value;
}

// Movement validation helpers
bool ChessPiece::isValidForwardMove(const Position& from, const Position& to, const ChessBoard& board) const {
    // Check if the move is vertical (same x-coordinate)
    if (from.x != to.x) return false;
    
    // Calculate the direction and distance based on piece color
    int direction = (color == Color::WHITE) ? 1 : -1;
    int distance = (to.y - from.y) * direction;
    
    // Check if the distance is valid based on movement properties
    int maxDistance = movementProperties.find("forward") != movementProperties.end() 
                     ? movementProperties.at("forward") : 0;
    
    // Special case for first move (e.g., pawns)
    if (!moved && movementProperties.find("first_move_forward") != movementProperties.end()) {
        maxDistance = std::max(maxDistance, movementProperties.at("first_move_forward"));
    }
    
    // Check if the distance is within range
    if (distance <= 0 || distance > maxDistance) return false;
    
    // Check if the path is clear (no pieces in between)
    return isPathClear(from, to, board);
}

bool ChessPiece::isValidSidewaysMove(const Position& from, const Position& to, const ChessBoard& board) const {
    // Check if the move is horizontal (same y-coordinate)
    if (from.y != to.y) return false;
    
    // Calculate the distance
    int distance = std::abs(to.x - from.x);
    
    // Check if the distance is valid based on movement properties
    int maxDistance = movementProperties.find("sideways") != movementProperties.end() 
                     ? movementProperties.at("sideways") : 0;
    
    // Check if the distance is within range
    if (distance <= 0 || distance > maxDistance) return false;
    
    // Check if the path is clear (no pieces in between)
    return isPathClear(from, to, board);
}

bool ChessPiece::isValidDiagonalMove(const Position& from, const Position& to, const ChessBoard& board) const {
    // Check if the move is diagonal (equal change in x and y)
    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);
    
    if (dx != dy) return false;
    
    // Calculate the distance
    int distance = dx; // or dy, they're equal
    
    // Check if the distance is valid based on movement properties
    int maxDistance = movementProperties.find("diagonal") != movementProperties.end() 
                     ? movementProperties.at("diagonal") : 0;
    
    // Check if this is a diagonal capture (e.g., for pawns)
    bool isDiagonalCapture = false;
    if (movementProperties.find("diagonal_capture") != movementProperties.end()) {
        maxDistance = std::max(maxDistance, movementProperties.at("diagonal_capture"));
        // For diagonal captures, usually need a piece to capture
        if (distance <= movementProperties.at("diagonal_capture")) {
            isDiagonalCapture = true;
        }
    }
    
    // Check if the distance is within range
    if (distance <= 0 || distance > maxDistance) return false;
    
    // For diagonal captures (e.g., pawns), usually need a piece to capture
    if (isDiagonalCapture) {
        const ChessPiece* targetPiece = board.getPieceAt(to);
        // If no piece to capture, it's not a valid move (unless it's en passant, which would be handled separately)
        if (!targetPiece) return false;
        // Can't capture own pieces
        if (targetPiece->getColor() == color) return false;
    }
    
    // Check if the path is clear (no pieces in between)
    return isPathClear(from, to, board);
}

bool ChessPiece::isValidLShapeMove(const Position& from, const Position& to) const {
    // Check if the move is L-shaped (Knight's move)
    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);
    
    // Knight's move: 2 squares in one direction and 1 square perpendicular
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

bool ChessPiece::isPathClear(const Position& from, const Position& to, const ChessBoard& board) const {
    // Calculate the direction of movement
    int dx = (to.x > from.x) ? 1 : (to.x < from.x) ? -1 : 0;
    int dy = (to.y > from.y) ? 1 : (to.y < from.y) ? -1 : 0;
    
    // Start from the position after 'from' and check each position until just before 'to'
    Position current(from.x + dx, from.y + dy);
    
    while (!(current == to)) {
        // If there's a piece in the path, the path is not clear
        if (!board.isPositionEmpty(current)) {
            return false;
        }
        
        // Move to the next position in the path
        current.x += dx;
        current.y += dy;
    }
    
    // Check if the destination has a piece of the same color
    const ChessPiece* targetPiece = board.getPieceAt(to);
    if (targetPiece && targetPiece->getColor() == color) {
        return false; // Can't capture own pieces
    }
    
    return true;
}

// Factory method to create piece based on type
std::unique_ptr<ChessPiece> ChessPiece::createPiece(
    const std::string& type, Color color,
    const std::unordered_map<std::string, int>& movement,
    const std::unordered_map<std::string, int>& abilities) {
    
    // Standard pieces
    if (type == "King") return std::make_unique<King>(color);
    if (type == "Queen") return std::make_unique<Queen>(color);
    if (type == "Rook") return std::make_unique<Rook>(color);
    if (type == "Bishop") return std::make_unique<Bishop>(color);
    if (type == "Knight") return std::make_unique<Knight>(color);
    if (type == "Pawn") return std::make_unique<Pawn>(color);
    
    // Custom piece
    return std::make_unique<CustomPiece>(color, type, movement, abilities);
}

// Standard chess piece implementations
King::King(Color color) : ChessPiece(color, "King") {
    movementProperties["forward"] = 1;
    movementProperties["sideways"] = 1;
    movementProperties["diagonal"] = 1;
    specialAbilities["royal"] = 1;
    specialAbilities["castling"] = 1;
}

bool King::canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const {
    // Check for castle move (handled separately by GameManager)
    if (!moved && std::abs(to.x - from.x) == 2 && to.y == from.y) {
        return true; // GameManager will validate castling conditions
    }
    
    // Standard king movement (one square in any direction)
    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);
    
    if (dx <= 1 && dy <= 1 && (dx > 0 || dy > 0)) {
        return isPathClear(from, to, board);
    }
    
    return false;
}

Queen::Queen(Color color) : ChessPiece(color, "Queen") {
    movementProperties["forward"] = 8;
    movementProperties["sideways"] = 8;
    movementProperties["diagonal"] = 8;
}

bool Queen::canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const {
    // Queen can move horizontally, vertically, or diagonally
    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);
    
    // Horizontal move
    if (dy == 0 && dx > 0) {
        return isValidSidewaysMove(from, to, board);
    }
    
    // Vertical move
    if (dx == 0 && dy > 0) {
        return isValidForwardMove(from, to, board);
    }
    
    // Diagonal move
    if (dx == dy && dx > 0) {
        return isValidDiagonalMove(from, to, board);
    }
    
    return false;
}

Rook::Rook(Color color) : ChessPiece(color, "Rook") {
    movementProperties["forward"] = 8;
    movementProperties["sideways"] = 8;
}

bool Rook::canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const {
    // Rook can move horizontally or vertically
    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);
    
    // Horizontal move
    if (dy == 0 && dx > 0) {
        return isValidSidewaysMove(from, to, board);
    }
    
    // Vertical move
    if (dx == 0 && dy > 0) {
        return isValidForwardMove(from, to, board);
    }
    
    return false;
}

Bishop::Bishop(Color color) : ChessPiece(color, "Bishop") {
    movementProperties["diagonal"] = 8;
}

bool Bishop::canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const {
    // Bishop can move diagonally
    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);
    
    // Diagonal move
    if (dx == dy && dx > 0) {
        return isValidDiagonalMove(from, to, board);
    }
    
    return false;
}

Knight::Knight(Color color) : ChessPiece(color, "Knight") {
    specialAbilities["jump_over"] = 1;
}

bool Knight::canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const {
    // Knight moves in L-shape and can jump over pieces
    if (isValidLShapeMove(from, to)) {
        // Knight doesn't care about pieces in the path, only at the destination
        const ChessPiece* targetPiece = board.getPieceAt(to);
        return !targetPiece || targetPiece->getColor() != color;
    }
    
    return false;
}

Pawn::Pawn(Color color) : ChessPiece(color, "Pawn") {
    movementProperties["forward"] = 1;
    movementProperties["first_move_forward"] = 2;
    movementProperties["diagonal_capture"] = 1;
    specialAbilities["promotion"] = 1;
    specialAbilities["en_passant"] = 1;
}

bool Pawn::canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const {
    // Determine pawn's direction based on color
    int direction = (color == Color::WHITE) ? 1 : -1;
    
    // Check if it's a diagonal capture
    int dx = to.x - from.x;
    int dy = to.y - from.y;
    
    // Diagonal capture
    if (std::abs(dx) == 1 && dy == direction) {
        const ChessPiece* targetPiece = board.getPieceAt(to);
        
        // Must have a piece to capture (or be en passant, handled by GameManager)
        if (targetPiece && targetPiece->getColor() != color) {
            return true;
        }
        
        // En passant is handled separately by the GameManager
        return false;
    }
    
    // Forward move (can't capture forward)
    if (dx == 0 && (dy == direction || (!moved && dy == 2 * direction))) {
        // Check if the path is clear
        Position intermediate(from.x, from.y + direction);
        
        // For two-square move, check both squares
        if (dy == 2 * direction) {
            if (!board.isPositionEmpty(intermediate)) {
                return false;
            }
        }
        
        // Destination must be empty
        return board.isPositionEmpty(to);
    }
    
    return false;
}

// Custom piece implementation
CustomPiece::CustomPiece(Color color, const std::string& type, 
                         const std::unordered_map<std::string, int>& movement,
                         const std::unordered_map<std::string, int>& abilities)
    : ChessPiece(color, type) {
    
    // Copy movement properties
    for (const auto& [key, value] : movement) {
        movementProperties[key] = value;
    }
    
    // Copy special abilities
    for (const auto& [key, value] : abilities) {
        specialAbilities[key] = value;
    }
}

bool CustomPiece::canMoveTo(const Position& from, const Position& to, const ChessBoard& board) const {
    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);
    
    // L-shaped move
    if (movementProperties.find("l_shape") != movementProperties.end() && 
        movementProperties.at("l_shape") > 0 && 
        isValidLShapeMove(from, to)) {
        // Knights can jump over pieces
        const ChessPiece* targetPiece = board.getPieceAt(to);
        return !targetPiece || targetPiece->getColor() != color;
    }
    
    // Diagonal move
    if (dx == dy && dx > 0 && 
        movementProperties.find("diagonal") != movementProperties.end() && 
        dx <= movementProperties.at("diagonal")) {
        return isValidDiagonalMove(from, to, board);
    }
    
    // Horizontal move
    if (dy == 0 && dx > 0 && 
        movementProperties.find("sideways") != movementProperties.end() && 
        dx <= movementProperties.at("sideways")) {
        return isValidSidewaysMove(from, to, board);
    }
    
    // Vertical move
    if (dx == 0 && dy > 0) {
        int direction = (color == Color::WHITE) ? 1 : -1;
        int moveDistance = direction * dy;
        
        // Check if moving in the correct direction
        if ((direction > 0 && to.y < from.y) || (direction < 0 && to.y > from.y)) {
            return false;
        }
        
        if (movementProperties.find("forward") != movementProperties.end() && 
            moveDistance <= movementProperties.at("forward")) {
            return isValidForwardMove(from, to, board);
        }
    }
    
    return false;
}