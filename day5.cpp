#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>
#include <string>
#include <vector>
#include <cmath>
#include <set>
#include <map>

using namespace std;

void part1();

class Seed {
    
    private:
        map<string, unsigned long long> attributes;
        unsigned long long seed;

    public:
        Seed();

        Seed(unsigned long long s) {
            seed = s;
        }

        void setAttribute(string a, unsigned long long value) {
            attributes[a] = value;
        }

        unsigned long long getAttribute(string a) {
            return attributes[a];
        }

        unsigned long long getSeedNum() {
            return seed;
        }
};

bool compare(Seed l, Seed r) {
    return l.getAttribute("location") < r.getAttribute("location");
}

class Mappings {
    private:
        map<pair<unsigned long long, unsigned long long>, unsigned long long> ranges;

    public:
        void addRange(unsigned long long srcMin, unsigned long long dstMin, unsigned long long length) {
            auto offset = length - 1;
            auto srcMax = srcMin + offset;
            ranges[pair(srcMin, srcMax)] = dstMin;
        }

        unsigned long long getDestFromSrc(unsigned long long src) {
            unsigned long long distance = 0;
            unsigned long long rv = src;

            for (auto [k, destMin] : ranges) {
                if ( (src >= k.first) && (src <= k.second) ) {
                    distance = src - k.first;
                    rv = destMin + distance;        
                    cout << "\tFor source " << src << " diff is " << distance << " and dest is " << rv << endl;
                    return rv;
                }
            }

            cout << "\tNo mapping for source " << src << " so returning same " << rv << endl;
            return src;
        }
};


void part1() {
    ifstream input_file("inputs/day5.txt");

    auto mapDefPattern = regex("([a-z]+)-to-([a-z]+)\\s+map:");
    auto mapLinePattern = regex("(\\d+)\\s+(\\d+)\\s+(\\d+)");
    auto seedLinePattern = regex("seeds:\\s+(.+)");

    string seedLine;
    
    vector<pair<string, string>> propertyMap;
    map<pair<string, string>, Mappings> rm;

    // read in data
    if (input_file.is_open()) {
        string line;
        smatch m;

        string currentMap;
        string currentSrc;
        string currentDest;
        pair<string, string> currentPair;

        while(getline(input_file, line)) {
            // cout << line << endl;

            if ( regex_match(line, m, seedLinePattern) ) {
                seedLine = m[1];
                cout << "seeds: " << seedLine;
            }
            else if ( regex_match(line, m, mapDefPattern) ) {
                currentMap = m[0];
                currentSrc = m[1];
                currentDest = m[2];

                cout << "\t" << currentSrc << " -> " << currentDest << endl;
                
                currentPair = pair(currentSrc, currentDest);

                if ( find(propertyMap.begin(), propertyMap.end(), pair(currentSrc, currentDest)) == propertyMap.end() ) {
                    propertyMap.push_back(currentPair);
                }
            }
            else if ( regex_match(line, m, mapLinePattern) ) {

                auto srcStart = m[2].str();
                auto destStart = m[1].str();
                auto validFor = m[3].str();

                cout << "\t\tsrc  start: " << srcStart << endl;
                cout << "\t\tdest start: " << destStart << endl;
                cout << "\t\tvalid  for: " << validFor << endl;
                cout << "\t\t===" << endl;

                if (! rm.contains(currentPair) ) {
                    rm[currentPair] = Mappings();
                }

                rm[currentPair].addRange(
                    strtoull(srcStart.c_str(), NULL, 10),
                    strtoull(destStart.c_str(), NULL, 10),
                    strtoull(validFor.c_str(), NULL, 10)
                );
            }
            else {
                cout << endl;
            }
        }
    }

    cout << "done building map\n";

    // rm[pair("seed", "soil")].getDestFromSrc(49);
    // rm[pair("seed", "soil")].getDestFromSrc(50);
    // rm[pair("seed", "soil")].getDestFromSrc(51);
    // rm[pair("seed", "soil")].getDestFromSrc(52);
    // rm[pair("seed", "soil")].getDestFromSrc(53);
    // // ...
    // rm[pair("seed", "soil")].getDestFromSrc(96);
    // rm[pair("seed", "soil")].getDestFromSrc(97);
    // rm[pair("seed", "soil")].getDestFromSrc(98);
    // rm[pair("seed", "soil")].getDestFromSrc(99);
    // rm[pair("seed", "soil")].getDestFromSrc(100);
    // rm[pair("seed", "soil")].getDestFromSrc(101);
    // rm[pair("seed", "soil")].getDestFromSrc(102);
    
    vector<Seed> seeds;
    std::regex number("(\\b[0-9]+)\\b");
    for(auto i = sregex_iterator(seedLine.begin(), seedLine.end(), number); i != std::sregex_iterator(); ++i) {
        smatch m = *i;
        unsigned long long seedNum = strtoull(m.str().c_str(), NULL, 10);
        cout << seedNum << endl;
        Seed s = Seed(seedNum);

        for (auto [from, to] : propertyMap) {
            cout << "FROM " << from << " TO " << to << endl;

            if (from == "seed") {
                unsigned long long dest = rm[pair(from, to)].getDestFromSrc(seedNum);
                s.setAttribute(to, dest);
            }
            else {
                unsigned long long src = s.getAttribute(from);
                unsigned long long dest = rm[pair(from, to)].getDestFromSrc(src);
                s.setAttribute(to, dest);
            }
        }
        seeds.push_back(s);
        cout << endl << endl;
    }

    auto m = *min_element(seeds.begin(), seeds.end(), compare);
    
    cout << "Part 1: minimum seed number is " << m.getSeedNum() << " at location " << m.getAttribute("location") << endl;
}

int main() {
    part1();
    return 0;
}