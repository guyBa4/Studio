#include "Customer.h"


Customer::Customer(string c_name, int c_id) : name(c_name), id(c_id)
{
}

string Customer::getName() const
{
	return name;
}

int Customer::getId() const
{
	return id;
}

Customer::~Customer()
{
}

SweatyCustomer::SweatyCustomer(string name, int id) : Customer(name, id)
{
}

SweatyCustomer* SweatyCustomer::clone()
{
	return new SweatyCustomer(*this);
}

vector<int> SweatyCustomer::order(const vector<Workout>& workout_options)
{
	vector<int> order;
	for (int i = 0; i < (int)workout_options.size(); i++)
	{
		if (workout_options[i].getType() == CARDIO)
			order.push_back((int)workout_options[i].getId());
	}
	return order;
}

string SweatyCustomer::toString() const
{
	return this->getName() + ",swt";
}

CheapCustomer::CheapCustomer(string name, int id) : Customer(name, id)
{
}

CheapCustomer* CheapCustomer::clone()
{
	return new CheapCustomer(*this);
}

vector<int> CheapCustomer::order(const vector<Workout>& workout_options)
{
	vector<int> order;
	int cheapestPrice = (int)workout_options.at(0).getPrice();
	int cheapestId = (int)workout_options.at(0).getId();
	for (int i = 1; i < (int)workout_options.size(); i++)
	{
		if ((int)workout_options.at(i).getPrice() < cheapestPrice)
			cheapestId = (int)workout_options.at(i).getId();
	}
	order.push_back(cheapestId);
	return order;
}

string CheapCustomer::toString() const
{
	return this->getName() + ",chp";
}

HeavyMuscleCustomer::HeavyMuscleCustomer(string name, int id) : Customer(name, id)
{
}

HeavyMuscleCustomer* HeavyMuscleCustomer::clone()
{
	return new HeavyMuscleCustomer(*this);
}

vector<int> HeavyMuscleCustomer::order(const vector<Workout>& workout_options)
{
	vector<Workout*> w;
	vector<int> order;
	for (int i = 0; i < (int)workout_options.size(); i++)
	{
		if (workout_options[i].getType() == ANAEROBIC)
		{
			Workout* work = new Workout(workout_options[i]);
			w.push_back(work);
		}
	}
	for (int i = 0; i < (int)w.size(); i++)
	{
		int max = 0;
		int index = 0;
		int id = 0;
		for (int j = 0; j < (int)w.size(); j++)
		{
			if (w[j] != nullptr && (int)w[j]->getPrice() > max)
			{
				index = j;
				max = (int)w[j]->getPrice();
			}
		}
		id = (int)w[index]->getId();
		delete w[index];
		w[index] = nullptr;
		order.push_back(id);
	}

	return order;
}

string HeavyMuscleCustomer::toString() const
{
	return this->getName() + ",mcl";
}


FullBodyCustomer::FullBodyCustomer(string name, int id) : Customer(name, id)
{
}

FullBodyCustomer* FullBodyCustomer::clone()
{
	return new FullBodyCustomer(*this);
}

vector<int> FullBodyCustomer::order(const vector<Workout>& workout_options)
{
	vector<Workout*> anaerobic_ptr;
	vector<Workout*> mixed_ptr;
	vector<Workout*> cardio_ptr;
	vector<int> ids;
	for (int i = 0; i < (int)workout_options.size(); i++)
	{
		Workout* w = new Workout(workout_options[i]);
		if (w->getType() == ANAEROBIC)
			anaerobic_ptr.push_back(w);
		if (w->getType() == MIXED)
			mixed_ptr.push_back(w);
		if (w->getType() == CARDIO)
			cardio_ptr.push_back(w);
	}

	int cardio_min = 0;
	int cardio_id = -1;

	if (!cardio_ptr.empty())
	{
		cardio_min = (int)cardio_ptr[0]->getPrice();
		cardio_id = (int)cardio_ptr[0]->getId();
		delete cardio_ptr[0];
		cardio_ptr[0] = nullptr;
	}
	for (int i = 1; i < (int)cardio_ptr.size(); i++)
	{
		if ((int)cardio_ptr[i]->getPrice() < cardio_min)
		{
			cardio_min = (int)cardio_ptr[i]->getPrice();
			cardio_id = (int)cardio_ptr[i]->getId();
		}
		delete cardio_ptr[i];
		cardio_ptr[i] = nullptr;
	}

	int mixed_max = 0;
	int mixed_id = -1;

	for (int i = 0; i < (int)mixed_ptr.size(); i++)
	{
		if ((int)mixed_ptr[i]->getPrice() > mixed_max)
		{
			mixed_max = (int)mixed_ptr[i]->getPrice();
			mixed_id = (int)mixed_ptr[i]->getId();
		}
		delete mixed_ptr[i];
		mixed_ptr[i] = nullptr;
	}

	int anaerobic_min = 0;
	int anaerobic_id = -1;

	if (!anaerobic_ptr.empty())
	{
		anaerobic_min = (int)anaerobic_ptr[0]->getPrice();
		anaerobic_id = (int)anaerobic_ptr[0]->getId();
		delete anaerobic_ptr[0];
		anaerobic_ptr[0] = nullptr;
	}
	for (int i = 1; i < (int)anaerobic_ptr.size(); i++)
	{
		if ((int)anaerobic_ptr[i]->getPrice() < anaerobic_min)
		{
			anaerobic_min = (int)anaerobic_ptr[i]->getPrice();
			anaerobic_id = (int)anaerobic_ptr[i]->getId();
		}
		delete anaerobic_ptr[i];
		anaerobic_ptr[i] = nullptr;
	}

	if (cardio_id >= 0)
		ids.push_back(cardio_id);
	if (mixed_id >= 0)
		ids.push_back(mixed_id);
	if (anaerobic_id >= 0)
		ids.push_back(anaerobic_id);
	if (ids.size() < 3)
		ids.clear();
	return ids;
}

string FullBodyCustomer::toString() const
{
	return this->getName() + ",fbd";
}
