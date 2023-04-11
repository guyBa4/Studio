#ifndef STUDIO_H_
#define STUDIO_H_
#pragma once
#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


using namespace std;
class BaseAction;
class PrintActionsLog;
class Trainer;
class Workout;

class Studio {
public:
    Studio();
    Studio(const Studio& other);
    Studio(Studio&& other);
    Studio& operator=(const Studio& other);
    Studio& operator=(Studio&& other);
    Studio(const string& configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int ti);
    const vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    vector<Workout>& getWorkoutOptions();
    void pushToActionsLog(BaseAction* action);
    void pushLog(PrintActionsLog* action);
    int currentId;
    ~Studio();
private:
    bool open;
    vector<Trainer*> trainers;
    vector<Workout> workout_options;
    vector<BaseAction*> actionsLog;
    vector<PrintActionsLog*> logsLog;
};

#endif
