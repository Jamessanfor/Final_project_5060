#pragma once

#include "DECK.h"




/*
ofstream myfile;
int simulations = 1000;
myfile.open("win_loss.csv"); */





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
        //cout << id << endl;
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
            for (int i = potential_hands.size() - 1; i > -1; i--) {
                if ( players[player_index].curr_hand == i) {
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
        players[player_index].check_hands();


        int prob = dist(gen);


        if (players[player_index].curr_hand >= (stopping_hand-2)|| players[player_index].high >= (stopping_card - 2) || prob > 6000) {
            players[player_index].action = raise;
            
        }
        else if (players[player_index].curr_hand >= (1) || players[player_index].high >= (stopping_card - 4) ||  prob > 9000 ) {
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

        players[player_index].check_hands();

        int prob = dist(gen);



        if (players[player_index].curr_hand >= (4) || players[player_index].high>10 || prob > 6000) {
            players[player_index].action = raise;
        }
        else if (players[player_index].curr_hand >= (2) || players[player_index].high > 5 ||  prob > 9000) {
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
           
           rank_map[players[player_index].high] = (rank_map[players[player_index].high] <0) ? (rank_map[players[player_index].high]-.01) : 0;
           hand_map[players[player_index].curr_hand] = (hand_map[players[player_index].curr_hand] < 0) ? (hand_map[players[player_index].curr_hand] - .01) : 0;

       }
       else {
           
           rank_map[players[player_index].high] = (rank_map[players[player_index].high] > 1) ? ((rank_map[players[player_index].high] + .01)) : 1;
           hand_map[players[player_index].curr_hand] = (hand_map[players[player_index].curr_hand] > 1) ? ((hand_map[players[player_index].curr_hand] + .01)) : 1;

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
    bool bluff() {

        return dist(gen) > 5000;

    }

    void raise_meet_fold(vector<player>& players) {
        players[player_index].check_hands();

        int prob = dist(gen);

        if (players[player_index].curr_hand != hand_player || players[player_index].high != rank_player  ) {
            set_hand(players);
        }

        //cout << "prob: " << hand_map[players[player_index].curr_hand] <<" : ";
        
        if ( prob  <= (hand_map[players[player_index].curr_hand] * 10000) || prob <= (rank_map[players[player_index].curr_hand] * 10000)) {
            players[player_index].action = raise;
            //cout<< " raise " << " : ";

        }
        else if (prob <= (hand_map[players[player_index].curr_hand] * 15000) || prob <= (rank_map[players[player_index].curr_hand] * 15000)) {
            players[player_index].action = meet;

         //   cout << " meet " << " : ";

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






class Monte_carlo_player {

public:

    int player_index = 0;
    int money = 0;

    int high = 0;
    double meet_prob = 0;
    double raise_prob = 0;

    vector<card> c;
    vector<int> potentialhands;


    Monte_carlo_player( int id) {
        player_index = id;
        potentialhands.resize(Possible_hand.size());
        //set inital prob
   
    }
    


    bool bluff() {
    
        return dist(gen) > 100;
    
    }





    void set_high(string a, string b) {
        for (int i = 0; i < ranks.size(); i++) {
            if (a == ranks[i] || b == ranks[i]) {
                high = i;
            }
        }
    }



    void check_flush_straight_royal(vector<card>& cards) {



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
                potentialhands[5] = 1;
            }

            for (int k = 0; k < straight.size(); k++) {
                //checking straight hand
                if (straight_mp[k]) {
                    stright_ct++;
                    if (straight[k] == cards[0].rank || straight[k] == cards[1].rank) stright_flag = true;
                    if (stright_flag && stright_ct > 4) potentialhands[4] = 1;
                }
                else stright_ct = 0;

                if (mp[j][k]) {
                    straight_count++;
                    if (!(straight_flush_flag) && ((cards[0].rank == straight[k] && cards[0].suit == flush[j]) || (cards[1].rank == straight[k] && cards[1].suit == flush[j]))) {
                        straight_flush_flag = true;
                    }
                    if (straight_count > 4 && straight_flush_flag) potentialhands[8] = 1;
                }
                else { straight_count = 0; straight_flush_flag = false; }


                if ((k > 9) && mp[j][k]) {
                    royal_count++;
                    if ((k > 9) && !(royal_flag) && ((cards[0].rank == straight[k] && cards[0].suit == flush[j]) || (cards[1].rank == straight[k] && cards[1].suit == flush[j]))) {
                        royal_flag = true;
                    }
                    if (royal_count > 4 && royal_flag) potentialhands[9] = 1;
                    ;
                }
                else { royal_count = 0; royal_flag = false; }

            }
            royal_count = 0;
            straight_count = 0;

        }



    }





    int find_duplics(vector<card>& cards) {
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
                potentialhands[7]++;
                set_high(c.first, c.first);
            }
            //set full house
            else if ((c.second == 3 && potentialhands[1]) || c.second == 2 && potentialhands[3]) {
                potentialhands[6]++;
                set_high(c.first, c.first);
            }
            //set 3 of a kind
            else if (c.second == 3) {
                potentialhands[3]++;
                set_high(c.first, c.first);
            }
            else if (c.second == 2 && potentialhands[1]) {
                potentialhands[2]++;
                set_high(c.first, c.first);
            }//set pair
            else if (c.second == 2) {
                potentialhands[1]++;
                set_high(c.first, c.first);
            }


        }
        if (potentialhands[1] && potentialhands[3]) {
            potentialhands[6]++;
        }
        mp.clear();
        return maxy;
    }
    void check_hands( vector<card>& cards) {
        check_flush_straight_royal(cards);

        find_duplics(cards);
        int count = 0;
        int temp_hand = 0;
        for (int i = 0; i < potentialhands.size(); i++) {
            if (potentialhands[i]) {
                temp_hand = max(i, temp_hand);
                count++;
            }
        }
        if (!count) {
            potentialhands[0] = 1;
            set_high(cards[0].rank, cards[1].rank);
        }
        //make contriput to sim bet
        



    }





    void compare( vector<player>& players) {
        int hand = 0;
        for (int i = potentialhands.size()-1; i >-1; i--) {
            if (potentialhands[i]) {
                hand = i;
                break;
            }
        }

        //cout<<"sim hand: "<< hand <<" real hand:  " << players[player_index].curr_hand << endl;
        if (hand < players[player_index].curr_hand) {
            raise_prob += .01;
        }
        if (hand <= players[player_index].curr_hand) {
            meet_prob += .01;
        }
        
    }
    bool cvp_hand(card& rand, vector<player>& players) {
        for (int i = 0; i < players[player_index].cards.size(); i++) {
            if (rand.rank == players[player_index].cards[i].rank &&  rand.suit == players[player_index].cards[i].suit) {
                return false;
            }
        }
        return true;
    }

    void rand_push(vector<card>& c, vector<player>& players) {
       // cout << 13 << " : ";
        while (c.size() < players[player_index].cards.size()) {
            card* rand = new card(suits[dist(gen)%suits.size()], ranks[dist(gen) % ranks.size()]);
            //cout << rand->rank << " : ";
            if (cvp_hand(*rand, players)) {//check if in hand;
                c.push_back(*rand);
              //  cout << 15 << " : ";
            }
            delete(rand);
        }
       // cout << 14 << " : ";
        
    }

    void run_sim(vector<player>& players) {
        int sim = 100;


        while (sim) {
            
            if (players[player_index].cards.size() > 2) {
                
                for (int i = 2; i < players[player_index].cards.size(); i++) {

                    c.push_back(players[player_index].cards[i]);

                }
            }
            rand_push(c, players);


            check_hands(c);
            compare( players);




            sim--;
            //reset
            for (int i = 0; i < potentialhands.size(); i++) {
            
                potentialhands[i] = 0;
            }
            c.clear();
        }




    
    }

    void raise_meet_fold(vector<player>& players) {

        //cout << 10 << " : ";
        players[player_index].check_hands();

        run_sim(players);

        int prob = dist(gen);
        //cout<<" raise: "<< raise_prob << " meet: " << meet_prob<<endl;
        if (prob < raise_prob*10000) {
            players[player_index].action = raise;

        }
        else if (prob < meet_prob * 10000) {
            players[player_index].action = meet;

        }
        else {
            players[player_index].action = fold;

        }
        raise_prob = 0;
        meet_prob = 0;

    }





    /*
    ofstream myfile;
    int simulations = 1000;
    myfile.open("win_loss.csv"); */




};

























