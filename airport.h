#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <fstream>
#include "person.h"
#include "crew.h"
#include "flight.h"
using namespace std;

class Airport
{
private:
    string Name;
    string Location;
    int Runways;

    Employee **Employees;  //array of Employee pointers
    CrewTeam **CrewTeams;
    Flight **Flights;

    int employeecapacity;
    int employeecount;
    int crewTeamCapacity;
    int crewcount;
    int flightCapacity;
    int flightCount;

    string datafilepath;

public:
    Airport(const string &name, const string &location, int runways);
    ~Airport();

    Airport(const Airport &) = delete;
    Airport &operator=(const Airport &) = delete;

    void AddFlight(int flightID, const string &origin, const string &destination, const string &aircraft);
    void updateFlightStatus(int flightID, const string &newStatus);
    void AssignCrewToFlight(int flightID, CrewTeam *crew);
    void addPassengerToFlight(int flightID, Passenger *passenger);

    void assignEmployee(Employee *employee, const string &teamID = "");
    void displayAllEmployees() const;

    void createCrewTeam(const string &teamID, const string &teamName);
    CrewTeam *findCrewTeam(const string &teamID) const;
    void displayAllCrewTeams() const;

    void displaySummary() const;

    void saveToFile(const string &filename = "airport_data.txt") const;
    void loadFromFile(const string &filename = "airport_data.txt");
    void setDataFilePath(const string &path);

    void autoSave() const;

    string getName() const;
    string getLocation() const;
    int getRunways() const;
    int getFlightCount() const;
    Flight *getFlight(int index) const;
    Employee *getEmployee(int index) const;
    int getemployeecount() const;
    CrewTeam *getCrewTeam(int index) const;
    int getCrewCount() const;
};

#endif
