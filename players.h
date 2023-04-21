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

#define fold 0
#define meet 1
#define raise 2

using namespace std;



default_random_engine gen;
uniform_int_distribution<int> dist(0, 10000);
vector<string> suits = { "Heart","Spade","Diamond","Club" };
vector<string> ranks = { "1","2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace" };
vector<string> Possible_hand = { "Royal Flush","Straight Flush","Four of a Kind","FULL House","Flush","Straight","Three of a Kind","Two Piar","Pair","High Card" };
vector<int> potential_hands= { 0,0,0,0,0,0,0,0,0,0};
unordered_map<string, int> mp;
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
    

    int money = INT_MAX / 5;
    int action = 0;
    int high = 0;
    int curr_hand = 0;
    int curr_bet = 0;
    int wins = 0;

    void meet_bet(int bet) {


        money = money - bet;

        curr_bet += bet;

        
    }


    void fold_bet() {

        curr_bet = 0;
    
    }




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
        set_high(a.rank, b.rank);
    }

    void check_flush_straight_royal() {
        vector<string> flush = { "Heart","Spade","Diamond","Club" };
        vector<string> straight = { "Ace","1","2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace" };
        vector<vector<int>> mp(flush.size(), vector<int>(straight.size()));
        vector<int> flush_mp(flush.size());
        vector<int> straight_mp(straight.size());

        for (int i = 0; i < cards.size(); i++) {
            for (int j = 0; j < flush.size(); j++) {
                if (cards[i].suit == flush[j]) {
                    flush_mp[j] ++;
                }
                for (int k = 0; k < straight.size(); k++) {
                    if (cards[i].rank == straight[j]) {
                        straight_mp[j] = 1;
                    }
                    if ((cards[i].rank == straight[k]) && (cards[i].suit == flush[j])) {
                        mp[j][k] = 1;
                    }
                }
            }
        }
        bool royal_flag = false;
        bool straight_flush_flag = false;
        bool stright_flag = false;
        int royal_count = 0;
        int straight_count = 0;
        int stright_ct = 0;

        for (int j = 0; j < flush.size(); j++) {
            //checking flush hand
            if ((flush_mp[j] > 4) && (cards[0].suit == flush[j] || cards[1].suit == flush[j])) {
                potential_hands[5] = 1;
            }

            for (int k = 0; k < straight.size(); k++) {
                //checking straight hand
                if (straight_mp[k]) {
                    stright_ct++;
                    if (straight[k] == cards[0].rank || straight[k] == cards[1].rank) stright_flag = true;
                    if (stright_flag && stright_ct > 4) potential_hands[4] = 1;
                }
                else stright_ct = 0;

                if (mp[j][k]) {
                    straight_count++;
                    if (!(straight_flush_flag) && ((cards[0].rank == straight[k] && cards[0].suit == flush[j]) || (cards[1].rank == straight[k] && cards[1].suit == flush[j]))) {
                        straight_flush_flag = true;
                    }
                    if (straight_count > 4 && straight_flush_flag) potential_hands[8] = 1;
                }
                else { straight_count = 0; straight_flush_flag = false; }


                if ((k > 9) && mp[j][k]) {
                    royal_count++;
                    if ((k > 9) && !(royal_flag) && ((cards[0].rank == straight[k] && cards[0].suit == flush[j]) || (cards[1].rank == straight[k] && cards[1].suit == flush[j]))) {
                        royal_flag = true;
                    }
                    if (royal_count > 4 && royal_flag) potential_hands[9] = 1;
                    ;
                }
                else { royal_count = 0; royal_flag = false; }

            }
            royal_count = 0;
            straight_count = 0;

        }



    }





    int find_duplics(vector<card> cards) {
        int maxy = 0;
        //unordered_map<string, int> mp;

        mp[cards[0].rank]++;
        mp[cards[1].rank]++;

        for (int i = 2; i < cards.size(); i++) {
            if ((cards[i].rank == cards[0].rank) || (cards[i].rank == cards[1].rank))
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
        mp.clear();
        return maxy;
    }
    void check_hands() {
        check_flush_straight_royal();

        find_duplics(cards);
        int count = 0;
        int temp_hand = 0;
        for (int i = 0; i < potential_hands.size(); i++) {
            if (potential_hands[i]) {
                temp_hand = max(i, temp_hand);
                count++;
            }
        }
        if (!count) {
            potential_hands[0] = 1;
            set_high(cards[0].rank, cards[1].rank);
        }
        curr_hand = temp_hand;
        temp_hand = 0;
        reset_for_next();

    }

    void reset_for_next() {
        potential_hands[0] = 0;
        potential_hands[1] = 0;
        potential_hands[2] = 0;
        potential_hands[3] = 0;
        potential_hands[4] = 0;
        potential_hands[5] = 0;
        potential_hands[6] = 0;
        potential_hands[7] = 0;
        potential_hands[8] = 0;
        potential_hands[9] = 0;
    
    }

    void reset_hand() {


        potential_hands[0] = 0;
        potential_hands[1] = 0;
        potential_hands[2] = 0;
        potential_hands[3] = 0;
        potential_hands[4] = 0;
        potential_hands[5] = 0;
        potential_hands[6] = 0;
        potential_hands[7] = 0;
        potential_hands[8] = 0;
        potential_hands[9] = 0;

        
        action = 1;

    }





};
