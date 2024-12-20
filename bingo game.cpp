#include <iostream>
#include <vector>
#include <cstdlib>   // For srand() and rand()
#include <ctime>     // For time()
#include <algorithm> // For std::random_shuffle
#include <sstream>   // For std::stringstream

using namespace std;

// Represents a Bingo card
struct BingoCard {
    vector<int> numbers;
};

// ANSI 
const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string BLUE = "\033[34m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string WHITE = "\033[37m";

// Function to generate a random number
int generateRandomNumber(int min, int max) {
    return min + (rand() % (max - min + 1));
}

// Function to create a Bingo card
BingoCard createBingoCard() {
    BingoCard card;
    vector<int> possibleNumbers;
    for (int i = 1; i <= 25; ++i) {
        possibleNumbers.push_back(i);
    }
    random_shuffle(possibleNumbers.begin(), possibleNumbers.end());

    for (int i = 0; i < 25; ++i) {
        card.numbers.push_back(possibleNumbers[i]);
    }
    return card;
}

// Function to draw a number and mark it on the cards
void drawNumberAndMark(BingoCard& player1Card, BingoCard& player2Card, int number) {
    for (int i = 0; i < player1Card.numbers.size(); ++i) {
        // Mark the number as drawn for Player 1
        if (player1Card.numbers[i] == number) {
            player1Card.numbers[i] = 0;
        }
        // Mark the number as drawn for Player 2
        if (player2Card.numbers[i] == number) {
            player2Card.numbers[i] = 0;
        }
    }
}

// Function to check if a player has achieved Bingo
int countBingos(const BingoCard& card) {
    int bingoCount = 0;

    // Check rows
    for (int i = 0; i < 5; ++i) {
        bool isBingo = true;
        for (int j = 0; j < 5; ++j) {
            if (card.numbers[i * 5 + j] != 0) {
                isBingo = false;
                break;
            }
        }
        if (isBingo) bingoCount++;
    }

    // Check columns
    for (int i = 0; i < 5; ++i) {
        bool isBingo = true;
        for (int j = 0; j < 5; ++j) {
            if (card.numbers[j * 5 + i] != 0) {
                isBingo = false;
                break;
            }
        }
        if (isBingo) bingoCount++;
    }

    // Check diagonals
    bool diagonalBingo1 = true, diagonalBingo2 = true;
    for (int i = 0; i < 5; ++i) {
        if (card.numbers[i * 5 + i] != 0) {
            diagonalBingo1 = false;
        }
        if (card.numbers[(i + 1) * 4] != 0) {
            diagonalBingo2 = false;
        }
    }
    if (diagonalBingo1) bingoCount++;
    if (diagonalBingo2) bingoCount++;

    return bingoCount;
}

// Function to display the Bingo card with color
void displayCard(const BingoCard& card, const string& color) {
    // Display BINGO header with aligned spacing
    cout << YELLOW << " B       I       N       G       O" << RESET << endl;

    for (size_t i = 0; i < card.numbers.size(); ++i) {
        if (card.numbers[i] != 0) {
            stringstream ss;
            ss << card.numbers[i];
            cout << color << ss.str() << RESET << "\t";
        } else {
            // Display the cross in white
            cout << WHITE << "X" << RESET << "\t";
        }
        if ((i + 1) % 5 == 0) {
            cout << endl;
        }
    }
}

// Function to print special prompts
void printPrompt(const string& prompt) {
    cout << YELLOW << prompt << RESET << endl;
}

int main() {
    // Seed the random number generator with the current time
    srand(time(0));

    // Ask user for game mode
    bool gameMode;
    cout << "Welcome to BINGO!\nEnter 1 for Player vs Player, or 0 for Player vs Computer: ";
    cin >> gameMode;

    string player1Name, player2Name;
    if (gameMode) {
        cout << "Enter Player 1's name: ";
        cin >> player1Name;
        cout << "Enter Player 2's name: ";
        cin >> player2Name;
    } else {
        cout << "Enter Player's name: ";
        cin >> player1Name;
        player2Name = "Computer";
    }

    BingoCard player1Card = createBingoCard();
    BingoCard player2Card = createBingoCard();

    int currentPlayer = 1; // Player 1 starts
    int player1Bingos = 0;
    int player2Bingos = 0;

    while (true) {
        BingoCard& currentCard = (currentPlayer == 1) ? player1Card : player2Card;

        // Draw a number based on game mode
        int drawnNumber;
        if (gameMode) {
            if (currentPlayer == 1) {
                cout << player1Name << ", enter a number to draw (1-25): ";
                cin >> drawnNumber;
            } else {
                cout << player2Name << ", enter a number to draw (1-25): ";
                cin >> drawnNumber;
            }
        } else {
            if (currentPlayer == 1) {
                cout << player1Name << ", enter a number to draw (1-25): ";
                cin >> drawnNumber;
            } else {
                drawnNumber = generateRandomNumber(1, 25); // Computer auto-draw
                cout << player2Name << " (Computer) draws: " << drawnNumber << endl;
            }
        }

        // Input validation
        while (drawnNumber < 1 || drawnNumber > 25) {
            cout << "Invalid number. Please enter a number between 1 and 25: ";
            cin >> drawnNumber;
        }

        drawNumberAndMark(player1Card, player2Card, drawnNumber);

        cout << player1Name << " and " << player2Name << " - Drawn Number: " << drawnNumber << endl;

        // Display both players' updated Bingo cards
        cout << player1Name << "'s Card:\n";
        displayCard(player1Card, GREEN);
        cout << "--------------------------" << endl;
        cout << player2Name << "'s Card:\n";
        displayCard(player2Card, (gameMode ? BLUE : RED));
        cout << "--------------------------" << endl;

        // Check for Bingo
        player1Bingos = countBingos(player1Card);
        player2Bingos = countBingos(player2Card);

        if (player1Bingos > 0) {
            cout << player1Name << " has achieved Bingo " << player1Bingos << " time(s)!" << endl;
        }
        if (player2Bingos > 0) {
            cout << player2Name << " has achieved Bingo " << player2Bingos << " time(s)!" << endl;
        }

        // Check for special prompts during the game
        if (player1Bingos == 1 || player2Bingos == 1) {
            printPrompt("Racing to Bingo!\nWho will score the next Bingo? The tension builds as the numbers roll in!");
        }

        // Check for victory
        if (player1Bingos >= 3) {
            printPrompt("Victory in Sight!\nThe competition is intense! Will this round decide the champion?");
            cout << player1Name << " wins the game with 3 Bingos! Congratulations!" << endl;
            break;
        }
        if (player2Bingos >= 3) {
            printPrompt("Victory in Sight!\nThe competition is intense! Will this round decide the champion?");
            cout << player2Name << " wins the game with 3 Bingos! Congratulations!" << endl;
            break;
        }

        // Switch players
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    return 0;
}
 
