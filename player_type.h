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






class Explore_Exploit {

public:
    int stopping_card = 0;
    int stopping_hand = 0;
    int player_index = 0;


    Explore_Exploit(int id) {
        player_index = id;
        
    }



    void raise_meet_fold(vector<player>& players) {
        //check current hand
        if (players[player_index].curr_hand >= (stopping_hand - 2) || players[player_index].high >= (stopping_card - 2) || dist(gen) > 6000) {
            players[player_index].action = raise;

        }
        else if (players[player_index].curr_hand >= (1) || players[player_index].high >= (stopping_card - 4) || dist(gen) > 9000) {
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









