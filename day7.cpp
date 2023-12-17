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

unsigned computePart1HandStrength(string hand);

template <typename T>
vector<vector<T>> comboBuilder(const vector<T>& input, unsigned len);

// end headers

template <typename T>
vector<vector<T>> comboBuilder(const vector<T>& candidates, unsigned len) {
    // for(auto i = 0; i < len; ++i) {
    //     cout << "\t";
    // }
    // cout << len << ": ";
    // for(auto i : candidates) {
    //     cout << i;
    // }
    // cout << endl;
    vector<vector<T>> rv;

    // base case add single card
    if (len == 1) {
        for (auto i : candidates) {
            // cout << "base case " << i << endl;
            rv.push_back( {i} );
        }
        return rv;
    }

    vector<vector<T>> shorterCombos = comboBuilder(candidates, len - 1);

    // 2 or 3 cards
    for (auto i : candidates) {
        for (vector<T> sv : shorterCombos) {
            sv.push_back(i);
            rv.push_back(sv);
        }
    }
    // for (auto v : rv) {
    //     for(auto i : v) {
    //         cout << i << " ";
    //     }
    // cout << endl;
    // }
    return rv;
}

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

unsigned computePart1HandStrength(string hand){
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

    cout << "\t[JOKER] " << hand << endl;

    string hexHand;
    map<char, unsigned> cardinalities;

    if (jokerCount == 0) {
        cout << "\t[JOKER] " << "nothing to remap" << endl;
        string hexHand = buildCardinalityMap(hand, cardinalities);
        return computeHandStrength(hexHand, cardinalities);
    }
    else if(jokerCount == 5) {
        cout << "\t[JOKER] " << "was all J so all A" << endl;
        string hexHand = buildCardinalityMap("AAAAA", cardinalities);
        return computeHandStrength(hexHand, cardinalities);
    }

    vector<char> nonJokerCards;

    for(char c : hand) {
        if (c != 'J') {
            nonJokerCards.push_back(c);
        }
    }

    auto combos = comboBuilder(nonJokerCards, jokerCount);
    unsigned maxScore = 0;
    string maxHand;

    for (auto combo : combos) {
        string testCase = "";
        unsigned k = 0;

        for(auto i = 0; i < hand.size(); ++i) {
            if (hand[i] != 'J') {
                testCase.push_back(hand[i]);
                continue;
            }
            else {
                testCase.push_back(combo[k]);
                ++k;
            }
        }
        cardinalities.clear();
        auto testHexHand = buildCardinalityMap(testCase, cardinalities);

        // TODO: original hand breaks ties

        auto handStrength = computeHandStrength(testHexHand, cardinalities);

        if (handStrength > maxScore) {
            maxScore = handStrength;
            maxHand = testCase;
        }
    }
    cout << "For original hand " << hand << " the best combo is " << hand << " ===> " << maxHand << " with stength " << maxScore << endl;
    return maxScore;
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

                // auto strength = computeFixedHandStrength(hand);
                auto strength = computeJokerHandStrength(hand);
                if (result.contains(strength)) {
                    cout << "duplicate strength detected!" << endl;
                    exit(1);
                }
                result[strength] = pair(hand, stoi(bid));
            }
        }
    }

    unsigned long long winnings = 0;
    unsigned rank = 1;

    for (auto i = result.begin(); i != result.end(); ++i) {
        cout << rank << " " << i->second.first << " " << i->second.second << endl;
        winnings += (rank * i->second.second);
        rank++;
    }

    cout << "Total winnings are " << winnings << endl;

    return 0;
}