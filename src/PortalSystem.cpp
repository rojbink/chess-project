#include "../include/PortalSystem.hpp"

void PortalSystem::addPortal(std::unique_ptr<Portal> portal) {
    // Store the portal ID for quick cooldown lookup
    portalCooldownStatus[portal->getId()] = false;
    portals.push_back(std::move(portal));
}

const Portal* PortalSystem::getPortalById(const std::string& id) const {
    for (const auto& portal : portals) {
        if (portal->getId() == id) {
            return portal.get();
        }
    }
    return nullptr;
}

const Portal* PortalSystem::getPortalByEntry(const Position& position) const {
    for (const auto& portal : portals) {
        if (portal->getEntry() == position) {
            return portal.get();
        }
    }
    return nullptr;
}

bool PortalSystem::isEntryPoint(const Position& position) const {
    return getPortalByEntry(position) != nullptr;
}

bool PortalSystem::isExitPoint(const Position& position) const {
    for (const auto& portal : portals) {
        if (portal->getExit() == position) {
            return true;
        }
    }
    return false;
}

bool PortalSystem::canUsePortal(const Portal* portal, const ChessPiece* piece) const {
    if (!portal || !piece) {
        return false;
    }
    
    // Check if the portal is in cooldown
    if (portal->isInCooldown()) {
        return false;
    }
    
    // Check if the piece's color is allowed to use this portal
    if (!portal->isColorAllowed(piece->getColor())) {
        return false;
    }
    
    // Check if the piece has the portal_master ability (can use portals without restrictions)
    if (piece->hasSpecialAbility("portal_master")) {
        return true;
    }
    
    return true;
}

Position PortalSystem::getExitPosition(const Position& entryPos) const {
    const Portal* portal = getPortalByEntry(entryPos);
    return portal ? portal->getExit() : entryPos;
}

void PortalSystem::usePortal(const std::string& portalId) {
    // Find the portal by ID
    Portal* portal = nullptr;
    for (auto& p : portals) {
        if (p->getId() == portalId) {
            portal = p.get();
            break;
        }
    }
    
    if (!portal) {
        return;
    }
    
    // Activate the cooldown
    portal->activateCooldown();
    
    // Add to cooldown queue if cooldown is greater than 0
    if (portal->getCooldown() > 0) {
        portalCooldownStatus[portalId] = true;
        cooldownQueue.emplace(portalId, portal->getCooldown());
    }
}

void PortalSystem::processCooldowns() {
    // Create a temporary queue to handle requeuing
    std::queue<PortalCooldown> tempQueue;
    
    // Process each item in the cooldown queue
    while (!cooldownQueue.empty()) {
        PortalCooldown current = cooldownQueue.front();
        cooldownQueue.pop();
        
        // Decrease the cooldown
        current.turnsRemaining--;
        
        // If cooldown is still active, requeue it
        if (current.turnsRemaining > 0) {
            tempQueue.push(current);
        } else {
            // Reset cooldown status
            portalCooldownStatus[current.portalId] = false;
            
            // Find the portal and reset its cooldown
            for (auto& portal : portals) {
                if (portal->getId() == current.portalId) {
                    portal->decrementCooldown();
                    break;
                }
            }
        }
    }
    
    // Restore the queue with updated cooldowns
    cooldownQueue = std::move(tempQueue);
}

std::vector<std::string> PortalSystem::getPortalsInCooldown() const {
    std::vector<std::string> result;
    
    for (const auto& [id, inCooldown] : portalCooldownStatus) {
        if (inCooldown) {
            result.push_back(id);
        }
    }
    
    return result;
}