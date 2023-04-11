#include "Trainer.h"


Trainer::Trainer(int t_capacity)
  : capacity(t_capacity), open(false), customersList(), orderList()
{
}

Trainer::Trainer(const Trainer& other)
  : capacity(other.capacity), open(other.open), customersList(), orderList()
{
	for (int i = 0; i < (int)other.customersList.size(); i++)
	{
		Customer* customerClone = other.customersList[i]->clone();
		customersList.push_back(customerClone);
	}
	for (int i = 0; i < (int)other.orderList.size(); i++)
	{
		orderList.push_back(other.orderList[i]);
	}

}

Trainer::Trainer(Trainer&& other)
	: capacity(other.capacity), open(other.open), customersList(other.customersList), orderList(other.orderList)
{
	other.customersList.clear();
}

Trainer& Trainer::operator=(const Trainer& other)
{
	if (&other != this)
	{
		capacity = other.capacity;
		open = other.open;
		orderList = other.orderList;
		for (int i = 0; i < (int)customersList.size(); i++)
		{
			delete customersList[i];
			customersList[i] = nullptr;
		}
		customersList.clear();
		for (int i = 0; i < (int)other.customersList.size(); i++)
		{
			customersList.push_back(other.customersList[i]->clone());
		}
	}
	
	return *this;
}

Trainer& Trainer::operator=(Trainer&& other)
{
	if (&other != this) {
		capacity = other.capacity;
		open = other.open;
		orderList = other.orderList;
		for (int i = 0; i < (int)customersList.size(); i++)
		{
			delete customersList[i];
		}
		customersList.clear();
		customersList = other.customersList;
		other.customersList.clear();
	}
	return *this;
}

Trainer* Trainer::clone()
{
	return new Trainer(*this);
}

int Trainer::getCapacity() const
{
	return capacity;
}

void Trainer::addCustomer(Customer* customer)
{
	customersList.push_back(customer);
}

void Trainer::addCustomer(Customer* customer, vector<OrderPair> orderPairs)
{
	addCustomer(customer);
	for (int i = 0; i < (int)orderPairs.size(); i++)
		orderList.push_back(orderPairs[i]);
}

vector<OrderPair> Trainer::removeCustomer(int id)
{
	vector<OrderPair> orderPairs;
	for (int i = 0; i < (int)customersList.size(); i++) {
		if ((int)customersList[i]->getId() == id)
			customersList.erase(customersList.begin() + i);
	}
	for (int i = 0; i < (int)orderList.size(); i++)
	{
		if (orderList[i].first == id)
		{
			OrderPair op = orderList[i];
			orderPairs.push_back(op);
			orderList.erase(orderList.begin() + i);
			i--;
		}
	}
	return orderPairs;
}

Customer* Trainer::getCustomer(int id)
{
	for (int i = 0; i < (int)customersList.size(); i++)
	{
		if ((int)customersList[i]->getId() == id)
			return customersList[i];
	}
	return NULL;
}

vector<Customer*>& Trainer::getCustomers()
{
	return customersList;
}

vector<OrderPair>& Trainer::getOrders()
{
	return orderList;
}

void Trainer::order(const int customer_id, const vector<int> workout_ids, const vector<Workout>& workout_options)
{
	for (int i = 0; i < (int)workout_ids.size(); i++)
	{
		for (int j = 0; j < (int)workout_options.size(); j++)
		{
			if ((int)workout_options[j].getId() == (int)workout_ids[i])
			{
				Workout w = workout_options[j];
				orderList.push_back(OrderPair(customer_id, w));
			}
		}
	}
}
void Trainer::openTrainer()
{
	open = true;
}

void Trainer::closeTrainer()
{
	open = false;
	for (int i = 0; i < (int)customersList.size(); i++)
	  delete customersList[i];
	customersList.clear();
	orderList.clear();
}

int Trainer::getSalary()
{
	int salary = 0;
	for (int i = 0; i < (int)orderList.size(); i++)
	{
	  salary += (int)orderList[i].second.getPrice();
	}
	return salary;
}

bool Trainer::isOpen()
{
	return open;
}

Trainer::~Trainer()
{
	for (int i = 0; i < (int)customersList.size(); i++)
		delete customersList[i];
	customersList.clear();
}
