//
// Created by lee on 17-7-27.
//

#ifndef HETSNETS_CONFIGURATION_H
#define HETSNETS_CONFIGURATION_H

#include <iostream>
#include <map>
#include "MySQLManager.h"

using namespace std;

class shape_par
{
public:
    static shape_par& GetInstance();
    void set_shape();
    string get_shape();
    double get_length();
    double get_width();
    double get_radius();

    shape_par();
    ~shape_par();

private:
    static shape_par* _Instance;
    string shape;
    double length;
    double width;
    double radius;
};

class mode_par
{
public:
    static mode_par& GetInstance();
    static mode_par& GetInstance(string mode);
    void set_mode();
    int get_priority();
    string get_mode();
    int get_modeID();
    int get_numOfTx();
    double get_radius();
    double get_power();
    int get_numOfRx();
    vector<string> get_all_mode();


    mode_par();
    mode_par(string mode);
    ~mode_par();

private:
    static mode_par* _Instance;
    string mode;
    int modeID;
    int priority;
    int numOfTx;
    double radius;
    double power;
    int numOfRx;
    vector<string> all_mode;
    vector<string> all_mode_par;
};

class sense_par
{
public:
    static sense_par& GetInstance();
    void set_sense();
    int get_sence() const;
    int get_numOfSlot() const;
    int getNumOfSlot() const;
    int getNumOfSample() const;
    int getRankOfSymbol() const;
    int getNumOfMainBS() const;
    int getNumOfCooperationBS() const;
    double getFalseAlarmProbability() const;
    double getMinSINR() const;
    double getMaxSINR() const;

    sense_par();
    ~sense_par();

private:
    static sense_par* _Instance;
    int sense; //做频谱感知仿真时置为1
    int numOfSlot;
    int numOfSample;
    int rankOfSymbol;
    string SenseMainBS;
    int numOfMainBS;
    string SenseCooperationBS;
    int numOfCooperationBS;
    double falseAlarmProbability;
    double minSINR;
    double maxSINR;
};

#endif //HETSNETS_CONFIGURATION_H
