#pragma once
#ifndef WORKOUT_H_
#define WORKOUT_H_
#include <iostream>
#include <string>
using namespace std;

enum WorkoutType {
    ANAEROBIC, MIXED, CARDIO
};

class Workout {
public:
    Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type);
    int getId() const;
    string getName() const;
    int getPrice() const;
    WorkoutType getType() const;
    string getTypeString() const;
    Workout& operator=(const Workout& otherWorkout);
private:
    const int id;
    const std::string name;
    const int price;
    const WorkoutType type;
};


#endif