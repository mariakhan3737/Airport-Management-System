#include "Person.h"
#include <iostream>
using namespace std;

Person::Person(const string &name, const string &id, const string &email, const string &phoneNumber)
    : name(name), id(id), email(email), phoneNumber(phoneNumber) {}

string Person::getName() const
{
    return name;
}
string Person::getId() const
{
    return id;
}
string Person::getEmail() const
{
    return email;
}
string Person::getPhoneNumber() const
{
    return phoneNumber;
}

// constructor for employee
Employee::Employee(const string &name, const string &id, const string &email, const string &phoneNumber, const string &employeeID, double salary, const string &position)
    : Person(name, id, email, phoneNumber), employeeID(employeeID), salary(salary), position(position) {}

void Employee::displayInfo() const // display employee info on text area
{
    cout << "\nEmployee Info: " << endl;
    cout << "Name: " << getName() << endl;
    cout << "ID: " << getId() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Phone: " << getPhoneNumber() << endl;
    cout << "Employee ID: " << employeeID << endl;
    cout << "Position: " << position << endl;
    cout << "Salary: " << salary << endl;
}

string Employee::bookFlight() const // book a flight for a passenger
{
    return "Employee " + employeeID + " booked a flight for a passenger";
}

string Employee::cancelBooking() const // cancel a booking
{
    return "Employee " + employeeID + " canceled a booking";
}

string Employee::checkIn() const // check in a passenger
{
    return "Employee " + employeeID + " checked in a passenger";
}

string Employee::getEmployeeID() const // get employee ID
{
    return employeeID;
}
double Employee::getSalary() const // get employee salary
{
    return salary;
}
string Employee::getPosition() const // get employee position
{
    return position;
}
// constructor for passenger
Passenger::Passenger(const string &name, const string &id, const string &email, const string &phoneNumber, const string &passportNumber, const string &nationality)
    : Person(name, id, email, phoneNumber), passportNumber(passportNumber), nationality(nationality) {}

void Passenger::displayInfo() const // display passenger info on text area
{
    cout << "\nPasseneger info: " << endl;
    cout << "Name: " << getName() << endl;
    cout << "ID: " << getId() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Phone: " << getPhoneNumber() << endl;
    cout << "Passport: " << passportNumber << endl;
    cout << "Nationality: " << nationality << endl;
}

string Passenger::getPassportNumber() const
{
    return passportNumber;
}
string Passenger::getNationality() const
{
    return nationality;
}
