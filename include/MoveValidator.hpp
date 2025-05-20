#pragma once

#include "ChessBoard.hpp"
#include "PortalSystem.hpp"
#include "Utilities.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <memory>

// Simple graph representation for move validation
class MoveGraph {
public:
    // Add a node (position) to the graph
    void addNode(const Position& pos);
    
    // Add a directed edge between positions
    void addEdge(const Position& from, const Position& to, bool isPortal = false);
    
    // Get all neighbors of a position
    std::vector<Position> getNeighbors(const Position& pos) const;
    
    // Check if there's a portal edge between positions
    bool isPortalEdge(const Position& from, const Position& to) const;
    
    // Clear the graph
    void clear();
    
private:
    // Adjacency list representation of the graph
    std::unordered_map<Position, std::vector<Position>> adjacencyList;
    
    // Set of portal edges (from, to)
    std::unordered_set<std::pair<Position, Position>, 
                       std::function<size_t(const std::pair<Position, Position>&)>> portalEdges;
};

class MoveValidator {
public:
    MoveValidator(const ChessBoard& board, const PortalSystem& portalSystem);
    
    // Validate a move using graph-based approach
    bool isValidMove(const Position& from, const Position& to) const;
    
    // Find a valid path between positions (including portals)
    std::vector<Position> findPath(const Position& from, const Position& to) const;
    
    // Check if a piece can use a portal
    bool canUsePortal(const Position& pos, const ChessPiece* piece) const;
    
    // Rebuild the graph (called when board state changes)
    void rebuildGraph();
    
private:
    const ChessBoard& board;
    const PortalSystem& portalSystem;
    MoveGraph graph;
    
    // Build the graph representation of possible moves
    void buildGraph();
    
    // Add standard movement edges to the graph
    void addStandardMoveEdges();
    
    // Add portal edges to the graph
    void addPortalEdges();
    
    // BFS algorithm to find path between positions
    std::vector<Position> bfs(const Position& start, const Position& goal) const;
};