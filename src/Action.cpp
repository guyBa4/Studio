#include "Action.h"
#include <iostream>
#include <string>
#include <sstream>

BaseAction::BaseAction()
	: errorMsg(""), status(ERROR)
{
}

BaseAction::BaseAction(const BaseAction& other)
	: errorMsg(other.errorMsg), status(other.status)
{
}

ActionStatus BaseAction::getStatus() const
{
	return status;
}

void BaseAction::complete()
{
	status = COMPLETED;
}

void BaseAction::error(string errorMsg)
{
	status = ERROR;
	this->errorMsg = errorMsg;
	cout << "Error: " << errorMsg << endl;
}

string BaseAction::getErrorMsg() const
{
	return errorMsg;
}

BaseAction::~BaseAction()
{
}

OpenTrainer::OpenTrainer(int id, vector<Customer*>& customersList)
	: BaseAction(), trainerId(id), customers(customersList)
{
	/*
        for (int i = 0; i < (int)customersList.size(); i++)
	{
		customers.push_back(customersList[i]->clone());
		cout << customers[i]->getName() << endl;
	}
*/

}

OpenTrainer::OpenTrainer(const OpenTrainer& other)
	: BaseAction(other), trainerId(other.trainerId), customers()
{
	for (int i = 0; i < (int)other.customers.size(); i++)
		customers.push_back(other.customers[i]->clone());
}



void OpenTrainer::act(Studio& studio)
{
	Trainer* t = studio.getTrainer(trainerId);
	if (t == NULL || t->isOpen())
	{
		error("Workout session does not exist or is already open.");
		for (int i = 0; i < (int)customers.size(); i++)
		{
			delete customers[i];
		}
		customers.clear();
	}
		
	else if (t->getCapacity() < (int)customers.size())
	{
		for (int i = 0; i < (int)t->getCapacity(); i++)
		{
			t->addCustomer(customers[i]);
		}	
		for (int i = (int)customers.size() - 1; i >= (int)t->getCapacity(); i--)
		{
			delete customers[i];
			customers[i] = nullptr;
			customers.pop_back();
		}
		
		t->openTrainer();
		complete();
	}	
	else
	{
		for (int i = 0; i < (int)customers.size(); i++)
		{
			t->addCustomer(customers[i]->clone());
		}
		t->openTrainer();
		complete();
	}
	studio.pushToActionsLog(this);
}

string OpenTrainer::toString() const
{
	string s = "open " + to_string(trainerId);
	for (int i = 0; i < (int)customers.size(); i++)
		s = s + " " + customers[i]->toString();
	if (getStatus() == COMPLETED)
		s = s + " Completed";
	if (getStatus() == ERROR)
		s = s + " Error: " + getErrorMsg();
	return s;
}

OpenTrainer* OpenTrainer::clone()
{
	return new OpenTrainer(*this);
}

OpenTrainer::~OpenTrainer()
{
       	for (int i = 0; i < (int)customers.size(); i++)
	    delete customers[i];
	customers.clear();
}


Order::Order(int id)
	: BaseAction(), trainerId(id)
{
}

void Order::act(Studio& studio)
{
	Trainer* t = studio.getTrainer(trainerId);
	if (t == NULL || !t->isOpen())
		error("Trainer does not exist or is not open");
	else
	{
		for (int i = 0; i < (int)t->getCustomers().size(); i++)
		{
			Customer* c = t->getCustomers()[i];
			vector<int> workout_ids = c->order(studio.getWorkoutOptions());
			t->order(c->getId(), workout_ids, studio.getWorkoutOptions());
			for (int j = 0; j < (int)workout_ids.size(); j++)
			{
				string w = "";
				for (int k = 0; k < (int)studio.getWorkoutOptions().size(); k++)
				{
					if ((int)studio.getWorkoutOptions()[k].getId() == (int)workout_ids[j])
						w += studio.getWorkoutOptions()[k].getName();
				}
				cout << c->getName() << " Is Doing " << w << endl;
			}
		}
		complete();
	}
	studio.pushToActionsLog(this);
}

string Order::toString() const
{
	string s = "order " + to_string(trainerId);
	if (getStatus() == COMPLETED)
		s = s + " Completed";
	if (getStatus() == ERROR)
		s = s + " Error: " + getErrorMsg();
	return s;
}

Order* Order::clone()
{
	return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId)
	: BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId)
{
}

void MoveCustomer::act(Studio& studio)
{
	Trainer* src = studio.getTrainer(srcTrainer);
	Trainer* dst = studio.getTrainer(dstTrainer);
	if (src == nullptr || dst == nullptr || !src->isOpen() || !dst->isOpen() || (int)dst->getCustomers().size() == (int)dst->getCapacity())
		error("Cannot move customer");
	else
	{
		Customer* c = src->getCustomer(id);
		if (c == nullptr)
			error("Cannot move customer");
		else
		{
			vector<OrderPair> op = src->removeCustomer(id);
			if (src->getCustomers().empty())
			{
				Close* close = new Close(srcTrainer);
				close->act(studio);
			}
			dst->addCustomer(c, op);
			complete();
		}
	}
	studio.pushToActionsLog(this);
}

string MoveCustomer::toString() const
{
	string s = "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id);
	if (getStatus() == COMPLETED)
		s = s + " Completed";
	if (getStatus() == ERROR)
		s = s + " Error: " + getErrorMsg();
	return s;
}

MoveCustomer* MoveCustomer::clone()
{
	return new MoveCustomer(*this);
}

Close::Close(int id)
	: BaseAction(), trainerId(id)
{
}

void Close::act(Studio& studio)
{
	Trainer* t = studio.getTrainer(trainerId);
	if (t == NULL || !t->isOpen())
		error("Trainer does not exist or is not open");
	else
	{
		cout << "Trainer " + to_string(trainerId) + " closed. Salary " + to_string(t->getSalary()) + "NIS" << endl;
		t->closeTrainer();
		complete();
	}
	studio.pushToActionsLog(this);
}

string Close::toString() const
{
	string s = "close " + to_string(trainerId);
	if (getStatus() == COMPLETED)
		s = s + " Completed";
	if (getStatus() == ERROR)
		s = s + " Error: " + getErrorMsg();
	return s;
}

Close* Close::clone()
{
	return new Close(*this);
}

CloseAll::CloseAll()
	: BaseAction()
{
}

void CloseAll::act(Studio& studio)
{
	for (int i = 0; i < (int)studio.getNumOfTrainers(); i++)
	{
		Trainer* t = studio.getTrainer(i);
		if (t != NULL && t->isOpen())
		{
				Close* close = new Close(i);
				close->act(studio);
		}
	}
	complete();
	cout << "Studio is now closed!" << endl;
	studio.pushToActionsLog(this);
}

string CloseAll::toString() const
{
	string s = "closeall";
	if (getStatus() == COMPLETED)
		s = s + " Completed";
	if (getStatus() == ERROR)
		s = s + " Error: " + getErrorMsg();
	return s;
}

CloseAll* CloseAll::clone()
{
	return new CloseAll(*this);
}

PrintWorkoutOptions::PrintWorkoutOptions()
	: BaseAction()
{
}

void PrintWorkoutOptions::act(Studio& studio)
{
	for (Workout w : studio.getWorkoutOptions())
		cout << w.getName() << ", " << w.getTypeString() << ", " << to_string(w.getPrice()) << endl;
	complete();
	studio.pushToActionsLog(this);
}

string PrintWorkoutOptions::toString() const
{
	string s = "workout_options";
	if (getStatus() == COMPLETED)
		s = s + " Completed";
	if (getStatus() == ERROR)
		s = s + " Error: " + getErrorMsg();
	return s;
}

PrintWorkoutOptions* PrintWorkoutOptions::clone()
{
	return new PrintWorkoutOptions(*this);
}

PrintTrainerStatus::PrintTrainerStatus(int id)
	: BaseAction(), trainerId(id)
{
}

void PrintTrainerStatus::act(Studio& studio)
{
	Trainer* t = studio.getTrainer(trainerId);
	if (t->isOpen())
	{
		cout << "Trainer " << to_string(trainerId) << " status: open" << endl;
		cout << "Customers:" << endl;
		for (int i = 0; i < (int)t->getCustomers().size(); i++)
		{
			Customer* c = t->getCustomers()[i];
			cout << c->getId() << " " << c->getName() << endl;
			c = nullptr;
		}
		cout << "Orders:" << endl;
		for (int i = 0; i < (int)t->getOrders().size(); i++)
		{
			OrderPair op = t->getOrders()[i];
			cout << op.second.getName() << " " << op.second.getPrice() << "NIS " << op.first << endl;
		}
		cout << "Current Trainer's Salary: " << t->getSalary() << "NIS" << endl;
	}
	else
		cout << "Trainer " << trainerId << " status: closed" << endl;
	complete();
	studio.pushToActionsLog(this);
}

string PrintTrainerStatus::toString() const
{
	string s = "status " + to_string(trainerId);
	if (getStatus() == COMPLETED)
		s = s + " Completed";
	if (getStatus() == ERROR)
		s = s + " Error: " + getErrorMsg();
	return s;
}

PrintTrainerStatus* PrintTrainerStatus::clone()
{
	return new PrintTrainerStatus(*this);
}

PrintActionsLog::PrintActionsLog()
	: BaseAction()
{
}

void PrintActionsLog::act(Studio& studio)
{
	for (int i = 0; i < (int)studio.getActionsLog().size(); i++)
		cout << studio.getActionsLog()[i]->toString() << endl;
	complete();
	studio.pushLog(this);
}

string PrintActionsLog::toString() const
{
	return "log";
}

PrintActionsLog* PrintActionsLog::clone()
{
	return new PrintActionsLog(*this);
}

BackupStudio::BackupStudio()
	: BaseAction()
{
}

void BackupStudio::act(Studio& studio)
{
	if (backup != nullptr)
	{
		delete backup;
		backup = nullptr;
	}
	studio.pushToActionsLog(this);
	complete();
	backup = new Studio(studio);
}

string BackupStudio::toString() const
{
	string s = "backup";
	if (getStatus() == COMPLETED)
		s += " Completed";
	if (getStatus() == ERROR)
		s += " Error: " + getErrorMsg();
	return s;
}

BackupStudio* BackupStudio::clone()
{
	return new BackupStudio(*this);
}

RestoreStudio::RestoreStudio()
	: BaseAction()
{
}

void RestoreStudio::act(Studio& studio)
{
	if (backup == nullptr)
		error("No backup available");
	else
	{
		studio = *backup;
		complete();
	}
	studio.pushToActionsLog(this);
}

string RestoreStudio::toString() const
{
	string s = "restore";
	if (getStatus() == COMPLETED)
		s += " Completed";
	if (getStatus() == ERROR)
		s += " Error: " + getErrorMsg();
	return s;
}

RestoreStudio* RestoreStudio::clone()
{
	return new RestoreStudio(*this);
}
