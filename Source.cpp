


#include "player_type.h"



int main() {
    //lazly didnt want to change the vector
    reverse(Possible_hand.begin(), Possible_hand.end());
    vector<player> players;
    vector<card> c;
    int head = 2;//number of players

    deck D = deck();

    D.shuffle();
    D.shuffle();
    D.makegame(players,head );
    D.flop(players);
    D.turn(players);
    D.river(players);
    int sim = 1000;
    int flag = 0;
    optimal_stopper op_stop = optimal_stopper(D, players, 0);
    genaric_player gen_player = genaric_player(1);
    while (sim) {
        //game
        D.shuffle();
        D.shuffle();
        D.deal(players);
        op_stop.raise_meet_fold(players);
        gen_player.raise_meet_fold(players);
        flag = D.betting_stage(players);
        if (!flag) {
            D.flop(players);
            op_stop.raise_meet_fold(players);
            gen_player.raise_meet_fold(players);

            flag = D.betting_stage(players);
        }
        
        if (!flag) {
            D.turn(players);
            op_stop.raise_meet_fold(players);
            gen_player.raise_meet_fold(players);

        
            flag = D.betting_stage(players);
        }
        
        if (!flag) {
            D.river(players);

            op_stop.raise_meet_fold(players);
            gen_player.raise_meet_fold(players);
            D.divy_up_winnings(players);
        }
        flag = 0;
        //D.print_playerhands(players);
       // int enter;
        sim--;
        
       //cout<<players[0].money<<endl;
       // cout << players[1].money << endl;
    }
    cout << D.pot << endl;
    cout << players[0].money << endl;
    cout << players[1].money << endl;

        return 0;
}



