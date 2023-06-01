//
// Created by Parker Hitchcock on 11/14/22.
//

#ifndef RESEARCH_PLAYER_H
#define RESEARCH_PLAYER_H

#include "../game/gameDependencies.h"
#include "agents/common/tree.h"

struct probabilityDistr {
    int size;
    double* start;
};

class Player {
public:
    int id;
    explicit Player(int i);
    virtual GameAction takeAction(GameState& gs)=0;
    virtual ~Player() = default;
    virtual void updateState(GameAction& ga){};
    static string name(){return "BROKEN";};
};

class HumanPlayer: public Player {
public:
    using Player::Player;
    GameAction takeAction(GameState& gs) override;
    static string name();
};

class RandomPlayer: public Player {
public:
    using Player::Player;
    GameAction takeAction(GameState& gs) override;
    static string name();
};

class OSLA_V1: public Player {
public:
    using Player::Player;
    GameAction takeAction(GameState& gs) override;
    static string name();

    float evaluate(GameState &gs, GameAction& ga);
};

class RLBasic_V1: public Player {
public:
    using Player::Player;
//    explicit RLBasic_V1(int i);
    GameAction takeAction(GameState& gs) override;
    static string name() { return "RLBASIC_V1"; };

    static float evaluate(int pId, GameState &gs);
    float score(GameState &gs, GameAction& ga);
    probabilityDistr generateProbablities(GameState &gs);

    static float weights[4][14];
    static float learningConst;
    static string assetDir;

    static void readWeights();
    static void updateWeights(int pId, gameReplay &replay, float result);
    static void writeWeights();

};

class OSLA_V2: public Player {
public:
    using Player::Player;
    GameAction takeAction(GameState& gs) override;
    static string name();

    float evaluate(GameState &gs, GameAction& ga);
};

class MiniMax: public Player {
public:
    using Player::Player;
    static string name();

    GameAction takeAction(GameState& gs) override;
    float minimax(GameState* gs, int depth);
private:
    int d=0;
};

class MonteCarlo: public Player {
public:
    using Player::Player;
    MonteCarlo(int id, float explorationParam, int nSims);
    MonteCarlo(int id);

    static string name();

    GameAction takeAction(GameState& gs) override;
    void updateState(GameAction& ga) override;
    void printTree();
private:

    int nGames;
    const float expParam;

    static Player* simulatedPlayers[4];
    float UCB(node*n);
    int d=0;
    tree t{};
};

#endif //RESEARCH_PLAYER_H
