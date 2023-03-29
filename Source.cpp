

#include "Header.h"




int main() {
    //lazly didnt want to change the vector
    reverse(Possible_hand.begin(), Possible_hand.end());
    vector<player> players;
    vector<card> c;
    int head = 4;

    deck D = deck();
    D.shuffle();
    D.shuffle();
    D.makegame(players,head );
    for (int i = 0; i < players.size(); i++) {
        players[i].check_hands();
        for (int j = 0; j < players[i].cards.size(); j++) {
            cout << "| " << players[i].cards[j].rank << "  " << players[i].cards[j].suit << "| ";
        }
        cout << endl;
        for (int j = 0; j < players[i].potential_hands.size(); j++) {
            if (players[i].potential_hands[j])
                cout << Possible_hand[j] << " : ";
        }
        cout << endl;
    }
    
    return 0;
}



