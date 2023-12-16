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


string buildCardinalityMap(string hand, map<char, unsigned>& cardinalities);

unsigned computeHandStrength(string hand, map<char, unsigned int>& cardinalities);

unsigned computeJokerHandStrength(string hand);

unsigned computeFixedHandStrength(string hand);

// end headers

string buildCardinalityMap(string hand, map<char, unsigned>& cardinalities) {
    cout << "Hand is " << hand << endl;
    
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
    return builder;
}

unsigned computeFixedHandStrength(string hand){
    map<char, unsigned> cardinalities;
    string hexHand = buildCardinalityMap(hand, cardinalities);
    return computeHandStrength(hexHand, cardinalities);
}


unsigned computeHandStrength(string hexHand, map<char, unsigned int>& cardinalities) {
    unsigned cardOrderScore = stoi(hexHand, nullptr, 16);
    cout << hexHand << " is card level " << cardOrderScore << endl;

    unsigned handScore = 0x0;
    if (cardinalities.size() == 1){
        cout << "five of a kind" << endl;
        handScore = 0x700000;
    }
    else if (cardinalities.size() == 4) {
        cout << "one pair" << endl;
        handScore = 0x200000;
    }
    else if (cardinalities.size() == 5) {
        cout << "high card" << endl;
        handScore = 0x100000;
    }

    unordered_multiset<unsigned> cardinalityValues;
    for(auto const &[k, v] : cardinalities){
        cardinalityValues.emplace(v);
    }

    if (cardinalityValues == fourOfAKind) {
        cout << "four of a kind" << endl;
        handScore = 0x600000;
    }
    else if (cardinalityValues == fullHouse) {
        cout << "full house" << endl;
        handScore = 0x500000;
    }
    else if (cardinalityValues == threeOfAKind) {
        cout << "three of a kind" << endl;
        handScore = 0x400000;
    }
    else if (cardinalityValues == twoPair) {
        cout << "two pair" << endl;
        handScore = 0x300000;
    }

    cout << "Hand complete rank " << handScore + cardOrderScore << endl;
    cout << endl;
    return handScore + cardOrderScore;
}


unsigned computeJokerHandStrength(string hand) {

    // Jokers are mapped to 0xb
    auto jokerCount = ranges::count(hand, 'J');

    string hexHand;
    map<char, unsigned> cardinalities;

    if (jokerCount == 0) {
        string hexHand = buildCardinalityMap(hand, cardinalities);
        return computeHandStrength(hexHand, cardinalities);
    }
    else if(jokerCount == 5) {
        string hexHand = buildCardinalityMap("AAAAA", cardinalities);
        return computeHandStrength(hexHand, cardinalities);
    }
    return 0;
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

                auto strength = computeFixedHandStrength(hand);
                //auto strength = computeJokerHandStrength(hand);
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