#pragma once

#include "ChessBoard.hpp"
#include "ConfigReader.hpp" // For GameConfig, Movement, SpecialAbilities
#include "ChessPiece.hpp"   // For ChessPiece::createPiece and Color enum
#include <memory>           // For std::unique_ptr if we choose to use it for board_
#include <vector>
#include <string>
#include <unordered_map>   // For the helper maps in piece creation

class GameManager {
public:
    GameManager(const GameConfig& config);
    ~GameManager() = default;

    void initializeGame();
    void displayBoard() const;
    
    // Future methods:
    // void runGame();
    // bool processMove(const Position& from, const Position& to);
    // bool isGameOver() const;
    // Color getCurrentPlayer() const;

private:
    const GameConfig& gameConfig_; // Store a reference to the loaded configuration
    ChessBoard board_;             // GameManager owns the board

    // Helper to convert config structs to maps needed for piece creation
    // This is similar to the lambda previously in main.cpp
    std::pair<std::unordered_map<std::string, int>, std::unordered_map<std::string, int>>
    convertConfigMapsForPieceCreation(const Movement& m_conf, const SpecialAbilities& a_conf);
};
