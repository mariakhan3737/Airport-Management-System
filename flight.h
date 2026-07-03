#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include "crew.h"
#include "person.h"
using namespace std;

class Flight
{
private:
    int flightID;
    string origin;
    string destination;
    string status;
    string aircraft;
    CrewTeam *crew;
    Passenger **passengers;
    int passengerCapacity;
    int passengerCount;

public:
    Flight(int flightID, const string &origin, const string &destination, const string &aircraft);
    ~Flight();

    Flight(const Flight &) = delete;
    Flight &operator=(const Flight &) = delete;

    void updateStatus(const string &newStatus);
    void assignCrew(CrewTeam *crewTeam);
    void addPassenger(Passenger *passenger);

    void displayFlightInfo() const;
    void displayPassengers() const;

    int getFlightID() const;
    string getOrigin() const;
    string getDestination() const;
    string getStatus() const;
    string getAircraft() const;
    CrewTeam *getCrew() const;
    int getPassengerCount() const;
    Passenger *getPassenger(int index) const;

};

#endif
