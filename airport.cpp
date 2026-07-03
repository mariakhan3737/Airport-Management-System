#include "airport.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Airport::Airport(const string &name, const string &location, int runways)
    : Name(name), Location(location), Runways(runways),
      employeecapacity(50), employeecount(0),
      crewTeamCapacity(20), crewcount(0),
      flightCapacity(30), flightCount(0), // constructor initialising airport
      datafilepath("airport_data.txt")    // setting data file path
{
    Employees=new Employee *[employeecapacity]; // dynamically allocating arrays
    CrewTeams=new CrewTeam *[crewTeamCapacity];
    Flights=new Flight *[flightCapacity];

    for (int i=0; i<employeecapacity; i++)
        Employees[i]=nullptr; // setting everything to null
    for (int i=0; i<crewTeamCapacity; i++)
        CrewTeams[i]=nullptr;
    for (int i=0; i<flightCapacity; i++)
        Flights[i]=nullptr;

    cout << "Airport " << Name << " created with " << Runways << " runways" << endl;
}

Airport::~Airport()
{               // destructor
    autoSave(); // saving data before destroying it so that when we open it again its still there

    for (int i=0; i<employeecount; i++)
        delete Employees[i];
    for (int i=0; i<crewcount; i++)
        delete CrewTeams[i];
    for (int i=0; i<flightCount; i++)
        delete Flights[i]; // deleting things inside array

    delete[] Employees;
    delete[] CrewTeams;
    delete[] Flights; // deleting array to stop memory leaks
}

void Airport::setDataFilePath(const string &path)
{
    datafilepath=path;
}

void Airport::autoSave() const
{
    saveToFile(datafilepath);
}

void Airport::saveToFile(const string &filename) const    //saving data to file
{
    ofstream file(filename);  //open file for writing
    if (!file.is_open())   //if not opened
    {
        cout << "Error: Could not open file for saving: " << filename << endl;
        return;
    }
    
    file << "### AIRPORT DATA FILE ###" << endl;    //file= var for writer object

    file << endl << "[AIRPORT]" << endl;
    file << "Name:" << Name << endl;
    file << "Location:" << Location << endl;
    file << "Runways:" << Runways << endl;

    file << endl << "[EMPLOYEES]" << endl;
    file << "Count:" << employeecount << endl;
    for (int i=0; i<employeecount; i++)
    {
        if (Employees[i] != nullptr) // Saving employee data
        {
            file << "EMPLOYEE_START" << endl;
            file << "Name:" << Employees[i]->getName() << endl;
            file << "ID:" << Employees[i]->getId() << endl;
            file << "Email:" << Employees[i]->getEmail() << endl;
            file << "Phone:" << Employees[i]->getPhoneNumber() << endl;
            file << "EmployeeID:" << Employees[i]->getEmployeeID() << endl;
            file << "Salary:" << Employees[i]->getSalary() << endl;
            file << "Position:" << Employees[i]->getPosition() << endl;
            file << "EMPLOYEE_END" << endl;
        }
    }

    file << endl << "[CREW_TEAMS]" << endl;
    file << "Count:" << crewcount << endl;
    for (int i=0; i<crewcount; i++)
    {
        if (CrewTeams[i] != nullptr) // Saving crew team data
        {
            file << "CREW_START" << endl;
            file << "TeamID:" << CrewTeams[i]->getTeamID() << endl;
            file << "TeamName:" << CrewTeams[i]->getTeamName() << endl;
            file << "MemberCount:" << CrewTeams[i]->getMemberCount() << endl;

            file << "Members:";
            for (int j=0; j<CrewTeams[i]->getMemberCount(); j++)
            {
                Employee *member = CrewTeams[i]->getMember(j);
                if (member != nullptr)
                {
                    if (j>0)
                        file << ",";
                    file << member->getEmployeeID();
                }
            }
            file << endl;
            file << "CREW_END" << endl;
        }
    }

    file << endl << "[FLIGHTS]" << endl;
    file << "Count:" << flightCount << endl;
    for (int i=0; i < flightCount; i++)
    {
        if (Flights[i] != nullptr) // Saving flight data
        {
            file << "FLIGHT_START" << endl;
            file << "FlightID:" << Flights[i]->getFlightID() << endl;
            file << "Origin:" << Flights[i]->getOrigin() << endl;
            file << "Destination:" << Flights[i]->getDestination() << endl;
            file << "Aircraft:" << Flights[i]->getAircraft() << endl;
            file << "Status:" << Flights[i]->getStatus() << endl;

            if (Flights[i]->getCrew() != nullptr)
                file << "CrewTeamID:" << Flights[i]->getCrew()->getTeamID() << endl;
            else
                file << "CrewTeamID:NONE" << endl;

            file << "PassengerCount:" << Flights[i]->getPassengerCount() << endl;
            for (int j=0; j<Flights[i]->getPassengerCount(); j++)
            {
                Passenger *p = Flights[i]->getPassenger(j);
                if (p != nullptr)
                {
                    file << "PASSENGER_START" << endl;
                    file << "Name:" << p->getName() << endl;
                    file << "ID:" << p->getId() << endl;
                    file << "Email:" << p->getEmail() << endl;
                    file << "Phone:" << p->getPhoneNumber() << endl;
                    file << "Passport:" << p->getPassportNumber() << endl;
                    file << "Nationality:" << p->getNationality() << endl;
                    file << "PASSENGER_END" << endl;
                }
            }
            file << "FLIGHT_END" << endl;
        }
    }

    file << endl
         << "END OF DATA" << endl;
    file.close();
    cout << "Data saved to " << filename << endl;
}

void Airport::loadFromFile(const string &filename)    //loading data from file
{
    ifstream file(filename);  //open file for reading filename
    if (!file.is_open())   //if it doesnt open
    {
        cout << "No existing data file found. Starting fresh." << endl;
        return;
    }

    string line;  //to store the lines read
    string section = "";   //to see if were in airport, employee etc

    string empName, empId, empEmail, empPhone, empEmployeeID, empPosition;
    double empSalary=0;

    string crewTeamID, crewTeamName, crewMembers;

    int flightID=0;
    string flightOrigin, flightDest, flightAircraft, flightStatus, flightCrewID;  //temp var(only exist when reading file)
    string passName, passId, passEmail, passPhone, passPassport, passNationality;

    while (getline(file, line))  //reading every line into line
    {
        if (line.empty() || line[0]=='#')  //skip the empty lines
            continue;

        if (line=="[AIRPORT]")   //to keep track of where we are
        {
            section = "AIRPORT";
            continue;
        }
        else if (line=="[EMPLOYEES]")
        {
            section="EMPLOYEES";
            continue;
        }
        else if (line=="[CREW_TEAMS]")
        {
            section="CREW_TEAMS";
            continue;
        }
        else if (line=="[FLIGHTS]")
        {
            section="FLIGHTS";
            continue;
        }

        int colon = line.find(':');  
        if (colon==-1)   //if theres no colon
        {
            if (line=="EMPLOYEE_START")   //then its not a key value pair
            {
                empName = empId = empEmail = empPhone = empEmployeeID = empPosition = "";   //reset employee var
                empSalary = 0;
            }
            else if (line=="EMPLOYEE_END")
            {
                if (!empName.empty() && !empEmployeeID.empty())   //if theyre not empty
                {
                    Employee *emp = new Employee(empName, empId, empEmail, empPhone, empEmployeeID, empSalary, empPosition);  //make new objects
                    if (employeecount<employeecapacity)
                    {
                        Employees[employeecount]=emp;
                        employeecount++;
                    }
                }
            }
            else if (line=="CREW_START")
            {
                crewTeamID=crewTeamName = crewMembers = "";
            }
            else if (line=="CREW_END")
            {
                if (!crewTeamID.empty() && !crewTeamName.empty())
                {
                    if (crewcount < crewTeamCapacity)
                    {
                        CrewTeams[crewcount] = new CrewTeam(crewTeamID, crewTeamName);

                        if (!crewMembers.empty())
                        {
                            stringstream ss(crewMembers);
                            string memberID;
                            while (getline(ss, memberID, ','))
                            {
                                for (int i=0; i<employeecount; i++)
                                {
                                    if (Employees[i]->getEmployeeID()==memberID)  //finds id
                                    {
                                        CrewTeams[crewcount]->addMember(Employees[i]);  //adds id to crew
                                        break;
                                    }
                                }
                            }
                        }
                        crewcount++;
                    }
                }
            }
            else if (line=="FLIGHT_START")
            {
                flightID=0;
                flightOrigin = flightDest = flightAircraft = flightStatus = flightCrewID = "";
            }
            else if (line=="FLIGHT_END")
            {
                if (flightID > 0 && !flightOrigin.empty())
                {
                    if (flightCount < flightCapacity)
                    {
                        Flights[flightCount]=new Flight(flightID, flightOrigin, flightDest, flightAircraft);
                        if (flightStatus != "Scheduled")
                            Flights[flightCount]->updateStatus(flightStatus);

                        if (flightCrewID != "NONE" && !flightCrewID.empty())
                        {
                            CrewTeam *crew = findCrewTeam(flightCrewID);
                            if (crew != nullptr)
                                Flights[flightCount]->assignCrew(crew);
                        }
                        flightCount++;
                    }
                }
            }
            else if (line=="PASSENGER_START")
            {
                passName = passId = passEmail = passPhone = passPassport = passNationality = "";
            }
            else if (line=="PASSENGER_END")
            {
                if (!passName.empty() && !passPassport.empty() && flightCount > 0)
                {
                    Passenger *p = new Passenger(passName, passId, passEmail, passPhone, passPassport, passNationality);
                    Flights[flightCount-1]->addPassenger(p);   //adds passenger to recdent flight
                }
            }
            continue;
        }

        string key = line.substr(0, colon);    //split before colon
        string value = line.substr(colon+1);   //split after

        if (section=="EMPLOYEES")
        {
            if (key=="Name")
                empName = value;
            else if (key=="ID")
                empId = value;
            else if (key=="Email")
                empEmail = value;
            else if (key=="Phone")
                empPhone = value;
            else if (key=="EmployeeID")
                empEmployeeID = value;
            else if (key=="Salary")
                empSalary = stod(value);
            else if (key=="Position")
                empPosition = value;
        }
        else if (section=="CREW_TEAMS")
        {
            if (key=="TeamID")
                crewTeamID = value;
            else if (key=="TeamName")
                crewTeamName = value;
            else if (key=="Members")
                crewMembers = value;
        }
        else if (section=="FLIGHTS")
        {
            if (key=="FlightID")
                flightID = stoi(value);
            else if (key=="Origin")
                flightOrigin = value;
            else if (key=="Destination")
                flightDest = value;
            else if (key=="Aircraft")
                flightAircraft = value;
            else if (key=="Status")
                flightStatus = value;
            else if (key=="CrewTeamID")
                flightCrewID = value;
            else if (key=="Name")
                passName = value;
            else if (key=="ID")
                passId = value;
            else if (key=="Email")
                passEmail = value;
            else if (key=="Phone")
                passPhone = value;
            else if (key=="Passport")
                passPassport = value;
            else if (key=="Nationality")
                passNationality = value;
        }
    }

    file.close();
    cout << "Data loaded from " << filename << endl;
    cout << "  - " << employeecount << " employees" << endl;
    cout << "  - " << crewcount << " crew teams" << endl;
    cout << "  - " << flightCount << " flights" << endl;
}

void Airport::AddFlight(int flightID, const string &origin, const string &destination, const string &aircraft) // add flight to airport
{
    if (flightCount<flightCapacity)
    {
        Flights[flightCount]=new Flight(flightID, origin, destination, aircraft);
        flightCount++;
        cout << "Flight " << flightID << " added to " << Name << " airport" << endl;
        autoSave();
    }
    else
    {
        cout << "Can't add more flights airport capacity reached" << endl;
    }
}

void Airport::updateFlightStatus(int flightID, const string &newStatus) // update flight status
{
    for (int i=0; i<flightCount; i++)
    {
        if (Flights[i]->getFlightID()==flightID)
        {
            Flights[i]->updateStatus(newStatus);
            cout << "Flight " << flightID << " status updated to: " << newStatus << endl;
            autoSave();
            return;
        }
    }
    cout << "Flight " << flightID << " not found" << endl;
}

void Airport::AssignCrewToFlight(int flightID, CrewTeam *crew) // assign crew team to flight
{
    for (int i=0; i<flightCount; i++)
    {
        if (Flights[i]->getFlightID()==flightID)
        {
            Flights[i]->assignCrew(crew);
            autoSave();
            return;
        }
    }
    cout << "Flight " << flightID << " not found" << endl;
}

void Airport::addPassengerToFlight(int flightID, Passenger *passenger) // add passenger to flight
{
    for (int i=0; i<flightCount; i++)
    {
        if (Flights[i]->getFlightID()==flightID)
        {
            Flights[i]->addPassenger(passenger);
            autoSave();
            return;
        }
    }
    cout << "Flight " << flightID << " not found" << endl;
}

void Airport::assignEmployee(Employee *employee, const string &teamID) // assign employee to airport and to crew team
{
    if (employeecount < employeecapacity)
    {
        Employees[employeecount] = employee;
        employeecount++;
        cout << "Employee " << employee->getName() << " assigned to " << Name << endl;

        if (!teamID.empty())
        {
            for (int i=0; i<crewcount; i++)
            {
                if (CrewTeams[i]->getTeamID()==teamID)
                {
                    CrewTeams[i]->addMember(employee);
                    break;
                }
            }
        }
        autoSave();
    }
    else
    {
        cout << "Airport " << Name << " is full! Cannot hire more employees." << endl;
    }
}

void Airport::displayAllEmployees() const // display all employees at airport
{
    cout << "\nEMPLOYEES AT: " << Name << " " << endl;
    for (int i=0; i<employeecount; i++)
    {
        if (Employees[i] != nullptr)
        {
            cout << i+1 << ". " << Employees[i]->getName() << " - " << Employees[i]->getPosition() << endl;
        }
    }
}

void Airport::createCrewTeam(const string &teamID, const string &teamName) // create crew team at airport
{
    if (crewcount<crewTeamCapacity)
    {
        CrewTeams[crewcount]=new CrewTeam(teamID, teamName);
        crewcount++;
        cout << "Crew team " << teamName << " created at " << Name << " airport" << endl;
        autoSave();
    }
    else
    {
        cout << "cannot create more crew teams capacity reached" << endl;
    }
}

CrewTeam *Airport::findCrewTeam(const string &teamID) const // find crew team by ID
{
    for (int i=0; i<crewcount; i++)
    {
        if (CrewTeams[i]->getTeamID()==teamID)
        {
            return CrewTeams[i];
        }
    }
    return nullptr;
}

void Airport::displayAllCrewTeams() const // display all crew teams at airport
{
    cout << "\nCREW TEAMS AT: " << Name << " " << endl;
    for (int i=0; i<crewcount; i++)
    {
        if (CrewTeams[i] != nullptr)
        {
            CrewTeams[i]->displayTeam();
            CrewTeams[i]->displayMembers(); 
        }
    }
}

void Airport::displaySummary() const // display airport summary on text area
{
    cout << "\nAirport summary: " << endl;
    cout << "Name: " << Name << endl;
    cout << "Location: " << Location << endl;
    cout << "Runways: " << Runways << endl;
    cout << "Employees: " << employeecount << endl;
    cout << "Crew Teams: " << crewcount << endl;
    cout << "Flights: " << flightCount << endl;
}

string Airport::getName() const     //getters
{ 
    return Name; 
}
string Airport::getLocation() const 
{ 
    return Location; 
}
int Airport::getRunways() const 
{ 
    return Runways; 
}
int Airport::getFlightCount() const 
{ 
    return flightCount; 
}

Flight *Airport::getFlight(int index) const
{
    if (index >= 0 && index < flightCount)
        return Flights[index];
    return nullptr;
}

Employee *Airport::getEmployee(int index) const // get employee by index
{
    if (index >= 0 && index < employeecount)
        return Employees[index];
    return nullptr;
}

int Airport::getemployeecount() const { 
    return employeecount;    //// get employee count
 } 

CrewTeam *Airport::getCrewTeam(int index) const // get crew team by index
{
    if (index >= 0 && index < crewcount)
        return CrewTeams[index];
    return nullptr;
}

int Airport::getCrewCount() const { return crewcount; } // get crew team count
