#include "../include/Portal.hpp"

Portal::Portal(std::string id, Position entry, Position exit, 
               bool preserveDirection, int cooldown)
    : id(id), entry(entry), exit(exit), 
      preserveDirection(preserveDirection), cooldown(cooldown), remainingCooldown(0) {
    
    // By default, allow both colors to use the portal
    allowedColors.insert(Color::WHITE);
    allowedColors.insert(Color::BLACK);
}

bool Portal::isColorAllowed(Color color) const {
    return allowedColors.find(color) != allowedColors.end();
}

void Portal::addAllowedColor(Color color) {
    allowedColors.insert(color);
}