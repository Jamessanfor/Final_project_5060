#pragma once


#include <cmath>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <random>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>

#define e (2.71828)

using namespace std;

default_random_engine gen;
uniform_int_distribution<int> dist(0, 10000);
vector<string> suits = { "Heart","Spade","Diamond","Club" };
vector<string> ranks = { "1","2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace" };
vector<string> Possible_hand = { "Royal Flush","Straight Flush","Four of a Kind","FULL House","Flush","Straight","Three of a Kind","Two Piar","Pair","High Card" };


class card {
public:
    string suit;
    string rank;
    card(string a, string b) {
        suit = a;
        rank = b;
    }

};

class player {

public:

    vector<card> cards;
    vector<int> potential_hands;
    int bet;
    int high = 0;
    string dup_rank;
    void set_high(string a, string b) {
        for (int i = 0; i < ranks.size(); i++) {
            if (a == ranks[i] || b == ranks[i]) {
                high = i;
            }
        }
    }

    player(card a, card b) {
        cards.push_back(a);
        cards.push_back(b);
        potential_hands.resize(10);
        set_high(a.rank, b.rank);
    }

    bool check_straight() {
        vector<string> straight = { "Ace","1","2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace" };
        vector<int> mp(straight.size());
        int count = 0;
        int temp = 0;
        bool flag = false;
        for (int i = 1; i < cards.size(); i++) {
            for (int j = 1; j < straight.size(); j++) {
                if (cards[i].rank == straight[j]) {
                    mp[j] = 1;
                }
            }
        }
        for (int i = 0; i < mp.size(); i++) {
            if (mp[i]) {
                count++;
                if (straight[i]== cards[0].rank || straight[i] == cards[1].rank) flag = true;
                if (flag && count >= 5  )return true;
            }
            else count = 0;
        }


        return false;
    }
    bool check_flush() {
        vector<string> flush = { "Heart","Spade","Diamond","Club" };
        vector<int> mp(flush.size());
        int count = 0;
        int temp = 0;
        bool flag = false;

        for (int i = 0; i < cards.size(); i++) {
            for (int j = 0; j < flush.size(); j++) {
                if (cards[i].suit == flush[j]) {
                    mp[j] ++;
                }
            }
        }
        for (int i = 0; i < mp.size(); i++) {

            if (mp[i] >= 5 && (cards[0].suit == flush[i] || cards[1].suit == flush[i])) {
                return true;
            }

        }


        return false;


    }
    bool check_straight_flush() {
        vector<string> flush = { "Heart","Spade","Diamond","Club" };
        vector<string> straight = { "Ace","1","2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace" };
        vector<vector<int>> mp(flush.size(), vector<int>(straight.size()));
        bool flag = false;
        for (int i = 0; i < cards.size(); i++) {
            for (int j = 0; j < flush.size(); j++) {
                for (int k = 0; k < straight.size(); k++) {
                    if( ( cards[i].rank == straight[k]) && (cards[i].suit == flush[j]))
                    mp[j][k] = 1;
                    
                }
            }
        }
        int count = 0;
        
        for (int j = 0; j < flush.size(); j++) {
            for (int k = 0; k < straight.size(); k++) {
                if (mp[j][k]) {
                    count++;
                    if ( !(flag) && ((cards[0].rank==straight[k] && cards[0].suit == flush[j]) || (cards[1].rank == straight[k] && cards[1].suit == flush[j]))) { 
                        flag = true; }
                    if (count > 4 && flag )return true;
                }
                else { count = 0; flag = false; }

            }
        }
        return false;

    }
    bool check_Royal() {
        vector<string> flush = { "Heart","Spade","Diamond","Club" };
        vector<string> straight = { "10","Jack","Queen","King","Ace" };
        vector<vector<int>> mp(flush.size(), vector<int>(straight.size()));
        bool flag = false;
        for (int i = 0; i < cards.size(); i++) {
            for (int j = 0; j < flush.size(); j++) {
                for (int k = 0; k < straight.size(); k++) {
                    if( ( cards[i].rank == straight[k]) && (cards[i].suit == flush[j]))
                        mp[j][k] = 1;
                }
            }
        }
        int count = 0;
        for (int j = 0; j < flush.size(); j++) {
            for (int k = 0; k < straight.size(); k++) {
                if (mp[j][k]) {
                    count++;
                    if (!(flag) && ((cards[0].rank == straight[k] && cards[0].suit == flush[j]) || (cards[1].rank == straight[k] && cards[1].suit == flush[j]))) {
                        flag = true;
                    }
                    if (count > 4 && flag )return true;
                }
                else { count = 0; flag = false; }

            }
            count = 0;
        }
        return false;

    }


    int find_duplics(vector<card> cards) {
        int maxy = 0;
        unordered_map<string, int> mp;
        
        mp[cards[0].rank]++;
        mp[cards[1].rank]++;
        
        for (int i = 2; i < cards.size(); i++) {
            if((cards[i].rank == cards[0].rank ) ||( cards[i].rank == cards[1].rank))
            mp[cards[i].rank]++;
        }
        for (auto& c : mp) {
            //set 4 of a kind
            if (c.second == 4) {
                potential_hands[7]++;
                set_high(c.first, c.first);
            }
            //set full house
            else if ((c.second == 3 && potential_hands[1]) || c.second == 2 && potential_hands[3]) {
                potential_hands[6]++;
                set_high(c.first, c.first);
            }
            //set 3 of a kind
            else if (c.second == 3) {
                potential_hands[3]++;
                set_high(c.first, c.first);
            }
            else if (c.second == 2 && potential_hands[1]) {
                potential_hands[2]++;
                set_high(c.first, c.first);
            }//set pair
            else if (c.second == 2) {
                potential_hands[1]++;
                set_high(c.first, c.first);
            }

            
        }
        if (potential_hands[1] && potential_hands[3]) {
            potential_hands[6]++;
        }
        
        return maxy;
    }
    void check_hands() {
        potential_hands[4] = check_straight();
        potential_hands[5] = check_flush();
        potential_hands[8] = check_straight_flush();
        potential_hands[9] = check_Royal();
        find_duplics(cards);
        int count = 0;
            for (int i = 0; i < potential_hands.size(); i++) {
                if (potential_hands[i])
                    count++;
            }
            if (!count) {
                potential_hands[0] = 1;
                set_high(cards[0].rank, cards[1].rank);
            }

    }



};


class deck {
public:
    vector<card> DECK;
    deck() {
        for (int i = 0; i < suits.size(); i++) {
            for (int j = 0; j < ranks.size(); j++) {
                card* c = new card(suits[i], ranks[j]);
                DECK.push_back(*c);
            }
        }

    }
    void print_deck() {
        for (int i = 0; i < DECK.size(); i++) {

            cout << DECK[i].suit << "::" << DECK[i].rank << endl;

        }

    }
    void shuffle() {
        for (int i = 0; i < DECK.size(); i++) {
            swap(DECK[i], DECK[dist(gen) % (DECK.size() - 1)]);
        }

    }
    void makegame(vector<player>& players, int heads) {
        for (int i = 0; i < heads; i++) {
            player* temp = new player(DECK[DECK.size() - 1], DECK[DECK.size() - 2]);
            players.push_back(*temp);
            DECK.pop_back(); DECK.pop_back();
        }


        return;
    }
    void flop(vector<player>& players) {
        for (int i = 0; i < players.size(); i++) {
            for (int j = 0; j < 3; j++) {
                players[i].cards.push_back(DECK[DECK.size() - (j + 1)]);
            }
        }
        for (int i = 0; i < 3; i++) {
            DECK.pop_back();
        }
    }
    void turn(vector<player>& players) {
        for (int i = 0; i < players.size(); i++) {
            for (int j = 0; j < 1; j++) {
                players[i].cards.push_back(DECK[DECK.size() - (j + 1)]);
            }
        }
        for (int i = 0; i < 1; i++) {
            DECK.pop_back();
        }
    }

    void river(vector<player>& players) {
        for (int i = 0; i < players.size(); i++) {
            for (int j = 0; j < 1; j++) {
                players[i].cards.push_back(DECK[DECK.size() - (j + 1)]);
            }
        }
        for (int i = 0; i < 1; i++) {
            DECK.pop_back();
        }
    }

    void deal(vector<player>& players) {
        for (int i = 0; i < players.size(); i++) {
            for (int j = 0; j < 2; j++) {
                players[i].cards.push_back(DECK.back());
                DECK.pop_back();
            }
        }
        for (int i = 0; i < players.size(); i++) {
            for (int j = 0; j < 5; j++) {
                players[i].cards.push_back(DECK[DECK.size() - (j + 1)]);
            }
        }
        for (int i = 0; i < 5; i++) {
            DECK.pop_back();
        }


    }
    void mergeDECK(vector<player>& players) {
        for (int i = 0; i < players.size(); i++) {

            for (int j = 0; j < 2; j++) {
                DECK.push_back(players[i].cards[j]);
                players[i].cards.pop_back();
            }
            if (!i) {
                while (players[i].cards.size()) {
                    DECK.push_back(players[i].cards.back());
                    players[i].cards.pop_back();
                }
            }
            players[i].cards.clear();
            delete& players[i];
        }
        players.clear();

        return;
    }



    //these two are for testing
    void maketest(vector<player>& players, int heads, vector<card> cs) {
        for (int i = 0; i < heads; i++) {
            player* temp = new player(cs[cs.size() - 1], cs[cs.size() - 2]);
            players.push_back(*temp);
            cs.pop_back(); cs.pop_back();
        
        
            for (int j = 0; j < 5; j++) {
                players[i].cards.push_back(cs.back());
                cs.pop_back();
            }
            
        }

        return;
    }


    void makeroayal(vector<card>& cards) {
        vector<string> flush = { "Heart","Spade","Diamond","Club" };
        vector<string> straight = { "10","Jack","Queen","King","Ace" };
        for (int i = 0; i < flush.size(); i++) {
            for (int j = 0; j < straight.size(); j++) {
                card* c = new card(flush[i], straight[j]);
                cards.push_back(*c);
            }
        }
        for (int i = 0; i < flush.size(); i++) {
            for (int j = 0; j < straight.size(); j++) {
                card* c = new card(flush[i], straight[j]);
                cards.push_back(*c);
            }
        }


    }



};



