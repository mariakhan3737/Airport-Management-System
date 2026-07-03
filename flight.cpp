#include "Flight.h"
#include <iostream>

Flight::Flight(int flightID, const string &origin, const string &destination, const string &aircraft)
    : flightID(flightID), origin(origin), destination(destination), aircraft(aircraft),
      status("Scheduled"), crew(nullptr), passengerCapacity(200), passengerCount(0)
{

    passengers=new Passenger *[passengerCapacity];
    for (int i=0; i<passengerCapacity; i++)
    {
        passengers[i] = nullptr;
    }

    cout << "Flight " << flightID << " created: " << origin << " -> " << destination << endl;
}

Flight::~Flight() // destructor
{
    for (int i=0; i<passengerCount; i++)
    {
        delete passengers[i];
    }
    delete[] passengers;
}

void Flight::updateStatus(const string &newStatus) // update flight status
{
    status = newStatus;
    cout << "Flight " << flightID << " status updated to: " << newStatus << endl;
}

void Flight::assignCrew(CrewTeam *crewTeam) // assign crew team to flight
{
    crew = crewTeam;
    cout << "Crew team " << crewTeam->getTeamName() << " assigned to Flight " << flightID << endl;
}

void Flight::addPassenger(Passenger *passenger) // add passenger to flight
{
    if (passengerCount<passengerCapacity)
    {
        passengers[passengerCount] = passenger;
        passengerCount++;
        cout << "Passenger " << passenger->getName() << " added to Flight " << flightID << endl;
    }
    else
    {
        cout << "Flight " << flightID << " is full! Cannot add more passengers." << endl;
    }
}

void Flight::displayFlightInfo() const // display flight info on text area
{
    cout << "\nFlights " << flightID << ": " << endl;
    cout << "Route: " << origin << " → " << destination << endl;
    cout << "Aircraft: " << aircraft << endl;
    cout << "Status: " << status << endl;
    cout << "Passengers: " << passengerCount << "/" << passengerCapacity << endl;
    if (crew)
    {
        cout << "Crew: " << crew->getTeamName() << " (" << crew->getMemberCount() << " members)" << endl;
    }
    else
    {
        cout << "Crew: Not assigned" << endl;
    }
}

void Flight::displayPassengers() const // display passengers on flight
{
    cout << "\nPassengers on Flight " << flightID << ": " << endl;
    for (int i=0; i<passengerCount; i++)
    {
        if (passengers[i] != nullptr)
        {
            cout << i + 1 << ". " << passengers[i]->getName() << endl;
        }
    }
}

// return data
int Flight::getFlightID() const 
{ 
    return flightID; 
}
string Flight::getOrigin() const 
{ 
    return origin; 
}
string Flight::getDestination() const 
{
     return destination; 
}
string Flight::getStatus() const 
{ 
    return status; 
}
string Flight::getAircraft() const 
{ 
    return aircraft; 
}
CrewTeam *Flight::getCrew() const 
{ 
    return crew; 
}
int Flight::getPassengerCount() const 
{ return passengerCount; 
}
Passenger *Flight::getPassenger(int index) const // get passenger at index
{
    if (index>=0 && index<passengerCount && passengers[index] != nullptr)
    {
        return passengers[index];
    }
    return nullptr;
}
