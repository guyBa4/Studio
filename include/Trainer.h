#pragma once
#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef pair<int, Workout> OrderPair;

class Trainer {
public:
    Trainer(int t_capacity);
    Trainer(const Trainer& other);
    Trainer(Trainer&& other);
    Trainer& operator=(const Trainer& other);
    Trainer& operator=(Trainer&& other);
    Trainer* clone();
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void addCustomer(Customer* customer, vector<OrderPair> orderPairs);
    vector<OrderPair> removeCustomer(int id);
    Customer* getCustomer(int id);
    vector<Customer*>& getCustomers();
    vector<OrderPair>& getOrders();
    void order(const int customer_id, const vector<int> workout_ids, const vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    ~Trainer();
private:
    int capacity;
    bool open;
    vector<Customer*> customersList;
    vector<OrderPair> orderList;
};


#endif
