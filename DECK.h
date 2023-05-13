#pragma once

#include "players.h"
#include <queue>


class deck {
public:
    vector<card> DECK;
    int pot = 0;
    int round = 0;
    void putmoney(int money) {
        pot = pot + money;
       // cout<< " Pot: " << pot << endl;
    }


    
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
        round++;
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
    void resetplayers(vector<player>& players) {
        for (int i = 0; i < players.size();i++) {
            players[i].reset_hand();
        }
    }

    void deal(vector<player>& players) {
        round++;
        resetplayers(players);
        for (int i = 0; i < players.size(); i++) {
            if (!i) {
                for (int j = players[i].cards.size() - 1; j > 1; j--) {
                    if (j>1) {
                        DECK.push_back(players[i].cards.back());
                        players[i].cards.pop_back();
                    }

                }
            }
            else {
                for (int j = players[i].cards.size() - 1; j > 1; j--) {

                    
                    players[i].cards.pop_back();
                }
            }
            while ( players[i].cards.size()) {
                DECK.push_back(players[i].cards.back());
                players[i].cards.pop_back();

            }

        }

        shuffle();
        shuffle();

        for (int i = 0; i < players.size(); i++) {
            for (int j = 0; j < 2; j++) {
                players[i].cards.push_back(DECK.back());
                DECK.pop_back();
            }
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

    void divy_up_winnings(vector<player>& players) {
        
        priority_queue<pair<int, int> > pq;
        vector<int> winners;
        int best_hand = 0;
        int highest_card = 0;
        int winning_hands = 0;
        int count = 0;
        for (int i = 0; i < players.size(); i++) {

            if (players[i].action) {

                pq.push(make_pair(players[i].curr_hand, i));
            }
            
        }

        if (!pq.empty()) {
            pair<int, int> check = pq.top();


            pq.pop();
            winners.push_back(check.second);

            while (!pq.empty()) {

                if (players[check.second].curr_hand == players[pq.top().second].curr_hand && players[check.second].high < players[pq.top().second].high) {
                    winners.pop_back();
                    check = pq.top();
                    pq.pop();
                    winners.push_back(check.second);
                }
                else if (players[check.second].curr_hand == players[pq.top().second].curr_hand && players[check.second].high == players[pq.top().second].high) {
                    check = pq.top();
                    pq.pop();
                    winners.push_back(check.second);

                }

                else break;

            }
        }
        else return;


        //distribute winnings

        for (int i = 0; i < winners.size(); i++) {
            players[winners[i]].money += pot / winners.size();
            players[winners[i]].wins++;
            //cout << winners[i] <<" : " << pot << endl;

        }
        pot = 0;

    }


    int betting_stage(vector<player> &players) {
        vector<int> meet_players;
        vector<int> bets;
        int curr_bet = 5;
        int maxy_bet = curr_bet;
        int players_in = 0;
        for (int i = 0; i < players.size(); i++) {
            if (players[i].action == 2) {//
                //put money
                maxy_bet *= 2;
                players[i].meet_bet(maxy_bet);
                maxy_bet = (maxy_bet, players[i].curr_bet);
                meet_players.push_back(i);
                players_in++;
            }
            else if (players[i].action) {

                meet_players.push_back(i);
                players_in++;
            }
        }
        for (int i = 0; i < meet_players.size(); i++) {
            //cout<< "maxy: " << maxy_bet << endl;
            players[meet_players[i]].meet_bet(maxy_bet);

        }

        if (players_in > 1) {
            
            for (int i = 0; i<players.size(); i++) {
                if (players[i].action) {
                    int temp = players[i].curr_bet;
                   
                    bets.push_back(temp);
                    players[i].fold_bet();
                }

            }


        }
        else {
            
            divy_up_winnings(players);
            return 1;
        }

        for (int i = 0; i < bets.size(); i++) {
            putmoney(bets[i]);
            
            //cout<<"betting: " << pot << endl;

        }
        return 0;

    }

    void check_account_balance(vector<player>& players){
        for (int i = 0; i < players.size(); i++) {
            if (players[i].money<=0) {
                swap(players[i], players[players.size()-1]);
                
                players.pop_back();
            }
        }
    
    }

    void print_playerhands(vector<player>& players) {


        for (int i = 0; i < players.size(); i++) {

            players[i].check_hands();
            for (int j = 0; j < players[i].cards.size(); j++) {
                cout << "| " << players[i].cards[j].rank << "  " << players[i].cards[j].suit << "| ";
            }
            cout << endl;
            for (int j = 0; j < potential_hands.size(); j++) {
                if (players[i].curr_hand==j) {
                    cout << Possible_hand[j] << " : ";
                    if (Possible_hand[j] == Possible_hand[0] || Possible_hand[j] == Possible_hand[1]) {
                        cout << ranks[players[i].high] << endl;
                    }
                }
            }
            cout << endl;
        }


    }



};









//less efficient check
/*
   bool check_straight() {
       vector<string> straight = { "Ace","1","2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace" };
       vector<int> mp(straight.size());
       int count = 0;

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
   //*/
