#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ranges>
#include <regex>
#include <unordered_set>


using namespace std;

const unordered_multiset<unsigned> fourOfAKind = {4, 1};
const unordered_multiset<unsigned> fullHouse = {3, 2};
const unordered_multiset<unsigned> threeOfAKind = {3, 1, 1};
const unordered_multiset<unsigned> twoPair = {2, 2, 1};

unsigned computeHandStrenght();


unsigned computeHandString(string hand) {

    cout << "Hand is " << hand << endl;
    map<char, unsigned> cardinalities;
    
    string builder = "";

    for ( auto& c : hand) {
        cardinalities[c] += 1;

        if (c == 'A') {
            c = 'e';
        }
        else if(c == 'K') {
            c = 'd';
        }
        else if(c == 'Q') {
            c = 'c';
        }
        else if(c == 'J') {
            c = 'b';
        }
        else if(c == 'T') {
            c = 'a';
        }
        builder += c;
    }

    cout << "hex builder for hand is 0x" << builder << endl;
    unsigned cardOrderScore = stoi(builder, nullptr, 16);
    cout << builder << " is card level " << cardOrderScore << endl;

    unsigned handScore = 0x0;
    if (cardinalities.size() == 1){
        cout << "five of a kind" << endl;
        handScore = 0x700000;
        // todo - these all need to return
    }
    else if (cardinalities.size() == 4) {
        cout << "one pair" << endl;
        handScore = 0x200000;
        // todo - these all need to return
    }
    else if (cardinalities.size() == 5) {
        cout << "high card" << endl;
        handScore = 0x100000;
        // todo - these all need to return
    }

    unordered_multiset<unsigned> cardinalityValues;
    for(auto const &[k, v] : cardinalities){
        cardinalityValues.emplace(v);
    }

    if (cardinalityValues == fourOfAKind) {
        cout << "four of a kind" << endl;
        handScore = 0x600000;
        // todo - these all need to return
    }
    else if (cardinalityValues == fullHouse) {
        cout << "full house" << endl;
        handScore = 0x500000;
        // todo - these all need to return
    }
    else if (cardinalityValues == threeOfAKind) {
        cout << "three of a kind" << endl;
        handScore = 0x400000;
        // todo - these all need to return
    }
    else if (cardinalityValues == twoPair) {
        cout << "two pair" << endl;
        handScore = 0x300000;
        // todo - these all need to return
    }

    cout << "Hand complete rank " << handScore + cardOrderScore << endl;
    cout << endl;
    return handScore + cardOrderScore;
}


int main() {
    auto linePattern = regex("([A-Za-z0-9]+)\\s+(\\d+)");

    ifstream input_file("inputs/day7.txt");
    
    map<unsigned, pair<string, unsigned>> result;

    if (input_file.is_open()) {
        string line;
        smatch m;
        while(getline(input_file, line)) {
            if ( regex_match(line, m, linePattern)) {
                auto hand = m[1].str();
                auto bid = m[2].str();
                auto strength = computeHandString(hand);
                result[strength] = pair(hand, stoi(bid));
            }
        }
    }

    unsigned long long winnings = 0;
    unsigned rank = 1;

    for (auto i = result.begin(); i != result.end(); ++i) {
        // cout << rank << " " << i->second.first << " " << i->second.second << endl;
        winnings += (rank * i->second.second);
        rank++;
    }

    cout << "Total winnings are " << winnings << endl;

    return 0;
}