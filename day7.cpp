#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ranges>
#include <regex>
#include <unordered_set>
#include <iomanip>

using namespace std;

const unordered_multiset<unsigned> fourOfAKind = {4, 1};
const unordered_multiset<unsigned> fullHouse = {3, 2};
const unordered_multiset<unsigned> threeOfAKind = {3, 1, 1};
const unordered_multiset<unsigned> twoPair = {2, 2, 1};


string buildCardinalityMap(string hand, map<char, unsigned>& cardinalities);

unsigned computeHandStrength(string hand, map<char, unsigned int>& cardinalitie, string& resultType);

unsigned computeJokerHandStrength(string hand, string& resultType);

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
    // cout << "Hand is " << hand << endl;
    
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
            c = '1';
        }
        else if(c == 'T') {
            c = 'a';
        }
        builder += c;
    }

   // cout << "hex builder for hand is 0x" << builder << endl;
    return builder;
}

// unsigned computePart1HandStrength(string hand){
//     map<char, unsigned> cardinalities;
//     string hexHand = buildCardinalityMap(hand, cardinalities);
//     return computeHandStrength(hexHand, cardinalities);
// }


unsigned computeHandTypeRank(map<char, unsigned int>& cardinalities, string& testTypeResult) {

    unsigned handScore = 0x0;
    if (cardinalities.size() == 1){
        testTypeResult = "five of a kind";
        handScore = 0x700000;
    }
    else if (cardinalities.size() == 4) {
        testTypeResult = "one pair";
        handScore = 0x200000;
    }
    else if (cardinalities.size() == 5) {
        testTypeResult = "high card";
        handScore = 0x100000;
    }

    unordered_multiset<unsigned> cardinalityValues;
    for(auto const &[k, v] : cardinalities){
        cardinalityValues.emplace(v);
    }

    if (cardinalityValues == fourOfAKind) {
        testTypeResult = "four of a kind";
        handScore = 0x600000;
    }
    else if (cardinalityValues == fullHouse) {
        testTypeResult = "full house";
        handScore = 0x500000;
    }
    else if (cardinalityValues == threeOfAKind) {
        testTypeResult = "three of a kind";
        handScore = 0x400000;
    }
    else if (cardinalityValues == twoPair) {
        testTypeResult = "two pair";
        handScore = 0x300000;
    }

    return handScore;
}

unsigned computeHandStrength(string hexHand, map<char, unsigned int>& cardinalities, string& testTypeResult) {
    unsigned cardOrderScore = stoi(hexHand, nullptr, 16);
    // cout << hexHand << " is card level " << cardOrderScore << endl;

    unsigned handScore = 0x0;
    if (cardinalities.size() == 1){
        testTypeResult = "five of a kind";
        handScore = 0x700000;
    }
    else if (cardinalities.size() == 4) {
        testTypeResult = "one pair";
        handScore = 0x200000;
    }
    else if (cardinalities.size() == 5) {
        testTypeResult = "high card";
        handScore = 0x100000;
    }

    unordered_multiset<unsigned> cardinalityValues;
    for(auto const &[k, v] : cardinalities){
        cardinalityValues.emplace(v);
    }

    if (cardinalityValues == fourOfAKind) {
        testTypeResult = "four of a kind";
        handScore = 0x600000;
    }
    else if (cardinalityValues == fullHouse) {
        testTypeResult = "full house";
        handScore = 0x500000;
    }
    else if (cardinalityValues == threeOfAKind) {
        testTypeResult = "three of a kind";
        handScore = 0x400000;
    }
    else if (cardinalityValues == twoPair) {
        testTypeResult = "two pair";
        handScore = 0x300000;
    }

    cout << "Hand complete rank " << handScore + cardOrderScore << endl;
    cout << endl;
    return handScore + cardOrderScore;
}


unsigned computeJokerHandStrength(string hand, string& resultType) {

    cout << "========== TOP ==========" << endl;
    // Jokers are mapped to 0xb
    auto jokerCount = ranges::count(hand, 'J');

    map<char, unsigned> cardinalities;
    string hexHand = buildCardinalityMap(hand, cardinalities);
    auto originalHandStrength = computeHandStrength(hexHand, cardinalities, resultType);    

    // short circuit to reduce the search space
    if (jokerCount == 0) {
        cout << "Unchanged " << hand << " with strength " << originalHandStrength << endl;
        cout << "========== BOTTOM ==========" << endl;
        return originalHandStrength;
    }
    else if(jokerCount == 5) {
        string hexHand = buildCardinalityMap("AAAAA", cardinalities);
        cout << "All jokers " << hand << " to " << "AAAAA with strength " << originalHandStrength << endl;
        resultType = "five of a kind";
        cout << "========== BOTTOM ==========" << endl;
        return originalHandStrength;
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
    string maxHandType;

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
        map<char, unsigned> testCardinalities;
        auto testHexHand = buildCardinalityMap(testCase, testCardinalities);

        string testTypeResult;
        unsigned testHandStrength = 0;
        testHandStrength += computeHandTypeRank(testCardinalities, testTypeResult);
        testHandStrength += stoi(hexHand, nullptr, 16);

        if (testHandStrength > maxScore) {
            maxScore = testHandStrength;
            maxHand = testCase;
            maxHandType = testTypeResult;
        }
    }
    resultType = maxHandType;
    cout << "For original hand " << hand << " the best combo is " << maxHandType << " ===> " << maxHand << " with stength " << maxScore << endl;
    cout << "========== BOTTOM ==========" << endl;
    return maxScore;
}

int main() {
    auto linePattern = regex("([A-Za-z0-9]+)\\s+(\\d+)");

    ifstream input_file("inputs/day7.txt");
    
    map<unsigned, pair<pair<string, string>, unsigned>> result;

    if (input_file.is_open()) {
        string line;
        smatch m;
        while(getline(input_file, line)) {
            if ( regex_match(line, m, linePattern)) {
                auto hand = m[1].str();
                auto bid = m[2].str();

                string resultType;
                // auto strength = computeFixedHandStrength(hand);
                auto strength = computeJokerHandStrength(hand, resultType);
                cout << endl << endl ;
                if (result.contains(strength)) {
                    cout << "duplicate strength " << strength <<  " detected!" << endl;
                    cout << "existing: " << result[strength].first.first << " with bid " << bid << endl;
                    cout << "new: " << hand << " with bid " << bid << endl;
                    exit(1);
                }
                result[strength] = pair(pair(hand, resultType), stoi(bid));
            }
        }
    }

    unsigned long long winnings = 0;
    unsigned rank = 1;

    for (auto i = result.begin(); i != result.end(); ++i) {
        cout << setw(3) << rank << ". " << i->first << ": " << i->second.first.first << " is " << i->second.first.second << endl;
        winnings += (rank * i->second.second);
        rank++;
    }

    cout << "Total winnings are " << winnings << endl;

    return 0;
}