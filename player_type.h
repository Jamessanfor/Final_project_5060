#pragma once

#include "DECK.h"




vector<string> action_list = { "fold","meet","raise" };


class optimal_stopper {

public:

    int best = 0;
    int player_index = 0;
    int action = 0;

    optimal_stopper(deck& D, vector<player> players,int id) {
        player_index = id;
        find_stopping(D, players);
    }


    void find_stopping(deck& D, vector<player> players) {
        int sims = 1000;
        
        int maxy = 0;
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
                    best = i; break;
                }

            }
            maxy = max(best, maxy);
            sims--;
        }

        best = maxy;
        
        
    }




    //
    void raise_meet_fold(vector<player> players) {
        //check current hand
        if (players[player_index].curr_hand >= (best-2)) {
            action = raise;
            
        }
        else if (players[player_index].curr_hand >= (best - 3) && dist(gen)>5000 ) {
            action = meet;
            
        }
        else { 
            action = fold;
            
        }


    }





    /*
    ofstream myfile;
    int simulations = 1000;
    myfile.open("win_loss.csv"); */




};






