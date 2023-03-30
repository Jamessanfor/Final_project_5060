

#include "Header.h"




int main() {
    //lazly didnt want to change the vector
    reverse(Possible_hand.begin(), Possible_hand.end());
    vector<player> players;
    vector<card> c;
    int head = 4;

    deck D = deck();
    D.shuffle();
    D.makegame(players,head );
    D.flop(players);
    D.turn(players);
    D.river(players);
    D.deal(players);
    D.flop(players);
    D.turn(players);
    D.river(players);

    for (int i = 0; i < players.size(); i++) {
        players[i].check_hands();
        for (int j = 0; j < players[i].cards.size(); j++) {
            cout << "| " << players[i].cards[j].rank << "  " << players[i].cards[j].suit << "| ";
        }
        cout << endl;
        for (int j = 0; j < players[i].potential_hands.size(); j++) {
            if (players[i].potential_hands[j]) {
                cout << Possible_hand[j] << " : ";
                if (Possible_hand[j] == Possible_hand[0] || Possible_hand[j] == Possible_hand[1]) {
                    cout<< ranks[players[i].high] <<endl;
                }
            }
        }
        cout << endl;
    }
    
    return 0;
}



