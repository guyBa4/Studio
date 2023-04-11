#pragma once
#ifndef CUSTOMER_H_
#define CUSTOMER_H_
#include <vector>
#include <string>
#include <algorithm>
#include "Workout.h"
using namespace std;


class Customer {
public:
    Customer(string c_name, int c_id);
    virtual Customer* clone() = 0;
    virtual vector<int> order(const vector<Workout>& workout_options) = 0;
    virtual string toString() const = 0;
    string getName() const;
    int getId() const;
    virtual ~Customer();
private:
    const string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(string name, int id);
    virtual SweatyCustomer* clone();
    virtual vector<int> order(const vector<Workout>& workout_options);
    virtual string toString() const;
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(string name, int id);
    virtual CheapCustomer* clone();
    virtual vector<int> order(const vector<Workout>& workout_options);
    virtual string toString() const;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(string name, int id);
    virtual HeavyMuscleCustomer* clone();
    virtual vector<int> order(const vector<Workout>& workout_options);
    virtual string toString() const;
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(string name, int id);
    virtual FullBodyCustomer* clone();
    virtual vector<int> order(const vector<Workout>& workout_options);
    virtual string toString() const;
private:
};


#endif
