#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

// Define reward tiers
enum Rarity { COMMON = 0, NORMAL, GOLD, DIAMOND, RARE };

// Structure to hold reward details
struct Reward {
    std::string name;
    Rarity rarity;
    int percentage;
};

// Define player structure
struct Player {
    std::string name;
    bool isAdmin = false;
    int gold = 0;
    int tickets = 0;
    std::unordered_map<std::string, int> items;
    std::unordered_map<std::string, int> characters;
};

// Available rewards
std::map<std::string, Reward> rewardTable = {
    {"Gold Coin (+100)", {"Gold Coin (+100)", COMMON, 10}},
    {"Gold Coin (+250)", {"Gold Coin (+250)", COMMON, 10}},
    {"Gold Coin (+500)", {"Gold Coin (+500)", COMMON, 10}},
    {"Gold Coin (+1500)", {"Gold Coin (+1500)", COMMON, 10}},
    {"Ticket (+5)", {"Ticket (+5)", COMMON, 10}},
    {"Ticket (+10)", {"Ticket (+10)", COMMON, 10}},
    {"Ticket (+15)", {"Ticket (+15)", COMMON, 10}},
    {"Broken Sword", {"Broken Sword", COMMON, 10}},
    {"Green Shield", {"Green Shield", COMMON, 10}},
    {"Brand-new Sword", {"Brand-new Sword", NORMAL, 10}},
    {"Healing Tank", {"Healing Tank", NORMAL, 10}},
    {"Hero Armor", {"Hero Armor", NORMAL, 10}},
    {"Kaisen", {"Kaisen", NORMAL, 10}},
    {"Maiki", {"Maiki", NORMAL, 10}},
    {"Jinso", {"Jinso", NORMAL, 10}},
    {"Densetsu", {"Densetsu", NORMAL, 10}},
    {"Kage", {"Kage", NORMAL, 10}},
    {"Genjo", {"Genjo", NORMAL, 10}},
    {"Bukai", {"Bukai", GOLD, 10}},
    {"Gin", {"Gin", GOLD, 10}},
    {"Erinta", {"Erinta", GOLD, 10}},
    {"Domaisan", {"Domaisan", GOLD, 10}},
    {"Sainoken", {"Sainoken", GOLD, 10}},
    {"Pochi", {"Pochi", GOLD, 10}},
    {"The Lost Sword of Legend", {"The Lost Sword of Legend", GOLD, 10}},
    {"The Dragon Head Skull", {"The Dragon Head Skull", GOLD, 10}},
    {"Diamond Sword", {"Diamond Sword", DIAMOND, 10}},
    {"Diamond Ring", {"Diamond Ring", DIAMOND, 10}},
    {"Diamond Armor", {"Diamond Armor", DIAMOND, 10}},
    {"Diamond Shotgun", {"Diamond Shotgun", DIAMOND, 10}},
    {"Diamond Bow", {"Diamond Bow", DIAMOND, 10}},
    {"Scythe of Death", {"Scythe of Death", RARE, 10}},
    {"Cursed Swords", {"Cursed Swords", RARE, 10}},
    {"The Eyes of the Cursed Organization", {"The Eyes of the Cursed Organization", RARE, 10}},
    {"Mugen Shiro", {"Mugen Shiro", RARE, 10}},
    {"Shinno", {"Shinno", RARE, 10}}
};

// Function to simulate the gacha pull
Reward gachaPull(const std::string& playerName, int rollCount, bool& gotRare, Player& player) {
    int random = rand() % 100;
    if (rollCount >= 90) {
        gotRare = true;
        return rewardTable["Shinno"]; // Guaranteed rare reward
    }
    if (random < 50) {
        return rewardTable["Gold Coin (+100)"];
    } else if (random < 75) {
        return rewardTable["Brand-new Sword"];
    } else if (random < 90) {
        return rewardTable["Bukai"];
    } else if (random < 98) {
        return rewardTable["Diamond Sword"];
    } else {
        gotRare = true;
        return rewardTable["Scythe of Death"];
    }
}

// Utility functions
std::string getRarityName(Rarity rarity) {
    switch (rarity) {
        case COMMON: return "Common";
        case NORMAL: return "Normal";
        case GOLD: return "Gold";
        case DIAMOND: return "Diamond";
        case RARE: return "Rare";
        default: return "Unknown";
    }
}

void printInventory(const Player& player) {
    std::cout << "Inventory:\n";
    std::cout << "Gold Coins: " << player.gold << "\n";
    std::cout << "Tickets: " << player.tickets << "\n";
    std::cout << "Items:\n";
    for (const auto& item : player.items) {
        std::cout << "  " << item.first << ": " << item.second << "\n";
    }
    std::cout << "Characters:\n";
    for (const auto& character : player.characters) {
        std::cout << "  " << character.first << ": " << character.second << "\n";
    }
}

void printWelcomeMessage() {
    std::cout << "\033[1m\033[35mGACHA ROLLING | made by @fentyz.2601\033[0m\n";
    std::cout << "Welcome to Gacha rolling!\n";
    std::cout << "Set your name now: ";
}

void printCommands(bool isAdmin) {
    std::cout << "Available commands:\n";
    std::cout << "/shop - Exchange gold coins for tickets (100 gold coin/ticket).\n\n";
    std::cout << "/inventory - Show your current inventory.\n\n";
    std::cout << "/restart - Restart the game (all progress will be lost).\n\n";
    std::cout << "/roll [number] - Perform specified number of gacha rolls using tickets.\n\n";
    std::cout << "/setname - Change your player name.\n\n";
    std::cout << "/code [code] - Use a special code for rewards (STARTER or NEW).\n\n";
    if (isAdmin) {
        std::cout << "Admin commands:\n";
        std::cout << "/set [SUDO] | [name of tier], percent:[input number to change percentage of that tier] - Change percentage of reward tier.\n\n";
        std::cout << "/add [SUDO] [name item] [name of tier], [number how rare is that item] - Add an item to a tier.\n\n";
        std::cout << "/delete [SUDO] [name item] - Delete an item from a tier.\n\n";
        std::cout << "/roll check - Check roll of a player.\n\n";
    }
    std::cout << "/exit - Exit the game.\n\n";
}

void printCommandsPrompt(const Player& player) {
    std::cout << "\033[1m[" << player.name << "]:\033[0m ";
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    Player player;
    player.gold = 0;
    player.tickets = 50;
    player.isAdmin = false;
    bool codeUsed = false;

    printWelcomeMessage();
    std::getline(std::cin, player.name);
    if (player.name.substr(0, 6) == "Sudo |") {
        player.isAdmin = true;
    }
    printCommands(player.isAdmin);

    while (true) {
        printCommandsPrompt(player);
        std::string command;
        std::getline(std::cin, command);

        if (command == "/shop") {
            // Exchange gold for tickets
            if (player.gold >= 100) {
                player.gold -= 100;
                player.tickets += 1;
                std::cout << "Exchanged 100 gold coins for 1 ticket.\n";
            } else {
                std::cout << "Not enough gold coins.\n";
            }
        } else if (command == "/inventory") {
            printInventory(player);
        } else if (command == "/restart") {
            player.gold = 0;
            player.tickets = 50;
            player.items.clear();
            player.characters.clear();
            std::cout << "Game restarted.\n";
        } else if (command.substr(0, 5) == "/roll") {
            // Roll the gacha
            try {
                int rollCount = std::stoi(command.substr(6));
                if (player.tickets >= rollCount) {
                    player.tickets -= rollCount;
                    bool gotRare = false;
                    for (int i = 0; i < rollCount; ++i) {
                        Reward reward = gachaPull(player.name, rollCount, gotRare, player);
                        if (reward.rarity == COMMON || reward.rarity == NORMAL) {
                            player.items[reward.name]++;
                        } else {
                            player.characters[reward.name]++;
                        }
                        std::cout << "Rolled: " << reward.name << " (" << getRarityName(reward.rarity) << ")\n";
                    }
                } else {
                    std::cout << "Not enough tickets.\n";
                }
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid roll command. Please specify a valid number.\n";
            }
        } else if (command == "/setname") {
            std::cout << "Enter new name: ";
            std::getline(std::cin, player.name);
            std::cout << "Name changed to " << player.name << ".\n";
        } else if (command.substr(0, 5) == "/code") {
            std::string code = command.substr(6);
            if (!codeUsed && (code == "STARTER" || code == "NEW")) {
                player.tickets += 10;
                codeUsed = true;
                std::cout << "Code applied. You received 10 tickets.\n";
            } else {
                std::cout << "Invalid or already used code.\n";
            }
        } else if (player.isAdmin && command.substr(0, 4) == "/set") {
            // Admin command to set percentages (not implemented)
        } else if (player.isAdmin && command.substr(0, 4) == "/add") {
            // Admin command to add items (not implemented)
        } else if (player.isAdmin && command.substr(0, 7) == "/delete") {
            // Admin command to delete items (not implemented)
        } else if (player.isAdmin && command == "/roll check") {
            // Admin command to check rolls (not implemented)
        } else if (command == "/exit") {
            std::cout << "Thanks for playing my game | made by @fentyz.2601\n";
            break;
        } else {
            std::cout << "Invalid command.\n\n";
        }
    }

    return 0;
}
