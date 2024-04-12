#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <random>
#include <queue>
#include <mutex>
#include <chrono>
#include <regex>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <cctype>
#include "Player.h"
#include "PlayerScore.h"
#include "Categories.h"
#include "Button.h"
#include "ClickableButtonApp.h"

using namespace std;


void PlayGame(Player* playerId, Categories& categories, PhysicsCategory& Cat1, HarryPCategory& Cat2, PennStateCategory& Cat3, PlayerScore* score, mutex& mic, bool isHuman)
{
    int category;
    int diffInput;
    int index;

    // Seed the random number generator with a value based on the current time
    random_device rd;
    mt19937 gen(rd());      // Mersenne Twister pseudo-random number generator
    uniform_int_distribution<int> CatChoices(1, 3);
    uniform_int_distribution<int> diffChoices(1, 4);
    uniform_int_distribution<int> ABCDindex(0, 3);      // used to randomly select index of ABCD answer choices

    static array<char, 4> ABCD = { 'a', 'b', 'c', 'd' };
    static array<int, 4> difficulty = { 100, 200, 300, 400 };

    while (Cat1.NumQuestions > 0 || Cat2.NumQuestions > 0 || Cat3.NumQuestions > 0)
    {
        mic.lock();
        std::this_thread::sleep_for(std::chrono::seconds(1));

        string playerName = playerId->getName();

        cout << playerName << ", please select a category: \n\n1.Physics(" << Cat1.NumQuestions << " questions left)   \n2.Harry Potter(" << Cat2.NumQuestions << " questions left)    \n3.Penn State(" << Cat3.NumQuestions << " questions left)" << endl;
        cout << "User Input: ";

        if (isHuman) { cin >> category; }
        else {
            category = CatChoices(gen);
            cout << category;
        }
        cout << endl << endl;

        // Ensure category is within valid range before proceeding
        while (category < 1 || category > 3 || cin.fail()) {
            cout << "Invalid entry. Please try again." << endl;
            cin.clear(); // Clearing the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clearing the input buffer
            cout << "User Input: ";
            if (isHuman) { cin >> category; }
            cout << endl;
        }
        cout << "Please select a difficulty: \n1. 100    \n2. 200    \n3. 300    \n4. 400" << endl << endl;
        cout << "User Input: ";
        if (isHuman) { cin >> diffInput; }
        else
        {
            diffInput = diffChoices(gen);
            cout << diffInput;
        }
        cout << endl << endl;

        // Ensure diffInput is within valid range before proceeding
        while (diffInput < 1 || diffInput > 4 || cin.fail()) {
            cout << "Invalid entry. Please try again." << endl;
            cin.clear(); // Clearing the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clearing the input buffer
            cout << "User Input: ";
            if (isHuman) { cin >> diffInput; }
            cout << endl << endl;
        }
        if (category == 1) {
            if (Cat1.HasBeenAsked(diffInput - 1)) {
                cout << "\nThat question has already been asked! Please select another question.\n\n";
                mic.unlock();
            }
            else
            {
                Cat1.PrintQuestion(difficulty[diffInput - 1]);
                cout << "User Input: ";
                if (isHuman) {

                    char userAnswer = '\0'; // Initialize userAnswer
                    bool validInput = false; // Flag to track if input is valid
                    while (!validInput) {
                        cout << "User Input: ";
                        cin >> userAnswer;
                        userAnswer = tolower(userAnswer); // Normalize to lowercase for comparison
                        if (userAnswer == 'a' || userAnswer == 'b' || userAnswer == 'c' || userAnswer == 'd') {
                            validInput = true; // Valid input received, exit loop
                        }
                        else {
                            cout << "Invalid entry. Please select either 'A', 'B', 'C', or 'D'." << endl;
                            cin.clear(); // Clear any error flags
                            // Ignore the rest of the line to avoid issues with subsequent inputs

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }

                    Cat1.CheckAnswer(userAnswer, diffInput - 1, *score);        // Adjusted to pass correct index to CheckAnswer
                }
                else {
                    char userAnswer = '\0';

                    // Bot automatically selects a valid answer

                    int index = ABCDindex(gen);
                    userAnswer = ABCD[index];
                    cout << userAnswer << endl;
                    Cat1.CheckAnswer(userAnswer, diffInput - 1, *score);        // Adjusted to pass correct index to CheckAnswer
                }
                cout << endl;
                Cat1.AskedQuestion(diffInput - 1);                          // Sets 'Asked' within the category and difficulty level to true once the question is asked
                cout << "Dollars: " << *score << "\n\n";
                mic.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        else if (category == 2) {
            if (Cat2.HasBeenAsked(diffInput - 1)) {
                cout << "\nThat question has already been asked! Please select another question." << endl;
                mic.unlock();
            }
            else
            {
                Cat2.PrintQuestion(difficulty[diffInput - 1]);
                cout << "User Input: ";
                char userAnswer = '\0';

                if (isHuman) {
                    char userAnswer = '\0'; // Initialize userAnswer
                    bool validInput = false; // Flag to track if input is valid
                    while (!validInput) {

                        cout << "User Input: ";

                        cin >> userAnswer;

                        userAnswer = tolower(userAnswer); // Normalize to lowercase for comparison

                        if (userAnswer == 'a' || userAnswer == 'b' || userAnswer == 'c' || userAnswer == 'd') {

                            validInput = true; // Valid input received, exit loop

                        }

                        else {

                            cout << "Invalid entry. Please select either 'A', 'B', 'C', or 'D'." << endl;

                            cin.clear(); // Clear any error flags

                            // Ignore the rest of the line to avoid issues with subsequent inputs

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        }

                    }

                    Cat2.CheckAnswer(userAnswer, diffInput - 1, *score); // Adjusted to pass correct index to CheckAnswer

                }

                else {

                    // Bot automatically selects a valid answer

                    char userAnswer = '\0'; // Initialize userAnswer

                    int index = ABCDindex(gen);

                    userAnswer = ABCD[index];

                    cout << userAnswer << endl;

                    Cat2.CheckAnswer(userAnswer, diffInput - 1, *score); // Adjusted to pass correct index to CheckAnswer

                }
                cout << endl;
                Cat2.AskedQuestion(diffInput - 1);
                cout << "Dollars: " << *score << "\n\n";
                mic.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        else if (category == 3) {
            if (Cat3.HasBeenAsked(diffInput - 1)) {
                cout << "\nThat question has already been asked! Please select another question." << endl;
                mic.unlock();
            }
            else
            {
                Cat3.PrintQuestion(difficulty[diffInput - 1]);
                cout << "User Input: ";
                char userAnswer = '\0';

                if (isHuman) {

                    char userAnswer = '\0'; // Initialize userAnswer

                    bool validInput = false; // Flag to track if input is valid
                    while (!validInput) {

                        cout << "User Input: ";

                        cin >> userAnswer;

                        userAnswer = tolower(userAnswer); // Normalize to lowercase for comparison



                        if (userAnswer == 'a' || userAnswer == 'b' || userAnswer == 'c' || userAnswer == 'd') {

                            validInput = true; // Valid input received, exit loop

                        }

                        else {

                            cout << "Invalid entry. Please select either 'A', 'B', 'C', or 'D'." << endl;

                            cin.clear(); // Clear any error flags

                            // Ignore the rest of the line to avoid issues with subsequent inputs

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        }

                    }

                    Cat3.CheckAnswer(userAnswer, diffInput - 1, *score); // Adjusted to pass correct index to CheckAnswer

                }

                else {

                    // Bot automatically selects a valid answer

                    char userAnswer = '\0'; // Initialize userAnswer

                    int index = ABCDindex(gen);

                    userAnswer = ABCD[index];

                    cout << userAnswer << endl;

                    Cat3.CheckAnswer(userAnswer, diffInput - 1, *score); // Adjusted to pass correct index to CheckAnswer

                }
                cout << endl;
                Cat3.AskedQuestion(diffInput - 1);
                cout << "Dollars: " << *score << "\n\n";

                mic.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(1));// sleeps so that the next player has a chance to take the microphone
            }
        }
    }
}


void finalJeopardy(PlayerScore* humanScore, PlayerScore* bot1Score, PlayerScore* bot2Score) {
    // checks if score is greater than 0
    if (humanScore->score > 0) {
        string question = "\nRegarded as one of the greatest filmmakers in history, this director created masterpieces like \"The Godfather\" and \"Apocalypse Now.\"";
        string correctAnswer = "Francis Ford Coppola";
        // Ask the question
        cout << "Final Jeopardy Question:" << endl;
        cout << question << endl;
        string finalAnswer;
        string s;
        cin.ignore(); // created so getline can be used after 
        cout << "Your answer: ";
        getline(cin, finalAnswer); //takes the whole line answer instead of first word 

        // Validate the user's answer against the correct answer format - Regex corrects for minor spelling mistakes or capitalization
        if (regex_match(finalAnswer, regex("[Ff][Rr][Aa][Nn][Cc][Ii][Ss] [Ff][Oo][Rr][Dd] [Cc][AaOo][Pp]{1,2}[Oo][Ll]{1,2}[Aa]"))) {
            cout << "Correct! You've won Final Jeopardy!" << endl;
            *humanScore += 1000;
        }
        else {
            cout << "Incorrect. The correct answer is: " << correctAnswer << endl;
        }
    }

    //adds to one bot score to simulate one bot getting the correct answer
    if (bot1Score->score > 0) {
        *bot1Score += 1000;
    }

}

void winnerButton(int humanScore,int bot1Score, int bot2Score) {
    if ((humanScore > bot1Score) && (humanScore > bot1Score)) {// checks if humanscore is highest and gives star if highest
        cout << "Press the star 3 times to exit the game!"<<endl;
        ClickableButtonApp app;
        app.run();
    }
    else {
        cout << "You lose :(" << endl;
    }
}

void leaderBoard(int userPoints, int bot1Points, int bot2Points, string userName, string bot1Name, string bot2Name, queue<string> LeaderboardSpots, map<string, PlayerScore> Leaderboard) {
    //if statements to find who is in the lead and the order of scores
    if (userPoints > bot1Points)
    {
        if (userPoints > bot2Points)
        {
            LeaderboardSpots.push(userName);
            if (bot1Points > bot2Points)
            {
                LeaderboardSpots.push(bot1Name);
                LeaderboardSpots.push(bot2Name);
            }
            else
            {
                LeaderboardSpots.push(bot2Name);
                LeaderboardSpots.push(bot1Name);
            }
        }
        else
        {
            LeaderboardSpots.push(bot2Name);
            LeaderboardSpots.push(userName);
            LeaderboardSpots.push(bot1Name);
        }
    }
    else
    {
        if (bot1Points > bot2Points)
        {
            LeaderboardSpots.push(bot1Name);
            if (userPoints > bot2Points)
            {
                LeaderboardSpots.push(userName);
                LeaderboardSpots.push(bot2Name);
            }
            else
            {
                LeaderboardSpots.push(bot2Name);
                LeaderboardSpots.push(userName);
            }
        }
        else
        {
            LeaderboardSpots.push(bot2Name);
            LeaderboardSpots.push(bot1Name);
            LeaderboardSpots.push(userName);
        }
    }

    cout << "\nLeaderboard:\n\n";
    int place = 1;

    //prints out the leaderboard
    while (LeaderboardSpots.size() != 0)
    {
        cout << place << ") " << LeaderboardSpots.front() << ": " << Leaderboard[LeaderboardSpots.front()] << " dollars" << endl;
        LeaderboardSpots.pop();
        place++;
    }
    

}

string formatLeaderboard(int userPoints, int bot1Points, int bot2Points, string userName, string bot1Name, string bot2Name, queue<string> LeaderboardSpots, map<string, PlayerScore> Leaderboard) {
    //this is the same as the leaderboard function but returns a string so the leaderboard can be saved if wanted
    if (userPoints > bot1Points) {
        if (userPoints > bot2Points) {
            LeaderboardSpots.push(userName);
            if (bot1Points > bot2Points) {
                LeaderboardSpots.push(bot1Name);
                LeaderboardSpots.push(bot2Name);
            }
            else {
                LeaderboardSpots.push(bot2Name);
                LeaderboardSpots.push(bot1Name);
            }
        }
        else {
            LeaderboardSpots.push(bot2Name);
            LeaderboardSpots.push(userName);
            LeaderboardSpots.push(bot1Name);
        }
    }
    else {
        if (bot1Points > bot2Points) {
            LeaderboardSpots.push(bot1Name);
            if (userPoints > bot2Points) {
                LeaderboardSpots.push(userName);
                LeaderboardSpots.push(bot2Name);
            }
            else {
                LeaderboardSpots.push(bot2Name);
                LeaderboardSpots.push(userName);
            }
        }
        else {
            LeaderboardSpots.push(bot2Name);
            LeaderboardSpots.push(bot1Name);
            LeaderboardSpots.push(userName);
        }
    }

    string formattedLeaderboard = "Leaderboard:\n\n";
    int place = 1;

    while (!LeaderboardSpots.empty()) {
        formattedLeaderboard += to_string(place) + ") " + LeaderboardSpots.front() + ": " + to_string(Leaderboard[LeaderboardSpots.front()].getScore()) + " dollars\n";
        LeaderboardSpots.pop();
        place++;
    }
    return formattedLeaderboard;
}

Button ClickableButtonApp::button(0, 0, 0.2f);
int ClickableButtonApp::clickCount = 0;

int main()
{
    Categories categories;

    PhysicsCategory Physics; // Note: AssignQuestions is called within the constructor, so no need to call it here.
    HarryPCategory HarryP;
    PennStateCategory PSU;

    Player humanPlayer("User"), bot1("Wizard 1"), bot2("Wizard 2"); //creates player objects

    string userName = humanPlayer.getName();
    string bot1Name = bot1.getName();
    string bot2Name = bot2.getName();

    PlayerScore humanScore, bot1Score, bot2Score;

    cout << userName << " player Id: " << &humanPlayer << endl;
    cout << bot1Name << " player Id: " << &bot1 << endl;
    cout << bot2Name << " player Id: " << &bot2 << endl;
    cout << endl; //shows ids of the players

    PlayerScore* Scores[3] = { &humanScore, &bot1Score, &bot2Score };
    Player* Players[3] = { &humanPlayer , &bot1 , &bot2 };

    // Linking the player's money bank location with their player id
    map<Player*, PlayerScore*> PlayerMoneyBank;

    for (int i = 0; i < 3; i++)
    {
        PlayerMoneyBank[Players[i]] = Scores[i];
    }

    cout << "Welcome to Trivia Night!" << endl;
    cout << "Directions: When prompted, select a category, then difficulty. You will be given a question and options to answer. \nSelect your answer and you will be awarded/penalized points based on the difficulty level. \nA final question will be given at the end of the game. Type your answer to this question with as few spelling errors as possible.\nYou will be competing against two CPUs. Results will be shown at the end. \nGood Luck!\n\n";

    mutex microphone; //mutex to allow one player to answer at a time

    thread User(PlayGame, Players[0], ref(categories), ref(Physics), ref(HarryP), ref(PSU), Scores[0], ref(microphone), true);
    thread ai1(PlayGame, Players[1], ref(categories), ref(Physics), ref(HarryP), ref(PSU), Scores[1], ref(microphone), false);
    thread ai2(PlayGame, Players[2], ref(categories), ref(Physics), ref(HarryP), ref(PSU), Scores[2], ref(microphone), false);

    User.join();
    ai1.join();
    ai2.join();

    finalJeopardy(Scores[0], Scores[1], Scores[2]); //creates final jeopardy function 

    map<string, PlayerScore> Leaderboard;
    Leaderboard[userName] = *Scores[0];
    Leaderboard[bot1Name] = *Scores[1];
    Leaderboard[bot2Name] = *Scores[2];

    signed int userPoints = humanScore.getScore();
    signed int bot1Points = bot1Score.getScore();
    signed int bot2Points = bot2Score.getScore();

    queue<string> LeaderboardSpots;
    
    leaderBoard(userPoints, bot1Points, bot2Points, userName, bot1Name, bot2Name, LeaderboardSpots, Leaderboard);//displays the leaderboard

    char response;
    //allows user to save scoreboard if wanted
    cout << "Would you like to save the leaderboard? y/n" << endl;
    cin >> response;
    if (response == 'y') {
        ofstream outFile("Leaderboard.txt", ios::out);
        if (!outFile) {
            cerr << "File could not be opened" << endl;
            exit(1);
        }
        outFile << formatLeaderboard(userPoints, bot1Points, bot2Points, userName, bot1Name, bot2Name, LeaderboardSpots, Leaderboard);
    }
    cout << " " << endl;

    winnerButton(userPoints, bot1Points, bot2Points);

    return 0;

}