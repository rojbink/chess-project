#pragma once

#include "Portal.hpp"
#include "ChessPiece.hpp"
#include <vector>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

// Class to manage portal cooldowns using a queue
struct PortalCooldown {
    std::string portalId;
    int turnsRemaining;
    
    PortalCooldown(std::string id, int turns) : portalId(id), turnsRemaining(turns) {}
};

class PortalSystem {
public:
    PortalSystem() = default;
    ~PortalSystem() = default;
    
    // Portal management
    void addPortal(std::unique_ptr<Portal> portal);
    const Portal* getPortalById(const std::string& id) const;
    const Portal* getPortalByEntry(const Position& position) const;
    bool isEntryPoint(const Position& position) const;
    bool isExitPoint(const Position& position) const;
    
    // Teleportation logic
    bool canUsePortal(const Portal* portal, const ChessPiece* piece) const;
    Position getExitPosition(const Position& entryPos) const;
    void usePortal(const std::string& portalId);
    
    // Cooldown management
    void processCooldowns();
    std::vector<std::string> getPortalsInCooldown() const;
    
private:
    std::vector<std::unique_ptr<Portal>> portals;
    std::queue<PortalCooldown> cooldownQueue;
    std::unordered_map<std::string, bool> portalCooldownStatus;
};