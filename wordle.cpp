#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <fstream>

using namespace std;

#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define GREY    "\033[1;37m"
#define RESET   "\033[0m"

const int MAX_TRIES = 6;
const string WORD_FILE = "words.txt";

string toLower(const std::string& str) {
    string result = str;
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') { 
            c += 'a' - 'A';
        }
    }
    return result;
}

vector<string> loadWords(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    string word;
    while (file >> word) {
        if (word.length() == 5)
            words.push_back(toLower(word));
    }
    return words;
}

void feedback(const string& guess, const string& target) {
    string result = guess;
    vector<bool> matched(5, false);

    for (int i = 0; i < 5; ++i) {
        if (guess[i] == target[i]) {
            cout << GREEN << guess[i] << RESET;
            matched[i] = true;
            result[i] = '*';
        } else {
            result[i] = guess[i];
        }
    }

    cout << "  ";

    for (int i = 0; i < 5; ++i) {
        if (guess[i] == target[i]) continue;
        bool found = false;
        for (int j = 0; j < 5; ++j) {
            if (!matched[j] && guess[i] == target[j]) {
                found = true;
                matched[j] = true;
                break;
            }
        }
        if (found)
            cout << YELLOW << guess[i] << RESET;
        else
            cout << GREY << guess[i] << RESET;
    }
    cout << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    vector<string> words = loadWords(WORD_FILE);
    if (words.empty()) {
        cerr << "Error: could not load words from file '" << WORD_FILE << "'" << endl;
        return 1;
    }

    string target = words[rand() % words.size()];
    string guess;

    cout << "Welcome to Wordle!\nYou have " << MAX_TRIES << " tries to guess the 5-letter word.\n" << endl;

    for (int attempt = 1; attempt <= MAX_TRIES; ++attempt) {
        cout << "Attempt " << attempt << "/" << MAX_TRIES << ": ";
        cin >> guess;
        guess = toLower(guess);

        if (guess.length() != 5) {
            cout << "Please enter a 5-letter word.\n";
            --attempt;
            continue;
        }

        if (find(words.begin(), words.end(), guess) == words.end()) {
            cout << "Not in word list.\n";
            --attempt;
            continue;
        }

        feedback(guess, target);

        if (guess == target) {
            cout << GREEN << "CONGRATULATIONS! YOU HAVE SUCCESSFULLY GUESSED THE WORD!" << RESET << endl;
            return 0;
        }
    }

    cout << RED << "OUT OF TRIES! THE WORD IS: " << target << endl;
    return 0;
}
