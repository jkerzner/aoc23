#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

static int rows, columns, line_length, max_row_ix, max_col_ix;

void part1();

vector<pair<int, int>> coordinates_to_check(int row, int start_column, int length) {

    vector<pair<int, int>> to_check;

    int curr_col;
    for(int i = 0; i < length; i++) {
        curr_col = start_column + i;
        cout << "\t\t(" << row << "," << curr_col << ") ";

        if (i == 0) {
            to_check.push_back(pair<int, int>{row - 1, curr_col - 1});
            to_check.push_back(pair<int, int>{row + 0, curr_col - 1});
            to_check.push_back(pair<int, int>{row + 1, curr_col - 1});
        }

        if (i == (length - 1)) {
            to_check.push_back(pair<int, int>{row - 1, curr_col + 1});
            to_check.push_back(pair<int, int>{row + 0, curr_col + 1});
            to_check.push_back(pair<int, int>{row + 1, curr_col + 1});
        }

        // always do the row above and below the current position
        to_check.push_back(pair<int, int>{row - 1, curr_col});
        to_check.push_back(pair<int, int>{row + 1, curr_col});
    }
    cout << endl;

    to_check.erase(
        remove_if(
            to_check.begin(),
            to_check.end(),
            [](pair<int,int> coord) {
                if ( coord.first < 0 ) { return true; } // rows less than 0
                if ( coord.second < 0) { return true; } // columns less than 0
                if ( coord.first > max_row_ix ) { return true; } // rows beyond the bottom
                if ( coord.second > max_col_ix) { return true; } // columns beyond the right
                return false;
            }
        ),
        to_check.end()
    );

    return to_check;
}

void part1() {
    ifstream input_file("inputs/day3.txt");
    vector<string> lines;

    // read in data
    if (input_file.is_open()) {
        string line;
        while(getline(input_file, line)) {
            lines.push_back(line);
        }
    }
    cout << "Line count: " << lines.size() << endl;
    rows = lines.size();
    columns = lines[0].length();
    cout << "\tLength of line: " << columns << endl;

    // set max positions
    max_row_ix = rows - 1;
    max_col_ix = columns - 1;

    regex numPattern = regex("(\\d+)");
    regex punctPattern = regex("[^.a-z0-9]{1}");
    int accumulator = 0;

    for(size_t row = 0; row < lines.size(); row++) {
        string line = lines[row];
        cout << line << endl;
        auto numbers_begin = sregex_iterator(line.begin(), line.end(), numPattern);
        auto numbers_end = sregex_iterator();

        for(auto i = numbers_begin; i != numbers_end; i++) {
            smatch m = *i;
            

            int col, number_length, found_number;
            if (m.ready()) {
                col = m.position();
                number_length = m.length();
                found_number = stoi(m.str(0));
            }

            cout << "\tFOUND NUMBER: " << found_number;
            cout << " at row " << row << " column " << col;
            cout << " length " << number_length;
            cout << endl;

            auto coords = coordinates_to_check(row, col, number_length);

            bool include_number = false;
            for (auto coord : coords) {
                auto adjacent_character = string{lines[coord.first][coord.second]};
                if ( regex_match(adjacent_character, punctPattern)) {
                    include_number = true;
                }
            }

            if (include_number){
                cout << "\t\t>>> this number has adjacent punctuation: " << m.str(0) << endl;
                accumulator += found_number;
            } else {
                cout << "\t\t>>> this number is EXCLUDED: " << m.str(0) << endl;
            }
        } // end foreach number on line

        cout << endl;
    } // end foreach line

    cout << "Sum of all part numbers: " << accumulator;
}

int main() {
    part1();
    return 0;
}