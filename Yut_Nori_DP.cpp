#include <iostream>
#include <chrono>
using namespace std;

int factorial(int n);
int combinactorics(const int &n, const int &c);
const int DO     = 1;
const int GAE    = 2;
const int GUL    = 3;
const int YUT    = 4;
const int MO     = 5;
const int THEEND = 50;
const double FRONT   = 0.530125;
const double BACK    = 0.469875;
const double DO_Prob     = pow(FRONT, 3) * pow(BACK, 1) * combinactorics(4, 1);
const double GAE_Prob    = pow(FRONT, 2) * pow(BACK, 2) * combinactorics(4, 2);
const double GUL_Prob    = pow(FRONT, 1) * pow(BACK, 3) * combinactorics(4, 3);
const double YUT_Prob    = pow(FRONT, 0) * pow(BACK, 4) * combinactorics(4, 0);
const double MO_Prob     = pow(FRONT, 4) * pow(BACK, 0) * combinactorics(4, 4);
const int NUMCELLS = 29;

class Board {
public:
    Board();
    double comp_probability(int pos1, int pos2, bool p1_turn);
    int position_after_move(int pos, int amount);
private:
    // player 1's win_rate when it's [pos1][pos2] and it's p1's turn
    double dp[NUMCELLS][NUMCELLS];
    bool is_checked[NUMCELLS][NUMCELLS];
};

Board::Board() {
    for (int i = 0; i < NUMCELLS; i++)
        for (int j = 0; j < NUMCELLS; j++)
        {
            dp[i][j] = 0.0;
            is_checked[i][j] = false;
        }
}

// Arguments: Two integer positions of players, one boolean indicating whether it's p1's turn
// Return Value: Probability of player 1 winning based on the input values
double Board::comp_probability(int pos1, int pos2, bool p1_turn)
{
    if (pos1 == THEEND)
        return 1.0;
    else if (pos2 == THEEND)
        return 0.0;
    
    double prob = 0.0;
    // if it's player 1's turn
    if (p1_turn)
    {
        if (is_checked[pos1][pos2])
            return dp[pos1][pos2];
        
        // DO
        prob += comp_probability(position_after_move(pos1, DO), pos2, !p1_turn) * DO_Prob;
        
        // GAE
        prob += comp_probability(position_after_move(pos1, GAE), pos2, !p1_turn) * GAE_Prob;
        
        // GUL
        prob += comp_probability(position_after_move(pos1, GUL), pos2, !p1_turn) * GUL_Prob;
        
        // YUT
        prob += comp_probability(position_after_move(pos1, YUT), pos2, p1_turn) * YUT_Prob;
        
        // MO
        prob += comp_probability(position_after_move(pos1, MO), pos2, p1_turn) * MO_Prob;
        
        is_checked[pos1][pos2] = true;
        return dp[pos1][pos2] = prob;
    }
    
    // if it's player 2's turn
    else
    {
        if (is_checked[pos2][pos1])
            return (1-dp[pos2][pos1]);
        
        // DO
        prob += (1-comp_probability(pos1, position_after_move(pos2, DO), !p1_turn)) * DO_Prob;
        
        // GAE
        prob += (1-comp_probability(pos1, position_after_move(pos2, GAE), !p1_turn)) * GAE_Prob;
        
        // GUL
        prob += (1-comp_probability(pos1, position_after_move(pos2, GUL), !p1_turn)) * GUL_Prob;
        
        // YUT
        prob += (1-comp_probability(pos1, position_after_move(pos2, YUT), p1_turn)) * YUT_Prob;
        
        // MO
        prob += (1-comp_probability(pos1, position_after_move(pos2, MO), p1_turn)) * MO_Prob;
        
        is_checked[pos2][pos1] = true;
        return 1-(dp[pos2][pos1] = prob);
    }
}


int Board::position_after_move(int pos, int amount) {
    if (pos >= 15 && pos <= 19 && (pos + amount) >= 20)
        return THEEND;
    
    if (pos >= 24 && pos <= 28 && (pos + amount) >= 29)
        return THEEND;
    
    if (pos == 5)
    {
        if (amount == GUL)
            return 26;
        else if (amount >= YUT)
            return 18 + amount;
        else
            return 19 + amount;
    }
    if (pos == 10)
        return 23 + amount;
    
    if (pos == 20 || pos == 21)
    {
        if (pos + amount == 22)
            return 26;
        else if (pos + amount < 22)
            return pos + amount;
        else if (pos + amount == 23 || pos + amount == 24)
            return pos + amount - 1;
        else
            return pos + amount - 10;
    }
    
    if (pos == 22 || pos == 23)
    {
        if (pos + amount < 24)
            return pos + amount;
        else
            return pos + amount - 9;
    }
    
    else
        return pos + amount;
}

int factorial (int n) {
    if (n < 0)
        return -1;
    
    if (n == 0)
        return 1;
    
    int res = 1;
    while (n > 0) {
        res *= n;
        n--;
    }
    
    return res;
}

int combinactorics(const int &n, const int &c) {
    if (c > n || n < 0 || c < 0)
        return -1;
    
    return factorial(n)/(factorial(n-c) * factorial(c));
}

int main() {
    Board board;
    int p1, p2;
    
    cout << "Please input two positions" << endl;
    cout << "p1: ";
    cin >> p1;
    cout << "p2: ";
    cin >> p2;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    double prob1 = board.comp_probability(p1, p2, true);
    double prob2 = board.comp_probability(p2, p1, false);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    cout << "p1 win rate: " << prob1 << endl;
    cout << "p2 win rate: " << prob2 << endl;
    cout << "Runtime: " << duration.count() / 1000.0 << " ms" << endl;
}
