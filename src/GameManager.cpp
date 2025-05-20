#include "../include/GameManager.hpp"
#include <iostream> // For std::cout, std::cerr

GameManager::GameManager(const GameConfig& config)
    : gameConfig_(config), board_(config.game_settings.board_size) {
    // Constructor can be simple, initialization happens in initializeGame()
}

// Helper to convert config structs to maps needed for piece creation
std::pair<std::unordered_map<std::string, int>, std::unordered_map<std::string, int>>
GameManager::convertConfigMapsForPieceCreation(const Movement& m_conf, const SpecialAbilities& a_conf) {
    std::unordered_map<std::string, int> movement_map;
    if (m_conf.forward > 0) movement_map["forward"] = m_conf.forward;
    if (m_conf.sideways > 0) movement_map["sideways"] = m_conf.sideways;
    if (m_conf.diagonal > 0) movement_map["diagonal"] = m_conf.diagonal;
    if (m_conf.l_shape) movement_map["l_shape"] = 1; // Assuming 1 means true
    if (m_conf.diagonal_capture > 0) movement_map["diagonal_capture"] = m_conf.diagonal_capture;
    if (m_conf.first_move_forward > 0) movement_map["first_move_forward"] = m_conf.first_move_forward;

    std::unordered_map<std::string, int> abilities_map;
    if (a_conf.castling) abilities_map["castling"] = 1;
    if (a_conf.royal) abilities_map["royal"] = 1;
    if (a_conf.jump_over) abilities_map["jump_over"] = 1;
    if (a_conf.promotion) abilities_map["promotion"] = 1;
    if (a_conf.en_passant) abilities_map["en_passant"] = 1;
    for(const auto& custom_ab : a_conf.custom_abilities) {
        if(custom_ab.second) abilities_map[custom_ab.first] = 1; // Assuming value 1 for true
    }
    return std::make_pair(movement_map, abilities_map);
}

void GameManager::initializeGame() {
    // Populate the board with standard pieces
    for (const auto &piece_config : gameConfig_.pieces) {
        auto [movement_map, abilities_map] = convertConfigMapsForPieceCreation(piece_config.movement, piece_config.special_abilities);
        if (piece_config.positions.count("white")) {
            for (const auto &pos : piece_config.positions.at("white")) {
                auto piece = ChessPiece::createPiece(piece_config.type, Color::WHITE, movement_map, abilities_map);
                if (piece) {
                    board_.placePiece(std::move(piece), {pos.x, pos.y});
                } else {
                    std::cerr << "Failed to create piece: " << piece_config.type << std::endl;
                }
            }
        }
        if (piece_config.positions.count("black")) {
            for (const auto &pos : piece_config.positions.at("black")) {
                auto piece = ChessPiece::createPiece(piece_config.type, Color::BLACK, movement_map, abilities_map);
                if (piece) {
                    board_.placePiece(std::move(piece), {pos.x, pos.y});
                } else {
                    std::cerr << "Failed to create piece: " << piece_config.type << std::endl;
                }
            }
        }
    }

    // Populate the board with custom pieces (if any)
    for (const auto &piece_config : gameConfig_.custom_pieces) {
        auto [movement_map, abilities_map] = convertConfigMapsForPieceCreation(piece_config.movement, piece_config.special_abilities);
        if (piece_config.positions.count("white")) {
            for (const auto &pos : piece_config.positions.at("white")) {
                auto piece = ChessPiece::createPiece(piece_config.type, Color::WHITE, movement_map, abilities_map);
                if (piece) {
                    board_.placePiece(std::move(piece), {pos.x, pos.y});
                } else {
                    std::cerr << "Failed to create custom piece: " << piece_config.type << std::endl;
                }
            }
        }
        if (piece_config.positions.count("black")) {
            for (const auto &pos : piece_config.positions.at("black")) {
                auto piece = ChessPiece::createPiece(piece_config.type, Color::BLACK, movement_map, abilities_map);
                if (piece) {
                    board_.placePiece(std::move(piece), {pos.x, pos.y});
                } else {
                    std::cerr << "Failed to create custom piece: " << piece_config.type << std::endl;
                }
            }
        }
    }
    // TODO: Initialize portals on the board if PortalSystem is part of GameManager or ChessBoard
}

void GameManager::displayBoard() const {
    std::cout << "\n==== Current Board State ====" << std::endl;
    board_.displayBoard(); // Assumes ChessBoard::displayBoard() prints to std::cout by default
} 