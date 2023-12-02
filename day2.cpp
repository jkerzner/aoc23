#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

void part1();
map<string, int> parseRound(string s);
bool isRoundValid(map<string, int>& round);


map<string, int> pieces{
    {"red", 12},
    {"green", 13},
    {"blue", 14},
};


void part1() {
    //ifstream input_file("inputs/day2_1_ex.txt");
    ifstream input_file("inputs/day2.txt");

    // constants
    string delimeter = ";";
    regex gameIDPattern = regex("Game\\s(\\d+)");
    regex cubePattern = regex("(\\d+)\\s([a-z]+)");

    map<string, int> game{
        {"red", 0},
        {"green", 0},
        {"blue", 0},
    };

    int accumulator = 0;
    size_t start, end;
    int gameID;
    smatch m;

    if (input_file.is_open()) {
        string line, round;

        while(getline(input_file, line)) {
            cout << line << endl;
            
            // reset counters for this game
            game["red"] = 0;
            game["green"] = 0;
            game["blue"] = 0;

            // accumulate each round
            vector<bool> validRounds;

            // trim off the inital `Game \d+:`
            start = line.find(":");
            string gamePrefix = line.substr(0, start);
            regex_match(gamePrefix, m, gameIDPattern);
            gameID = stoi(m[1].str());
            // cout << "\t\t" << gameID << endl;
            start += 2;

            while((end = line.find(delimeter, start)) != string::npos) {
                round = line.substr(start, end - start);
                start = end + 2;
                cout << "\t ROUND '" << round << "' " << endl;
                auto r = parseRound(round);
                bool b = isRoundValid(r);
                validRounds.emplace_back(b);
                cout << "\t\t... " << b << endl;
            }

            // handle the last round
            round = line.substr(start, line.length());
            cout << "\t ROUND '" << round << "' (terminal) " << endl;
            auto r = parseRound(round);
            bool b = isRoundValid(r);
            validRounds.emplace_back(b);
            cout << "\t\t... " << b << endl;

            if (all_of(validRounds.begin(), validRounds.end(), [](bool x) {return x == true;})) {
                cout << "\t game " << gameID << " is valid" << endl;
                accumulator += gameID;
            }
            else {
                cout << "\t game " << gameID << " is NOT OK" << endl;
            }

            cout << endl;
        } //end each line


        input_file.close();
    }
    
    cout << accumulator << endl;
}

bool isRoundValid(map<string, int>& round) {
    for (auto& [color, count] : round) {
        if (pieces[color] < count) {
            return false;
        }
    }
    return true;
}

map<string, int> parseRound(string s) {
    regex cubePattern = regex("(\\d+)\\s([a-z]+)");
    map<string, int> rv;

    auto colors_begin = sregex_iterator(s.begin(), s.end(), cubePattern);
    auto colors_end = sregex_iterator();

    // cout << "There are " << distance(colors_begin, colors_end) << " matches\n";
    for(auto i = colors_begin; i != colors_end; i++) {
        smatch m = *i;
        if (m.ready()) {
            // cout << m.size() << " " << m.str(1) << "         '" << m.str(2) << "' " << endl;
            rv[m.str(2)] = stoi(m.str(1));
        }
    }
    return rv;
}


int main() {
    part1();
}