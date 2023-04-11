#include "Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) : id(w_id), name(w_name), price(w_price), type(w_type)
{
}

int Workout::getId() const
{
	return id;
}

std::string Workout::getName() const
{
	return name;
}

int Workout::getPrice() const
{
	return price;
}

WorkoutType Workout::getType() const
{
	return type;
}

string Workout::getTypeString() const
{
	string s = "";
	if (getType() == CARDIO)
		s += "Cardio";
	else if (getType() == ANAEROBIC)
		s += "Anaerobic";
	else s += "Mixed";
	return s;
}

Workout& Workout::operator=(const Workout& otherWorkout)
{
	return *this;
}
