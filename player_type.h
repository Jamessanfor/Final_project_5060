#pragma once

#include "DECK.h"




/*
ofstream myfile;
int simulations = 1000;
myfile.open("win_loss.csv"); */




vector<string> action_list = { "fold","meet","raise" };

//optimal stopping player 
//runs though a random output of hands and cards to see 
//hands are optimal to bet on
class optimal_stopper {

public:
    int stopping_card = 0;
    int stopping_hand = 0;
    int player_index = 0;
    

    optimal_stopper(deck& D, vector<player>& players,int id) {
        player_index = id;
        find_stopping(D, players);
    }


    void find_stopping(deck& D, vector<player>& players) {
        int sims = 1000;
        
        int maxy = 0;
        int maxy_card = 0;
        while (sims) {
            D.shuffle();
            D.deal(players);
            D.flop(players);
            D.turn(players);
            D.river(players);
            players[player_index].check_hands();
            D.shuffle();
            for (int i = players[player_index].potential_hands.size() - 1; i > -1; i--) {
                if (players[player_index].potential_hands[i]) {
                    stopping_hand = i; break;
                }

            }
            maxy = max(stopping_hand, maxy);
            maxy_card = max(maxy_card, players[player_index].high);
            sims--;
        }

        stopping_hand = maxy;
        stopping_card = maxy_card;
        
    }


    void raise_meet_fold(vector<player>& players) {
        //check current hand
        if (players[player_index].curr_hand >= (stopping_hand-2)|| players[player_index].high >= (stopping_card - 2) || dist(gen) > 6000) {
            players[player_index].action = raise;
            
        }
        else if (players[player_index].curr_hand >= (1) || players[player_index].high >= (stopping_card - 4) ||  dist(gen) > 9000 ) {
            players[player_index].action = meet;
            
        }
        else { 
            players[player_index].action = fold;
            
        }


    }





    /*
    ofstream myfile;
    int simulations = 1000;
    myfile.open("win_loss.csv"); */




};


//genaric player
//simulates baseline players
class genaric_player {

public:

    
    int player_index = 0;


    genaric_player( int id) {
        player_index = id;
        
    }




    void raise_meet_fold(vector<player>& players) {
        //check current hand



        if (players[player_index].curr_hand >= (4) || players[player_index].high>10 || dist(gen) > 6000) {
            players[player_index].action = raise;
        }
        else if (players[player_index].curr_hand >= (2) || players[player_index].high > 5 ||  dist(gen) > 9000) {
            players[player_index].action = meet;
        }
        else {
            players[player_index].action = fold;

        }


    }




};






class Explore_Exploit_player {

public:

    int player_index = 0;
    int money = 0;
    int rank_player= 0;
    int hand_player = 0;

    vector<int> hand_map;
    vector<int> rank_map;
    void set_prob_hand(vector<int> &map) {
    
        for (int i = 0; i < map.size(); i++) {
            if (i < ((map.size()) / 2))
                map[i] = 0.5;
            else map[i] = i / map.size();
        }

    }
    void set_prob(vector<int>& map) {

        for (int i = 0; i < map.size(); i++) {
                map[i] = i / map.size();
        }

    }
    Explore_Exploit_player(int id) {
        player_index = id;
        hand_map.resize(Possible_hand.size());
        rank_map.resize(ranks.size());
        //set inital prob
        set_prob_hand(hand_map);
        set_prob(rank_map);
    }
    void set_money(vector<player>& players) {
        money = players[player_index].money;

    }
    void check_money(vector<player>& players) {
        
       if( money > players[player_index].money){
           
           rank_map[players[player_index].high] = (rank_map[players[player_index].high] >0) ? (rank_map[players[player_index].high]-.01) : 0;
           hand_map[players[player_index].curr_hand] = (hand_map[players[player_index].curr_hand] > 0) ? (hand_map[players[player_index].curr_hand] - .01) : 0;

       }
       else {
           
           rank_map[players[player_index].high] = (rank_map[players[player_index].high] < 1) ? ((rank_map[players[player_index].high] + .01)) : 1;
           hand_map[players[player_index].curr_hand] = (hand_map[players[player_index].curr_hand] < 1) ? ((hand_map[players[player_index].curr_hand] + .01)) : 1;

       }

    }
    void check_set(vector<player>& players){
        
        check_money(players);
        
        set_money(players);
    }

    void set_hand(vector<player>& players) {
        rank_player = players[player_index].high;
        hand_player = players[player_index].curr_hand;
        
    }


    void raise_meet_fold(vector<player>& players) {


        if (players[player_index].curr_hand != hand_player || players[player_index].high != rank_player) {
            set_hand(players);
        }


        
        if ( dist(gen)  <= (hand_map[players[player_index].curr_hand] * 10000) || dist(gen) <= (rank_map[players[player_index].curr_hand] * 10000)) {
            players[player_index].action = raise;

        }
        else if (dist(gen) <= (hand_map[players[player_index].curr_hand] * 15000) || dist(gen) <= (rank_map[players[player_index].curr_hand] * 15000)) {
            players[player_index].action = meet;

        }
        else {
            players[player_index].action = fold;

        }


    }





    /*
    ofstream myfile;
    int simulations = 1000;
    myfile.open("win_loss.csv"); */




};









