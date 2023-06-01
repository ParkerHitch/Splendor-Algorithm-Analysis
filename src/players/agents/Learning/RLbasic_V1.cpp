//
// Created by Parker Hitchcock on 3/16/23.
//

#include "../../Player.h"
#include "../../../utils/Math.h"
#include <float.h>
#include <iostream>
#include <cmath>

float RLBasic_V1::score(GameState& gs, GameAction& ga){
    GameState* test = new GameState(gs);
    test->applyAction(ga);
    float score = evaluate(id, *test);
    delete test;
    return score;
}

float RLBasic_V1::evaluate(int pId, GameState& gs){
    float score = 0;
    for(int i=0; i<4; i++) {
        PlayerState &ps = gs.playerStates[(pId+i)%4]; // weights[0] is weight for current player
        score += weights[i][0] * (float) ps.discounts[0];
        score += weights[i][1] * (float) ps.discounts[1];
        score += weights[i][2] * (float) ps.discounts[2];
        score += weights[i][3] * (float) ps.discounts[3];
        score += weights[i][4] * (float) ps.discounts[4];
        score += weights[i][5] * (float) ps.balance0;
        score += weights[i][6] * (float) ps.balance1;
        score += weights[i][7] * (float) ps.balance2;
        score += weights[i][8] * (float) ps.balance3;
        score += weights[i][9] * (float) ps.balance4;
        score += weights[i][10] * (float) ps.balanceY;
        score += weights[i][11] * (float) ps.pts;
        int nReserved = (ps.reservedCards[0]==nullptr) + (ps.reservedCards[1]==nullptr) + (ps.reservedCards[2]==nullptr);
        int reservedPts = (ps.reservedCards[0]==nullptr?0:ps.reservedCards[0]->points) +
                          (ps.reservedCards[1]==nullptr?0:ps.reservedCards[1]->points) +
                          (ps.reservedCards[2]==nullptr?0:ps.reservedCards[2]->points);
        score += weights[i][12] * (float) nReserved;
        score += weights[i][13] * (float) reservedPts;
    }
    return exp(score);
}

probabilityDistr RLBasic_V1::generateProbablities(GameState &gs){
    double* probs = new double[gs.possibleActions.size()];
    double total = 0;
    int i = 0;
    for(GameAction ga: gs.possibleActions) {
        float p = score(gs, ga);
        total+=p;
        probs[i] = p;
        i++;
    }
    probs[0]/=total;
    for(int j=1; j<i;j++) {
        probs[j] = probs[j - 1] + (probs[j] / total);
    }
    return probabilityDistr{i, probs};
}

GameAction RLBasic_V1::takeAction(GameState &gs) {
    probabilityDistr pd = generateProbablities(gs);
    double p = (double)randRange(0, 100000000)/(double)100000000;
    for(int i=0; i<pd.size; i++){
        if(p<pd.start[i]){
            return gs.possibleActions[i];
        }
    }
    return gs.possibleActions[gs.possibleActions.size()-1];
}

void RLBasic_V1::readWeights() {
    ifstream weightFile(assetDir + "weights.txt");
    weightFile.seekg(0,ios::beg);
    std::string wLine;
    getline(weightFile, wLine); //Skip first label
    getline(weightFile, wLine);
    RLBasic_V1::learningConst = stof(wLine);
    getline(weightFile, wLine); //Skip 2nd label
    for(auto & p : RLBasic_V1::weights){
        for(float & w : p){
            getline(weightFile, wLine);
            w = stof(wLine);
        }
    }
    weightFile.close();
}

float distr(float t, float n){
    return exp(-0.5 * pow((t-n/2)/n/4,2));
}

void RLBasic_V1::updateWeights(int pId, gameReplay &replay, float result){
    for(int t=0; t<replay.allActionsTaken.size(); t++) {
        replay.baseGS.applyAction(replay.allActionsTaken[t]);
        float error = distr(t, replay.allActionsTaken.size())*learningConst * (result - evaluate(pId, replay.baseGS));
        for(int i=0; i<4; i++) {
            PlayerState &ps = replay.baseGS.playerStates[(pId+i)%4]; // weights[0] is weight for current player
            weights[i][0] += error * (float) ps.discounts[0];
            weights[i][1] += error * (float) ps.discounts[1];
            weights[i][2] += error * (float) ps.discounts[2];
            weights[i][3] += error * (float) ps.discounts[3];
            weights[i][4] += error * (float) ps.discounts[4];
            weights[i][5] += error * (float) ps.balance0;
            weights[i][6] += error * (float) ps.balance1;
            weights[i][7] += error * (float) ps.balance2;
            weights[i][8] += error * (float) ps.balance3;
            weights[i][9] += error * (float) ps.balance4;
            weights[i][10] += error * (float) ps.balanceY;
            weights[i][11] += error * (float) ps.pts;
            int nReserved = (ps.reservedCards[0]==nullptr) + (ps.reservedCards[1]==nullptr) + (ps.reservedCards[2]==nullptr);
            int reservedPts = (ps.reservedCards[0]==nullptr?0:ps.reservedCards[0]->points) +
                              (ps.reservedCards[1]==nullptr?0:ps.reservedCards[1]->points) +
                              (ps.reservedCards[2]==nullptr?0:ps.reservedCards[2]->points);
            weights[i][12] += error * (float) nReserved;
            weights[i][13] += error * (float) reservedPts;
        }
    }
    RLBasic_V1::writeWeights();
}

void RLBasic_V1::writeWeights() {
    ofstream weightFile(assetDir+"weights.txt");
    weightFile<<"Learning const:\n";
    weightFile<<learningConst<<"\n";
    weightFile<<"Weights:\n";
    for(auto & p : RLBasic_V1::weights){
        for(float & w : p){
            weightFile << w << "\n";
        }
    }
    weightFile.close();
}
//
//RLBasic_V1::RLBasic_V1(int i) : Player(i) {
//    weights;
//}
