#include "crew.h"
#include <iostream>

CrewTeam::CrewTeam(string id, string name)
    : teamID(id), teamName(name), capacity(10), count(0)
{
    crewMembers = new Employee *[capacity];
    for (int i=0; i<capacity; i++)
    {
        crewMembers[i]=nullptr;
    }
}

CrewTeam::~CrewTeam() // destructor
{
    delete[] crewMembers;
}

void CrewTeam::addMember(Employee *employee) // add member to crew
{
    if (count<capacity)
    {
        crewMembers[count] = employee;
        count++;
        cout << "Added " << employee->getName() << " to team " << teamName << endl;
    }
    else
    {
        cout << "Team " << teamName << " is full. Cannot add more members." << endl;
    }
}

void CrewTeam::displayTeam() const
{
    cout << "\nCrew Team: " << teamName << " (" << teamID << ")" << endl;
    cout << "Members: " << count << "/" << capacity << endl;
    
    for (int i=0; i<count; i++)
    {
        if (crewMembers[i] != nullptr)
        {
            cout << i+1 << ". " << crewMembers[i]->getName() << endl;
        }
    }
    cout << endl;
}

void CrewTeam::displayMembers() const
{
    cout << "\n Crew Team: " << teamName << " (" << teamID << ") :" << endl;
    cout << "Total Members: " << count << endl;
    
    if (count == 0)
    {
        cout << "No members in this team." << endl;
        return;
    }
    cout << "\nMember List:" << endl;
    for (int i=0; i<count; i++)
    {
        if (crewMembers[i] != nullptr)
        {
            cout << i+1 << ". " << crewMembers[i]->getName() << endl;
            cout << "   ID: " << crewMembers[i]->getEmployeeID() << endl;
            cout << "   Position: " << crewMembers[i]->getPosition() << endl;
            cout << "   Email: " << crewMembers[i]->getEmail() << endl;
            cout << "   Phone: " << crewMembers[i]->getPhoneNumber() << endl;
        }
    }
}

int CrewTeam::getMemberCount() const // get number of crew members
{
    return count;
}

Employee *CrewTeam::getMember(int index) const // get crew member at index
{
    if (index>=0 && index<count)
    {
        return crewMembers[index];
    }
    return nullptr;
}

string CrewTeam::getTeamID() const // get team ID
{
    return teamID;
}

string CrewTeam::getTeamName() const // get team name
{
    return teamName;
}
