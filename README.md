# Overview
This project is a C++ program that calculates the win probability for two players in a simplified version of [Yut Nori]([url](https://en.wikipedia.org/wiki/Yunnori)), a traditional Korean board game. 

This simulator uses dynamic programming to compute the probability of Player 1 winning given the starting positions of both players.

The program models the game mechanics, including the probabilities of different moves (DO, GAE, GUL, YUT, MO), the board's unique paths and shortcuts, and the win condition (returnin back to position 0). It also measures the runtime of the computation for performance analysis.

# Yut Nori Board
![Yut-nori Map (9)](https://github.com/user-attachments/assets/44e4195c-16d5-4efd-9df1-6abeda382978)

# Sample Input/Output
<img width="464" alt="Sample Output" src="https://github.com/user-attachments/assets/08944bab-004f-46a3-80fe-8d11d1829af8" />

# Game Rule
Each player throws four yuts on their turn and move their piece according to the outcome.

<img src="https://github.com/user-attachments/assets/0ccc0ea1-7d54-44f6-96ae-2f791692b48d" width=600>

### Special Rule
If a piece is standing on the edge (5, 10, or 26), they get to take the shortcut.<br>
ex) Is a piece is standing on 5 and the current result is Gae (Move 2 steps), they can take a shortcut by moving towards 21.

![Yut-nori Map (10)](https://github.com/user-attachments/assets/32971883-3a3c-4fec-9940-7882e37cc8d4)

# Implementation

### DFS Algorithm & Dynamic Programming
```
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
...
```

The function recursively called itself with modified arguments to test all five possible outcomes (Do, Gae, Gul, Yut, and Mo) in a given circumstance.

Then, it adds up all the probabilities and store it to corresponding cell in dp array. It also marks the corresponding is_checked array to be true<br>
Later on, if the is_checked is true, the code just uses the value stored in dp array without calculating all different combinations again (reduces redundant calculations)
