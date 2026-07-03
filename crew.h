#ifndef CREWTEAM_H
#define CREWTEAM_H

#include <string>
#include "person.h"
using namespace std;

class CrewTeam
{
private:
    string teamID;
    string teamName;
    Employee **crewMembers;
    int capacity;
    int count;

public:
    CrewTeam(string id, string name);
    ~CrewTeam();

    CrewTeam(const CrewTeam &) = delete;
    CrewTeam &operator=(const CrewTeam &) = delete;

    void addMember(Employee *employee);
    void removeMember(Employee *employee);
    void displayTeam() const;
    void displayMembers() const;
    int getMemberCount() const;
    Employee* getMember(int index) const;  

    string getTeamID() const;
    string getTeamName() const;
};

#endif
