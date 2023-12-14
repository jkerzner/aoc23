#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int main() {

    ifstream input_file("inputs/day6.txt");

    vector<int> times;
    vector<int> goals;

    int comboProduct = 1;

    auto linePattern = regex("([A-Za-z]+):(.*)");
    auto numPattern = regex("\\b(\\d+)\\b");

    // read in data
    if (input_file.is_open()) {
        string line;
        smatch m;
        while(getline(input_file, line)) {
            if ( regex_match(line, m, linePattern)) {
                auto row = m[1].str();
                auto nums = m[2].str();
                smatch n;


                auto numbers_begin = sregex_iterator(nums.begin(), nums.end(), numPattern);
                auto numbers_end = sregex_iterator();
                for (auto j = numbers_begin; j != numbers_end; j++) {
                    smatch jm = *j;
                    if (jm.ready()) {
                        if (m[1] == "Time") {
                            times.push_back(stoi(jm.str()));
                        }
                        else if(m[1] == "Distance") {
                            goals.push_back(stoi(jm.str()));
                        }                        
                    }
                }
            }
        }
    } // done reading

    for(int i = 0; i < times.size(); ++i){

        double a = -1;
        double b = times[i];
        double c = -1 * goals[i];

        double minTime = ((-b) + sqrt( (pow(b, 2.0) - (4.0 * a * c)) )) / (2.0 * a);
        double maxTime = ((-b) - sqrt( (pow(b, 2.0) - (4.0 * a * c)) )) / (2.0 * a);
        
        //fractional?
        int realMin, realMax;
        double intPart, fractionPart;
        
        fractionPart = std::modf(minTime, &intPart);
        if (fractionPart == 0.0) {
            realMin = intPart + 1;  // implicit cast to int
        }
        else if (fractionPart > 0.0) {
            realMin = ceil(minTime);
        }
        
        fractionPart = std::modf(maxTime, &intPart);
        if (fractionPart == 0.0) {
            realMax = intPart - 1;  // implicit cast to int
        }
        else if (fractionPart > 0.0) {
            realMax = floor(maxTime);
        }

        cout << "Solution range for " << i << ": " << realMin << " to " << realMax << endl;
        cout << "\tSolution count: " << realMax - realMin + 1 << endl;
        comboProduct *= (realMax - realMin + 1);
    }

    cout << "Combo product is " << comboProduct << endl;

    return 0;
}