#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>
#include <string>
#include <vector>
#include <cmath>
#include <set>

using namespace std;

void part1();

void part1() {
    ifstream input_file("inputs/day4.txt");

    unordered_map<int, pair<vector<int>, vector<int>>> cards;

    int totalScore = 0;

    auto linePattern = regex("Card\\s+(\\d+):\\s+(.*?)\\|(.*?)");
    auto numPattern = regex("\\b(\\d+)\\b");

    // read in data
    if (input_file.is_open()) {
        string line;

        smatch m;
        while(getline(input_file, line)) {
            cout << line << endl;

            if ( regex_match(line, m, linePattern) ) {
                for(int i = 0; i < m.size(); ++i) {
                    // cout << "\t" << i << ": " << m[i] << endl;
                    // group 0 => whole line
                    // group 1 => card #
                    // group 2 => winning numbers
                    // group 3 => hand
                }

                int card = stoi(m[1].str());

                cout << "\tWinning: ";
                string winning = m[2].str();
                auto numbers_begin = sregex_iterator(winning.begin(), winning.end(), numPattern);
                auto numbers_end = sregex_iterator();
                for (auto j = numbers_begin; j != numbers_end; j++) {
                    smatch jm = *j;
                    if (jm.ready()) {
                        cout << jm.str(0) << " ";
                        cards[card].first.emplace_back(stoi(jm.str(0)));
                    }
                }
                cout << endl;

                cout << "\tHand: ";
                string hand = m[3].str();
                numbers_begin = sregex_iterator(hand.begin(), hand.end(), numPattern);
                numbers_end = sregex_iterator();
                for (auto j = numbers_begin; j != numbers_end; j++) {
                    smatch jm = *j;
                    if (jm.ready()) {
                        cout << jm.str(0) << " ";
                        cards[card].second.emplace_back(stoi(jm.str(0)));
                    }
                }
                cout << endl;

                cout <<"\tSorted Winning ";
                sort(cards[card].first.begin(),  cards[card].first.end());
                sort(cards[card].second.begin(), cards[card].second.end());
                for (auto x : cards[card].first) {
                    cout << x << " ";
                }
                cout << endl;

                cout <<"\tSorted Hand ";
                for (auto x : cards[card].second) {
                    cout << x << " ";
                }
                cout << endl;

                vector<int> overlap;
                set<int> winning_set(
                    cards[card].first.begin(),
                    cards[card].first.end()
                );


                for (auto x : cards[card].second) {
                    bool is_in = winning_set.find(x) != winning_set.end();
                    if (is_in == true) {
                        overlap.push_back(x);
                    }
                }
 
                cout << "\t\t";
                for (int n : overlap) {
                    cout << n << " ";
                }
                cout << endl;

                cout << "\t\tScore: ";
                if (overlap.size() > 0) {
                    cout << pow(2, overlap.size() - 1);
                    totalScore += pow(2, overlap.size() - 1);
                }
                cout << endl;

            }
        } // end each line in file
    }

    cout << "Total score is " << totalScore << endl;
}

int main() {
    part1();
    return 0;
}