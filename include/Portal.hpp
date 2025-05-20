#pragma once

#include "Utilities.hpp"
#include <string>
#include <vector>
#include <unordered_set>

class Portal {
public:
    Portal(std::string id, Position entry, Position exit, 
           bool preserveDirection = true, int cooldown = 0);
    
    // Getters
    std::string getId() const { return id; }
    Position getEntry() const { return entry; }
    Position getExit() const { return exit; }
    bool doesPreserveDirection() const { return preserveDirection; }
    int getCooldown() const { return cooldown; }
    
    // Check if a piece color is allowed to use this portal
    bool isColorAllowed(Color color) const;
    
    // Add allowed color
    void addAllowedColor(Color color);
    
    // Cooldown management
    bool isInCooldown() const { return remainingCooldown > 0; }
    void activateCooldown() { remainingCooldown = cooldown; }
    void decrementCooldown() { if (remainingCooldown > 0) remainingCooldown--; }
    
private:
    std::string id;
    Position entry;
    Position exit;
    bool preserveDirection;
    int cooldown;
    int remainingCooldown;
    std::unordered_set<Color> allowedColors;
};