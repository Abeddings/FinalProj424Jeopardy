#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <map>

using namespace std;
class Categories {

protected:
    vector<int> DiffLevel;
    map<int, vector<string>> QandAs;
    vector<char> Answers;
    vector<char> AnswersUp;

public:
    condition_variable cv;
    Player currentTurn; // Shared among threads, protected by mtx

    int NumQuestions;

    unsigned int answeredQuestions; // Bit field for tracking answered questions
    Categories() : answeredQuestions(0) {

        DiffLevel = { 100, 200, 300, 400 };

        NumQuestions = DiffLevel.size();

    }
    virtual void AssignQuestions(const vector<int>& DiffLevel, const vector<vector<string>>& Questions) {

        for (size_t i = 0; i < DiffLevel.size(); ++i) {
            QandAs[DiffLevel[i]] = Questions[i]; // sets question for each difficulty
        }

    }
    virtual void PrintQuestion(int lev) { //function to print out the question that was selected

        cout << "Questions for difficulty level " << lev << ": " << endl;

        for (const string& question : QandAs[lev]) {

            cout << question;

        }

        cout << endl; // Added for clarity in output

    }
    virtual void CheckAnswer(char choice, int i, PlayerScore& playerScore) // pass 'score' by reference

    {

        // Adjusted to match only one correct answer condition and fixed the index to match DiffLevel

        if (choice == Answers[i] || choice == AnswersUp[i]) {

            cout << "Correct!" << endl;

            playerScore += DiffLevel[i]; // Add points based on difficulty level

        }

        else {

            cout << "Sorry, that is incorrect." << endl;

            playerScore -= DiffLevel[i]; // Subtract points for a wrong answer

        }

    }
    virtual void AskedQuestion(int i) {

        // Calculate the bit position based on difficulty

        unsigned int bitPosition = 1 << i; // Shift 1 to the left 'i' times

        answeredQuestions |= bitPosition; // Mark as answered

        NumQuestions--;

    }
    // Updated method to check if a question has been asked

    bool HasBeenAsked(int i) {

        unsigned int bitPosition = 1 << i; // Calculate bit position

        return answeredQuestions & bitPosition; // Check if the bit is set

    }
    void nextTurn(int playerCount) {

        //currentTurn = (currentTurn + 1) % playerCount;

        cv.notify_all();

    }
    bool isMyTurn(Player playerId) {

        //cv.wait(lock, [this, playerId] { return currentTurn == playerId; });

        return true;

    }
    // Constructor made virtual for proper cleanup in derived classes

    virtual ~Categories() {}

};

//following subclasses are used to store the questions and assign the questions to that subcategory object 
class PhysicsCategory : public Categories {
public:
    PhysicsCategory() {

        AssignQuestions(DiffLevel, {

            {"What is the term for the rate at which velocity changes over time?\nA. Jerk\nB. Force\nC. Acceleration\nD. Position\n"},

            {"299,792,458 meters per second is the speed of what in a vacuum?\nA. Sound\nB. Light\nC. International Space Station\nD. Gravity\n"},

            {"What is the process of splitting an atom called?\nA. Nuclear Fission\nB. Nuclear Fusion\nC. Nuclear Excitation\nD. Covalent Bond\n"},

            {"What is the smallest known particle in the universe?\nA. Quark\nB. Atom\nC. Photon\nD. Electron\n"}

            });

        Answers = { 'c', 'b', 'a', 'a' };

        AnswersUp = { 'C', 'B', 'A', 'A' };
    }

};
class HarryPCategory : public Categories {
public:
    HarryPCategory() {
        AssignQuestions(DiffLevel, {

                {"What is the shape of Harry's scar?\n", "A. Triangle\n", "B. Lightning Bolt\n", "C. X\n", "D. Circle\n"},

                {"Who kills professor Dumbledore?\n", "A. Draco Malfoy\n", "B. Fenrir Greyback\n", "C. Severus Snape\n", "D. Bellatrix Lestrange\n" },

                {"Who poses as Mad-Eye Moody, Harry's 4th year Defense Against the Dark Arts professor?\n", "A. Sirius Black\n", "B. Peter Pettigrew\n", "C. Barty Crouch Jr.\n", "D. Voldemort\n" },

                {"What does the imperius curse do? \n", "A. Controls\n", "B. Kills\n", "C. Tortures\n", "D. Flies\n" }

            });

        Answers = { 'b', 'c', 'c', 'a' };

        AnswersUp = { 'B', 'C', 'C', 'A' };

    }

};
class PennStateCategory : public Categories {
public:
    PennStateCategory() {
        AssignQuestions(DiffLevel, {

            {"What were Penn State's original colors?\nA. Blue and Gold\nB. Black and White\nC. Black and Gold\nD. Pink and Black\n"},

            {"What year was Penn State founded?\nA. 1863\nB. 1850\nC. 1855\nD. 1901\n"},

            {"How many people fit in Beaver Stadium?\nA. 106,572\nB. 102,780\nC. 107,601\nD. 101,915\n"},

            {"What was Penn State's original mascot?\nA. Samson the Squirrel\nB. Giddy the Great Dane\nC. Eddy the Eagle\nD. Old Coaly the Mule\n"}

            });

        Answers = { 'd', 'c', 'a', 'd' };

        AnswersUp = { 'D', 'C', 'A', 'D' };

    }

};