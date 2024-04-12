#pragma once
#include <iostream>
using namespace std;

class Player {

public:

    string name; // Added member variable to store the player's name
    Player() : name("") {} // Default constructor initializes name to an empty string
    explicit Player(const std::string& playerName) : name(playerName) {} // Constructor that initializes the player's name
    void setName(const std::string& playerName) { // Method to set the player's name

        name = playerName;

    }
    string getName() const { // Method to get the player's name

        return name;

    }
    ~Player() {}

};