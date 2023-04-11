#pragma once
#ifndef ACTION_H_
#define ACTION_H_
#include <string>
#include <iostream>
#include <vector>
#include "Studio.h"
#include "Customer.h"
using namespace std;

enum ActionStatus {
    COMPLETED, ERROR
};

//Forward declaration
class Studio;
extern Studio* backup;

class BaseAction {
public:
    BaseAction();
    BaseAction(const BaseAction& other);
    ActionStatus getStatus() const;
    virtual void act(Studio& studio) = 0;
    virtual string toString() const = 0;
    virtual BaseAction* clone() = 0;
    virtual ~BaseAction();
protected:
    void complete();
    void error(string errorMsg);
    string getErrorMsg() const;
private:
    string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, vector<Customer*>& customersList);
    OpenTrainer(const OpenTrainer& other);
    void act(Studio& studio);
    string toString() const;
    OpenTrainer* clone();
    virtual ~OpenTrainer();
private:
    const int trainerId;
    vector<Customer*> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio& studio);
    string toString() const;
    Order* clone();
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio& studio);
    string toString() const;
    MoveCustomer* clone();
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio& studio);
    string toString() const;
    Close* clone();
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio& studio);
    string toString() const;
    CloseAll* clone();
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio& studio);
    string toString() const;
    PrintWorkoutOptions* clone();
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio& studio);
    string toString() const;
    PrintTrainerStatus* clone();
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio& studio);
    string toString() const;
    PrintActionsLog* clone();
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio& studio);
    string toString() const;
    BackupStudio* clone();
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio& studio);
    string toString() const;
    RestoreStudio* clone();
};


#endif
