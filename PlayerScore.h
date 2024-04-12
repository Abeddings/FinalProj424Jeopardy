#pragma once
#include <iostream>
using namespace std;
class PlayerScore {

public:

    signed int score;

    PlayerScore() : score(0) {}
    // Overload += operator

    PlayerScore& operator+=(int points) { //adds using operator overloading

        this->score += points;

        return *this;

    }

    // Overload -= operator

    PlayerScore& operator-=(int points) { //subtracts using operator overloading

        this->score -= points;

        return *this;

    }
    // Overload << operator to print score

    friend ostream& operator<<(ostream& os, const PlayerScore& ps) {

        os << ps.score;

        return os;

    }
    signed int getScore() const { // Method to get the player's score

        return score;

    }

};