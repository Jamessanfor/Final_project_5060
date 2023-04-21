


#include "player_type.h"



int main() {


    ofstream myfile;
    int simulations = 1000;
    myfile.open("win_loss.csv");
    //*/

        //lazly didnt want to change the vector
    reverse(Possible_hand.begin(), Possible_hand.end());
    vector<player> players;
    vector<card> c;
    int head = 4;//number of players

    deck D = deck();
    D.shuffle();
    D.shuffle();
    D.makegame(players, head);
    D.flop(players);
    D.turn(players);
    D.river(players);
    int sim = 1000;

    cout << "players size: " << players.size() << endl;
    optimal_stopper op_stop = optimal_stopper(D, players, 0);
    genaric_player gen_player = genaric_player(1);
    Explore_Exploit_player exp_exp = Explore_Exploit_player(2, players);
    Monte_carlo_player monte = Monte_carlo_player(3);

    while (sim) {

        poker_sim(D, players, op_stop, gen_player, exp_exp, monte);

        sim--;
        for (int o = 0; o < players.size(); o++) {
            myfile << players[o].wins << ",";
        }
        myfile << "\n";

    }
    myfile.close();
    cout << "Winnings for each player ater 100k sims" << endl;
    cout << "Generic Player: " << players[1].money << endl;
    cout << "Optimal Stopping: " << players[0].money << endl;
    cout << "Explore Exploit: " << players[2].money << endl;
    cout << "Monte Carlo: " << players[3].money << endl;

    return 0;
}


