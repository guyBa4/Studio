#include "Studio.h"
#include "Studio.h"
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

Studio::Studio() : currentId(0), open(false), trainers(), workout_options(), actionsLog(), logsLog()
{
}

Studio::Studio(const Studio& other)
  : currentId(other.currentId), open(other.open), trainers(), workout_options(other.workout_options), actionsLog(), logsLog()
{
	for (int i = 0; i < (int)other.trainers.size(); i++)
		trainers.push_back(other.trainers[i]->clone());
	for (int i = 0; i < (int)other.actionsLog.size(); i++)
	{
		actionsLog.push_back(other.actionsLog[i]->clone());
	}
}

Studio::Studio(Studio&& other) 
  : currentId(other.currentId), open(other.open), trainers(other.trainers), workout_options(other.workout_options), actionsLog(other.actionsLog), logsLog()
{
  other.trainers.clear();
  other.actionsLog.clear();
}

Studio& Studio::operator=(const Studio& other)
{
	if (&other != this) {
		this->currentId = other.currentId;
		open = other.open;
		workout_options = other.workout_options;
		
		for (int i = 0; i < (int)trainers.size(); i++)
		{
			delete trainers[i];
		}
		trainers.clear();
		for (int i = 0; i < (int)other.trainers.size(); i++)
			trainers.push_back(other.trainers[i]->clone());
		for (int i = 0; i < (int)actionsLog.size(); i++)
		{
			delete actionsLog[i];
		}
		actionsLog.clear();
		for (int i = 0; i < (int)other.actionsLog.size(); i++)
			actionsLog.push_back(other.actionsLog[i]->clone());
	}
	return *this;
}

Studio& Studio::operator=(Studio&& other) 
{
	open = other.open;
	currentId = other.currentId;
	workout_options = other.workout_options;
	if (&other != this)
	{
		for (int i = 0; i < (int)trainers.size(); i++)
		{
			delete trainers[i];
		}
		trainers.clear();
		for (int i = 0; i < (int)other.trainers.size(); i++)
		{
			Trainer* t = other.trainers[i];
			trainers.push_back(t);
			other.trainers[i] = nullptr;
		}
		for (int i = 0; i < (int)actionsLog.size(); i++)
		{
			delete actionsLog[i];
			actionsLog[i] = nullptr;
		}
		actionsLog.clear();
		for (int i = 0; i < (int)other.actionsLog.size(); i++)
		{
			BaseAction* ba = other.actionsLog[i];
			actionsLog.push_back(ba);
			other.actionsLog[i] = nullptr;
		}

	}
	return *this;
}

Studio::Studio(const string& configFilePath)
	: currentId(0), open(false), trainers(), workout_options(), actionsLog()
{
	ifstream file(configFilePath);
	string line;
	getline(file, line);
	while (line[0] == '#' || line[0] == '\0' || line[0] == '\n'){ 
		getline(file, line);
	}
	getline(file, line);
	while (line[0] == '#' || line[0] == '\0' || line[0] == '\n'){
		getline(file, line);
	}
	string sCap = "";
	int cap;
	int idt = 0;
	for (int i = 0; i <= (int)line.size(); i++) {
	  if (i == (int)line.size() || line[i] == ',') {
			cap = stoi(sCap.c_str());
			Trainer* t = new Trainer(cap);
			idt++;
			trainers.push_back(t);
			sCap = "";
		}
		else
			sCap = sCap + line[i];
	}
	string name;
	string type;
	string price;
	int id = 0;
	while (getline(file, line)) {
		if (line[0] == '#' || line[0] == '\0' || line[0] == '\n')
			continue;
		name = "";
		type = "";
		price = "";

		int i = 0;
		while (line[i] != ',') {			//initiate name
			name = name + line[i];
			i++;
		}
		i++;
		while (line[i] == ' ')
			i++;

		while (line[i] != ',') {			//initiate type
			type = type + line[i];
			i++;
		}
		i++;

		while (line[i] == ' ')
			i++;

		while (i < (int)line.size()) {			//initiate price
			price = price + line[i];
			i++;
		}
		WorkoutType wt;
		if (type.compare("Anaerobic") == 0)
			wt = ANAEROBIC;
		else if (type.compare("Mixed") == 0)
			wt = MIXED;
		else
			wt = CARDIO;
		workout_options.push_back(Workout(id, name, stoi(price.c_str()), wt));
		id++;

	}

	cout << "Studio is now open!" << endl;

}


void Studio::start()
{
	open = true;
	while (open) {
		BaseAction* ba;
		bool legalCommand = false;
		string input;
		getline(cin, input);
		istringstream ss(input);
		vector<string> line;
		string action;
		ss >> action;
		if (action == "open") {
			legalCommand = true;
			string sTrainerId;
			ss >> sTrainerId;
			int trainerId = atoi(sTrainerId.c_str());
			vector<Customer*> customersList;
			string s;
			while (ss >> s) {
				string customer = "";
				Customer* c;

				for (int i = 0; s[i] != ','; i++)
					customer = customer + s[i];

				string strategy = s.substr(s.length() - 3, s.length() - 1);
				if (strategy == "swt")
					c = new SweatyCustomer(customer, currentId);
				else if (strategy == "chp")
					c = new CheapCustomer(customer, currentId);
				else if (strategy == "mcl")
					c = new HeavyMuscleCustomer(customer, currentId);
				else
					c = new FullBodyCustomer(customer, currentId);
				currentId++;

				customersList.push_back(c);
			}
			ba = new OpenTrainer(trainerId, customersList);
		}
		else if (action == "move") {
			legalCommand = true;
			string t1;
			string t2;
			string c;
			ss >> t1;
			ss >> t2;
			ss >> c;
			ba = new MoveCustomer(atoi(t1.c_str()), atoi(t2.c_str()), atoi(c.c_str()));

		}
		else if (action == "order") {
			legalCommand = true;
			string id;
			ss >> id;
			ba = new Order(atoi(id.c_str()));
		}
		else if (action == "status") {
			legalCommand = true;
			string id;
			ss >> id;
			ba = new PrintTrainerStatus(atoi(id.c_str()));
		}
		else if (action == "log") {
			legalCommand = true;
			ba = new PrintActionsLog();
		}
		else if(action == "backup") {
			legalCommand = true;
			ba = new BackupStudio();
		}
		else if (action == "restore") {
			legalCommand = true;
			ba = new RestoreStudio();
		}
		else if(action == "workout_options") {
			legalCommand = true;
			ba = new PrintWorkoutOptions();
		}
		else if (action == "close") {
			legalCommand = true;
			string id;
			ss >> id;
			ba = new Close(atoi(id.c_str()));

		}
		else if (action == "closeall") {
			legalCommand = true;
			ba = new CloseAll();
			open = false;
		}
		if (legalCommand)
			ba->act(*this);
		else cout << "Illegal command" << endl;
	}
}

int Studio::getNumOfTrainers() const
{
	return trainers.size();
}

Trainer* Studio::getTrainer(int ti)
{
  if (ti <(int) trainers.size())
		return trainers[ti];
	return NULL;
}

const vector<BaseAction*>& Studio::getActionsLog() const
{
	return actionsLog;
}

vector<Workout>& Studio::getWorkoutOptions()
{
	return workout_options;
}
void Studio::pushToActionsLog(BaseAction* action)
{
	actionsLog.push_back(action);
}

void Studio::pushLog(PrintActionsLog* action)
{
	logsLog.push_back(action);
}

Studio::~Studio()
{
	for (int i = 0; i < (int)trainers.size(); i++)
		delete trainers[i];
	trainers.clear();
	for (int i = 0; i < (int)actionsLog.size(); i++)
		delete actionsLog[i];
	actionsLog.clear();
	for (int i = 0; i < (int)logsLog.size(); i++)
		delete logsLog[i];
	logsLog.clear();
}
