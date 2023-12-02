#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

void part1();
void part2();
int printIndexMap(map<int, int> &tempMap);

int printIndexMap(map<int, int> &tempMap) {
    for (auto & [position, value] : tempMap) {
        cout << "\tposition " << position << " has value " << value << endl;
    }
    cout << "\tfirst and last digit combined: " << tempMap.begin()->second << tempMap.rbegin()->second <<endl;
    string cc = to_string(tempMap.begin()->second) + to_string(tempMap.rbegin()->second);
    return stoi(cc);
}

void part2() {

    vector<regex> words = {
        regex("zero"), regex("one"), regex("two"), regex("three"), regex("four"), 
        regex("five"), regex("six"), regex("seven"), regex("eight"), regex("nine"), 
    };

    int accumulator = 0;
    map<int, int> positionToValue;

    ifstream input_file("inputs/day1_2_ex.txt");
    // ifstream input_file("inputs/day1.txt");
    
    if(input_file.is_open()) {
        string line;

        while(getline(input_file, line)) {
            map<int, int> positionToValue;
            cout << line << endl;

            for(unsigned i = 0; i < words.size(); ++i) {

                auto words_begin = sregex_iterator(line.begin(), line.end(), words[i]);
                auto words_end = sregex_iterator();

                for (auto j = words_begin; j != words_end; ++j) {
                    smatch match = *j;
                    string match_str = match.str();
                    int position = match.position();
                    cout << "\t\t Found match at " << position << endl;
                    positionToValue[match.position()] = i;
                }
            }

            auto first_position = line.find_first_of("0123456789");
            if (first_position != std::string::npos) {
                string first_value = string{line.at(first_position)};
                positionToValue[first_position] = stoi(first_value);
            }

            auto last_position = line.find_last_of("0123456789");
            if (last_position != std::string::npos) {
                string last_value = string{line.at(last_position)};
                positionToValue[last_position] = stoi(last_value);
            }

            accumulator += printIndexMap(positionToValue);

            // cout << endl;
        }
        input_file.close();
        cout << "Total is " << accumulator << endl;
    }
}

void part1() {
    ifstream input_file("inputs/day1.txt");
    int accumulator = 0;

    if (input_file.is_open()) {
        string line;
        while(getline(input_file, line)) {
            string hidden;
            hidden += line.at(line.find_first_of("0123456789"));
            hidden += line.at(line.find_last_of("0123456789"));
            accumulator += stoi(hidden);
        }
        input_file.close();
    }
    
    cout << accumulator << endl;
}

int main() {
    part2();
    part1();
}