#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ranges>
#include <unordered_set>

using namespace std;

const unordered_set<unsigned> fourOfAKind = {4, 1};
const unordered_set<unsigned> fullHouse = {3, 2};
const unordered_set<unsigned> threeOfAKind = {3, 1, 1};
const unordered_set<unsigned> twoPair = {2, 2, 1};

int computeHandType();


int main() {
    vector<string> hand = {"A", "2", "3", "A", "4"};
    map<string, unsigned> cardinalities;
    
    string builder = "";

    for ( auto& c : hand) {
        cardinalities[c] += 1;

        if (c == "A") {
            c = "e";
        }
        else if(c == "K") {
            c = "d";
        }
        else if(c == "Q") {
            c = "c";
        }
        else if(c == "J") {
            c = "b";
        }
        else if(c == "T") {
            c = "a";
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

    unordered_set<unsigned> cardinalityValues;
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

    return 0;
}