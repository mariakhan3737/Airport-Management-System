#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "airport.h"
using namespace std;

class inputbox
{
    sf::RectangleShape box;
    sf::Text text;
    sf::Text label;
    string content;
    bool active;
    bool numtrue;

public:
    inputbox(sf::Font &font, const string &labelText, sf::Vector2f pos, bool onlynum=false)
    {
        numtrue = onlynum;
        active = false;
        content = "";

        label.setFont(font);
        label.setString(labelText);
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::White);
        label.setPosition(pos.x, pos.y - 25);

        box.setSize({300, 35});
        box.setPosition(pos);
        box.setFillColor(sf::Color(50, 50, 50));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(1);

        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setPosition(pos.x + 5, pos.y + 5);
    }

    void handleEvent(sf::Event &event, sf::RenderWindow &window)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mousePos(sf::Mouse::getPosition(window));
            active = box.getGlobalBounds().contains(mousePos);
            box.setOutlineColor(active ? sf::Color::Cyan : sf::Color::White);
        }

        if (active && event.type == sf::Event::TextEntered)  //if inputbox clicked
        {
            if (event.text.unicode == 8)  //handling backsapce
            {
                if (!content.empty())
                    content.pop_back();
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 127) //all characters
            {
                char c = static_cast<char>(event.text.unicode);  //convert to char
                if (numtrue)
                {
                    if (isdigit(c) || c == '.')
                        content += c;
                }
                else
                {
                    content += c;
                }
            }
            text.setString(content);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(label);
        window.draw(box);
        window.draw(text);
    }

    string getText() const { return content; }

    int getInt() const
    {
        try
        {
            return content.empty() ? 0 : stoi(content);  //if cant convert string to int then return 0
        }
        catch (...)
        {
            return 0;
        }
    }

    double getDouble() const
    {
        try
        {
            return content.empty() ? 0.0 : stod(content);  //if cant convert string to double then return 0
        }
        catch (...)  //
        {
            return 0.0;
        }
    }

    void clear()
    {
        content = "";
        text.setString("");
    }
};

//  Buttons 
class Button
{
public:
    sf::RectangleShape shape;
    sf::Text text;
    bool ispress;
    bool enabled;

    Button(sf::Font &font, string label, sf::Vector2f pos, sf::Vector2f size = {240, 50})
    {
        ispress = false;
        enabled = true;
        shape.setSize(size);
        shape.setPosition(pos);
        shape.setFillColor(sf::Color(60, 60, 60));

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            pos.x + (size.x - textBounds.width) / 2 - textBounds.left,
            pos.y + (size.y - textBounds.height) / 2 - textBounds.top);
    }

    bool isClicked(sf::RenderWindow &window)
    {
        if (!enabled)
            return false;

        bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        sf::Vector2f m(sf::Mouse::getPosition(window));
        bool hover = shape.getGlobalBounds().contains(m);

        if (hover)
            shape.setFillColor(sf::Color(80, 80, 80));
        else
            shape.setFillColor(sf::Color(60, 60, 60));

        if (isPressed && !ispress && hover)
        {
            ispress = true;
            return true;
        }

        if (!isPressed)
            ispress = false;

        return false;
    }

    void setPosition(sf::Vector2f pos)
    {
        shape.setPosition(pos);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            pos.x + (shape.getSize().x - textBounds.width) / 2 - textBounds.left,  //to center
            pos.y + (shape.getSize().y - textBounds.height) / 2 - textBounds.top);
    }

    void draw(sf::RenderWindow &w)
    {
        w.draw(shape);
        w.draw(text);
    }
};

// Text Area
class LargeTextArea
{
private:
    sf::RectangleShape box;
    sf::Text text;
    string content;

public:
    LargeTextArea(sf::Font &font, sf::Vector2f pos, sf::Vector2f size, unsigned int charsize = 16)
    {
        box.setPosition(pos);
        box.setSize(size);
        box.setFillColor(sf::Color(20, 20, 20));
        box.setOutlineColor(sf::Color(100, 100, 100));
        box.setOutlineThickness(2);

        text.setFont(font);
        text.setCharacterSize(charsize);
        text.setFillColor(sf::Color::White);
        text.setPosition(pos.x + 10, pos.y + 10);
    }

    void setText(const string &t)
    {
        content = t;
        text.setString(content);
    }

    void clear()
    {
        content = "";
        text.setString("");
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(box);
        window.draw(text);
    }
};

// Message Box 
class MessageBox
{
private:
    sf::RectangleShape background;
    sf::Text messageText;
    float displayTime;
    float currentTime;
    bool visible;

public:
    MessageBox(sf::Font &font, sf::Vector2f pos)
    {
        visible = false;
        displayTime = 3.0f;
        currentTime = 0;

        background.setSize({400, 50});
        background.setPosition(pos);
        background.setFillColor(sf::Color(0, 100, 0, 200));

        messageText.setFont(font);
        messageText.setCharacterSize(18);
        messageText.setFillColor(sf::Color::White);
        messageText.setPosition(pos.x + 10, pos.y + 12);
    }

    void show(const string &message, bool isError = false)
    {
        messageText.setString(message);
        background.setFillColor(isError ? sf::Color(150, 0, 0, 200) : sf::Color(0, 100, 0, 200));
        visible = true;
        currentTime = 0;
    }

    void update(float timeelapse)  //to hide notifs after some time
    {
        if (visible)
        {
            currentTime += timeelapse;
            if (currentTime >= displayTime)
                visible = false;
        }
    }

    void draw(sf::RenderWindow &window)
    {
        if (visible)
        {
            window.draw(background);
            window.draw(messageText);
        }
    }
};

// Screen Enum 
enum class Screen  //19 screens
{
    MAIN,
    FLIGHT_MANAGEMENT,
    ADD_FLIGHT,
    EDIT_FLIGHT,
    VIEW_FLIGHTS,
    ASSIGN_CREW_TO_FLIGHT,
    PASSENGER_SERVICES,
    ADD_PASSENGER,
    VIEW_PASSENGERS,
    EMPLOYEE_MANAGEMENT,
    ADD_EMPLOYEE,
    VIEW_EMPLOYEES,
    VIEW_FLIGHT_CREW,
    CREW_MANAGEMENT,
    CREATE_CREW,
    ADD_EMPLOYEE_TO_CREW,
    ASSIGN_CREW,
    VIEW_CREWS,
    VIEW_CREW_MEMBERS
};

// Helper Functions 

string getFlightListText(Airport &airport)
{
    stringstream ss;
    ss << "=== ALL FLIGHTS ===\n\n";

    if (airport.getFlightCount() == 0)
    {
        ss << "No flights available.\n";
    }
    else
    {
        for (int i = 0; i < airport.getFlightCount(); i++)
        {
            Flight *f = airport.getFlight(i);
            if (f)
            {
                ss << "Flight #" << f->getFlightID() << "\n";
                ss << "  Route: " << f->getOrigin() << " -> " << f->getDestination() << "\n";
                ss << "  Aircraft: " << f->getAircraft() << "\n";
                ss << "  Status: " << f->getStatus() << "\n";
                ss << "  Passengers: " << f->getPassengerCount() << "\n";
                if (f->getCrew())
                    ss << "  Crew: " << f->getCrew()->getTeamName() << "\n";
                else
                    ss << "  Crew: Not assigned\n";
                ss << "\n";
            }
        }
    }
    return ss.str();
}

string getEmployeeListText(Airport &airport)
{
    stringstream ss;
    ss << "=== ALL EMPLOYEES ===\n\n";

    if (airport.getemployeecount() == 0)
    {
        ss << "No employees registered.\n";
    }
    else
    {
        for (int i = 0; i < airport.getemployeecount(); i++)
        {
            Employee *e = airport.getEmployee(i);
            if (e)
            {
                ss << i + 1 << ". " << e->getName() << "\n";
                ss << "   Position: " << e->getPosition() << "\n";
                ss << "   ID: " << e->getEmployeeID() << "\n";
                ss << "   Email: " << e->getEmail() << "\n\n";
            }
        }
    }
    return ss.str();
}
//  Get All Passengers Text 

string getCrewListText(Airport &airport)
{
    stringstream ss;
    ss << "=== ALL CREW TEAMS ===\n\n";

    if (airport.getCrewCount() == 0)
    {
        ss << "No crew teams created.\n";
    }
    else
    {
        for (int i = 0; i < airport.getCrewCount(); i++)
        {
            CrewTeam *c = airport.getCrewTeam(i);
            if (c)
            {
                ss << "Team: " << c->getTeamName() << " (" << c->getTeamID() << ")\n";
                ss << "  Members: " << c->getMemberCount() << "\n\n";
            }
        }
    }
    return ss.str();
}

string formatCrewMembersText(Airport &airport)
{
    stringstream ss;
    
    if (airport.getCrewCount() == 0)
    {
        ss << "No crew teams created.\n\n";
        ss << "Use 'Create Crew Team' to create a crew team first,\n";
        ss << "then add employees using 'Add Employee to Crew'.";
        return ss.str();
    }
    
    for (int i=0; i<airport.getCrewCount(); i++)
    {
        CrewTeam *crew = airport.getCrewTeam(i);
        if (crew)
        {
            ss << "TEAM: " << crew->getTeamName() << " (ID: " << crew->getTeamID() << ")\n";
            ss << "Total Members: " << crew->getMemberCount() << "\n\n";
            
            if (crew->getMemberCount() == 0)
            {
                ss << "  No members in this team.\n";
                ss << "  Use 'Add Employee to Crew' to add members.\n\n";
            }
            else
            {
                for (int j=0; j<crew->getMemberCount(); j++)
                {
                    Employee* member = crew->getMember(j);
                    if (member)
                    {
                        ss << "  Member " << j+1 << ": " << member->getName() << "\n";
                        ss << "    Employee ID: " << member->getEmployeeID() << "\n";
                        ss << "    Position: " << member->getPosition() << "\n";
                        ss << "    Email: " << member->getEmail() << "\n";
                        ss << "    Phone: " << member->getPhoneNumber() << "\n";
                        ss << "    Personal ID: " << member->getId() << "\n\n";
                    }
                }
            }
        }
    }
    return ss.str();
}



string getCrewMembersText(Airport &airport)
{
    stringstream ss;
    ss << "MEMBERS: \n\n";
    
    if (airport.getCrewCount() == 0)
    {
        ss << "No crew teams created.\n";
    }
    else
    {
        for (int i = 0; i < airport.getCrewCount(); i++)
        {
            CrewTeam *crew = airport.getCrewTeam(i);
            if (crew)
            {
                ss << "TEAM: " << crew->getTeamName() << " (ID: " << crew->getTeamID() << ")\n";
                ss << "Total Members: " << crew->getMemberCount() << "\n";
                
                for (int j=0; j<crew->getMemberCount(); j++)
                {
                    Employee* member = crew->getMember(j);
                    if (member)
                    {
                        ss << j+1 << ". " << member->getName() << "\n";
                        ss << "   Employee ID: " << member->getEmployeeID() << "\n";
                        ss << "   Position: " << member->getPosition() << "\n";
                        ss << "   Email: " << member->getEmail() << "\n";
                        ss << "   Phone: " << member->getPhoneNumber() << "\n";
                        ss << "   Personal ID: " << member->getId() << "\n\n";
                    }
                }
            }
        }
    }
    return ss.str();
}

string getFlightCrewText(Flight *flight)
{
    stringstream ss;

    if (flight == nullptr)
    {
        ss << "Flight not found!\n";
        return ss.str();
    }

    ss << "=== CREW FOR FLIGHT " << flight->getFlightID() << " ===\n\n";
    ss << "Route: " << flight->getOrigin() << " -> " << flight->getDestination() << "\n";
    ss << "Aircraft: " << flight->getAircraft() << "\n";
    ss << "Status: " << flight->getStatus() << "\n\n";

    CrewTeam *crew = flight->getCrew();

    if (crew == nullptr)
    {
        ss << "No crew assigned to this flight yet.\n";
        ss << "Use 'Assign Crew to Flight' to assign a crew team.\n";
    }
    else
    {
        ss << "Assigned Crew Team: " << crew->getTeamName() << "\n";
        ss << "Team ID: " << crew->getTeamID() << "\n";
        ss << "Total Members: " << crew->getMemberCount() << "\n\n";
        ss << "--- Crew Members ---\n\n";
        ss << "Crew team assigned. Use 'View All Crews' to see team details.\n";
    }

    return ss.str();
}

string getPassengerListText(Flight *flight)
{
    stringstream ss;

    if (flight == nullptr)
    {
        ss << "Flight not found!\n";
        return ss.str();
    }

    ss << "=== PASSENGERS ON FLIGHT " << flight->getFlightID() << " ===\n\n";
    ss << "Route: " << flight->getOrigin() << " -> " << flight->getDestination() << "\n";
    ss << "Aircraft: " << flight->getAircraft() << "\n";
    ss << "Status: " << flight->getStatus() << "\n\n";

    int count = flight->getPassengerCount();
    ss << "Total Passengers: " << count << "\n\n";

    if (count == 0)
    {
        ss << "No passengers on this flight yet.\n";
    }
    else
    {
        ss << "--- Passenger List ---\n\n";
        for (int i = 0; i < count; i++)
        {
            Passenger *p = flight->getPassenger(i);
            if (p)
            {
                ss << i + 1 << ". " << p->getName() << "\n";
                ss << "   Passport: " << p->getPassportNumber() << "\n";
                ss << "   Nationality: " << p->getNationality() << "\n";
                ss << "   Email: " << p->getEmail() << "\n";
                ss << "   Phone: " << p->getPhoneNumber() << "\n";
            }
            else
            {
                ss << i + 1 << ". [NULL PASSENGER]\n\n";
            }
        }
    }

    return ss.str();
}

//  Main Function 
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 900), "Airport Management System");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") &&
        !font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf") &&
        !font.loadFromFile("/System/Library/Fonts/Arial.ttf"))
    {
        cout << "Error loading font!" << endl;
        return -1;
    }

    //  Create Airport 
    Airport airport("Karachi International", "Karachi, Pakistan", 4);

    // Load existing data from file
    airport.loadFromFile("airport_data.txt");

    Screen currentScreen = Screen::MAIN;
    MessageBox msgBox(font, {440, 820});
    sf::Clock clock;

    //  Main Menu Buttons 
    Button btnFlightMgmt(font, "Flight Management", {490, 300}, {300, 60});
    Button btnPassengerSvc(font, "Passenger Services", {490, 380}, {300, 60});
    Button btnEmployeeMgmt(font, "Employee Management", {490, 460}, {300, 60});
    Button btnCrewMgmt(font, "Crew Management", {490, 540}, {300, 60});
    Button btnViewSummary(font, "View Airport Summary", {490, 620}, {300, 60});
    Button btnExit(font, "Exit", {490, 700}, {300, 60});

    Button btnBack(font, "Back", {50, 820}, {150, 50});

    // Flight Management Buttons 
    Button btnAddFlight(font, "Add Flight", {150, 300}, {200, 50});
    Button btnViewFlights(font, "View All Flights", {400, 300}, {200, 50});
    Button btnEditFlight(font, "Update Status", {650, 300}, {200, 50});
    Button btnAssignCrewToFlight(font, "Assign Crew", {900, 300}, {200, 50}); // NEW

    //  Passenger Buttons 
    Button btnAddPassenger(font, "Add Passenger to Flight", {300, 300}, {300, 50});
    Button btnViewPassengers(font, "View Flight Passengers", {650, 300}, {300, 50});

    // Employee Button
    Button btnAddEmployee(font, "Add Employee", {250, 300}, {250, 50});
    Button btnViewEmployees(font, "View All Employees", {550, 300}, {250, 50});
    Button btnViewFlightCrew(font, "View Flight Crew", {400, 380}, {250, 50});

    //  Crew Buttons 
    Button btnCreateCrew(font, "Create Crew Team", {150, 300}, {220, 50});
    Button btnAddEmployeeToCrew(font, "Add Employee to Crew", {420, 300}, {250, 50});
    Button btnViewCrews(font, "View All Crews", {720, 300}, {220, 50}); 
    Button btnViewCrewMembers(font, "View Crew Members", {720, 380}, {220, 50}); 

    // Text Display Area
    LargeTextArea displayArea(font, {100, 380}, {1080, 400});

    // Input Boxes to Add Flight
    inputbox inputFlightID(font, "Flight ID:", {200, 320}, true);
    inputbox inputOrigin(font, "Origin:", {200, 400});
    inputbox inputDestination(font, "Destination:", {200, 480});
    inputbox inputAircraft(font, "Aircraft Type:", {200, 560});
    Button btnSubmitFlight(font, "Add Flight", {200, 640}, {200, 50});

    // Input Boxes to Edit Flight
    inputbox inputEditFlightID(font, "Flight ID:", {200, 320}, true);
    inputbox inputNewStatus(font, "New Status (Scheduled/Boarding/Departed/Cancelled):", {200, 400});
    Button btnSubmitStatus(font, "Update Status", {200, 480}, {200, 50});

    // Input Boxes to Add Passenger
    inputbox inputPassFlightID(font, "Flight ID:", {200, 280}, true);
    inputbox inputPassName(font, "Passenger Name:", {200, 360});
    inputbox inputPassEmail(font, "Email:", {550, 280});
    inputbox inputPassPhone(font, "Phone:", {550, 440});
    inputbox inputPassport(font, "Passport Number:", {200, 440});
    inputbox inputNationality(font, "Nationality:", {550, 360});
    Button btnSubmitPassenger(font, "Add Passenger", {400, 540}, {200, 50});

    // Input Boxes to Add Employee
    inputbox inputEmpName(font, "Name:", {200, 280});
    inputbox inputEmpID(font, "ID:", {200, 360});
    inputbox inputEmpEmail(font, "Email:", {200, 440});
    inputbox inputEmpPhone(font, "Phone:", {550, 280});
    inputbox inputempnumber(font, "Employee Number:", {550, 360});
    inputbox inputEmpSalary(font, "Salary:", {550, 440}, true);
    inputbox inputEmpPosition(font, "Position:", {200, 520});
    Button btnSubmitEmployee(font, "Add Employee", {400, 620}, {200, 50});

    // Input Boxes to Create Crew
    inputbox inputCrewID(font, "Crew Team ID:", {200, 320});
    inputbox inputCrewName(font, "Crew Team Name:", {200, 400});
    Button btnSubmitCrew(font, "Create Team", {200, 480}, {200, 50});

    inputbox inputAddEmpCrewID(font, "Crew Team ID:", {200, 320});
    inputbox inputAddEmpID(font, "Employee Number:", {200, 400});
    Button btnSubmitAddEmpToCrew(font, "Add to Crew", {200, 480}, {200, 50});

    // Input Boxes to Assign Crew
    inputbox inputAssignFlightID(font, "Flight ID:", {200, 320}, true);
    inputbox inputAssignCrewID(font, "Crew Team ID:", {200, 400});
    Button btnSubmitAssign(font, "Assign Crew", {200, 480}, {200, 50});

    // Input Boxes to View Passengers
    inputbox inputViewPassFlightID(font, "Enter Flight ID to view passengers:", {200, 320}, true);
    Button btnViewPassSubmit(font, "View Passengers", {520, 330}, {200, 50});

    inputbox inputViewCrewFlightID(font, "Enter Flight ID to view crew:", {200, 320}, true);
    Button btnViewCrewSubmit(font, "View Crew", {520, 330}, {200, 50});

    //  MAIN LOOP 
    while (window.isOpen())
    {
        float timeelapse = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // code to handle input events
            if (currentScreen == Screen::ADD_FLIGHT)
            {
                inputFlightID.handleEvent(event, window);
                inputOrigin.handleEvent(event, window);
                inputDestination.handleEvent(event, window);
                inputAircraft.handleEvent(event, window);
            }
            else if (currentScreen == Screen::EDIT_FLIGHT)
            {
                inputEditFlightID.handleEvent(event, window);
                inputNewStatus.handleEvent(event, window);
            }
            else if (currentScreen == Screen::ADD_PASSENGER)
            {
                inputPassFlightID.handleEvent(event, window);
                inputPassName.handleEvent(event, window);
                inputPassEmail.handleEvent(event, window);
                inputPassPhone.handleEvent(event, window);
                inputPassport.handleEvent(event, window);
                inputNationality.handleEvent(event, window);
            }
            else if (currentScreen == Screen::ADD_EMPLOYEE)
            {
                inputEmpName.handleEvent(event, window);
                inputEmpID.handleEvent(event, window);
                inputEmpEmail.handleEvent(event, window);
                inputEmpPhone.handleEvent(event, window);
                inputempnumber.handleEvent(event, window);
                inputEmpSalary.handleEvent(event, window);
                inputEmpPosition.handleEvent(event, window);
            }
            else if (currentScreen == Screen::CREATE_CREW)
            {
                inputCrewID.handleEvent(event, window);
                inputCrewName.handleEvent(event, window);
            }
            else if (currentScreen == Screen::ASSIGN_CREW)
            {
                inputAssignFlightID.handleEvent(event, window);
                inputAssignCrewID.handleEvent(event, window);
            }
            else if (currentScreen == Screen::ASSIGN_CREW_TO_FLIGHT)
            {
                inputAssignFlightID.handleEvent(event, window);
                inputAssignCrewID.handleEvent(event, window);
            }
            else if (currentScreen == Screen::ADD_EMPLOYEE_TO_CREW)
            {
                inputAddEmpCrewID.handleEvent(event, window);
                inputAddEmpID.handleEvent(event, window);
            }
            else if (currentScreen == Screen::VIEW_PASSENGERS)
            {
                inputViewPassFlightID.handleEvent(event, window);
            }
            else if (currentScreen == Screen::VIEW_FLIGHT_CREW)
            {
                inputViewCrewFlightID.handleEvent(event, window);
            }
        }

        msgBox.update(timeelapse);
        window.clear(sf::Color(30, 30, 35));

        //  MAIN MENU 
        if (currentScreen == Screen::MAIN)
        {
            sf::Text title("Airport Management System", font, 48);
            title.setPosition(380, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            sf::Text subtitle("Karachi International Airport", font, 24);
            subtitle.setPosition(470, 170);
            subtitle.setFillColor(sf::Color(150, 150, 150));
            window.draw(subtitle);

            btnFlightMgmt.draw(window);
            btnPassengerSvc.draw(window);
            btnEmployeeMgmt.draw(window);
            btnCrewMgmt.draw(window);
            btnViewSummary.draw(window);
            btnExit.draw(window);

            if (btnFlightMgmt.isClicked(window))
                currentScreen = Screen::FLIGHT_MANAGEMENT;
            else if (btnPassengerSvc.isClicked(window))
                currentScreen = Screen::PASSENGER_SERVICES;
            else if (btnEmployeeMgmt.isClicked(window))
                currentScreen = Screen::EMPLOYEE_MANAGEMENT;
            else if (btnCrewMgmt.isClicked(window))
                currentScreen = Screen::CREW_MANAGEMENT;
            else if (btnViewSummary.isClicked(window))
            {
                stringstream ss;
                ss << " AIRPORT SUMMARY \n\n";
                ss << "Name: " << airport.getName() << "\n";
                ss << "Total Flights: " << airport.getFlightCount() << "\n";
                ss << "Total Employees: " << airport.getemployeecount() << "\n";
                ss << "Total Crew Teams: " << airport.getCrewCount() << "\n";
                displayArea.setText(ss.str());
                currentScreen = Screen::VIEW_FLIGHTS;
            }
            else if (btnExit.isClicked(window))
                window.close();
        }

        //  FLIGHT MANAGEMENT
        else if (currentScreen == Screen::FLIGHT_MANAGEMENT)
        {
            sf::Text title("Flight Management", font, 40);
            title.setPosition(480, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            btnAddFlight.draw(window);
            btnViewFlights.draw(window);
            btnEditFlight.draw(window);
            btnAssignCrewToFlight.draw(window);
            btnBack.draw(window);

            if (btnAddFlight.isClicked(window))
            {
                inputFlightID.clear();
                inputOrigin.clear();
                inputDestination.clear();
                inputAircraft.clear();
                currentScreen = Screen::ADD_FLIGHT;
            }
            else if (btnViewFlights.isClicked(window))
            {
                displayArea.setText(getFlightListText(airport));
                currentScreen = Screen::VIEW_FLIGHTS;
            }
            else if (btnEditFlight.isClicked(window))
            {
                inputEditFlightID.clear();
                inputNewStatus.clear();
                currentScreen = Screen::EDIT_FLIGHT;
            }
            else if (btnAssignCrewToFlight.isClicked(window))
            {
                inputAssignFlightID.clear();
                inputAssignCrewID.clear();
                currentScreen = Screen::ASSIGN_CREW_TO_FLIGHT;
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::MAIN;
        }

        else if (currentScreen == Screen::ADD_FLIGHT)
        {
            sf::Text title("Add New Flight", font, 40);
            title.setPosition(500, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            inputFlightID.draw(window);
            inputOrigin.draw(window);
            inputDestination.draw(window);
            inputAircraft.draw(window);
            btnSubmitFlight.draw(window);
            btnBack.draw(window);

            if (btnSubmitFlight.isClicked(window))
            {
                int flightID = inputFlightID.getInt();
                string origin = inputOrigin.getText();
                string dest = inputDestination.getText();
                string aircraft = inputAircraft.getText();

                if (flightID > 0 && !origin.empty() && !dest.empty() && !aircraft.empty())
                {
                    airport.AddFlight(flightID, origin, dest, aircraft);
                    msgBox.show("Flight " + to_string(flightID) + " added successfully!");
                    inputFlightID.clear();
                    inputOrigin.clear();
                    inputDestination.clear();
                    inputAircraft.clear();
                }
                else
                    msgBox.show("Please fill all fields correctly!", true);
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::FLIGHT_MANAGEMENT;
        }

        else if (currentScreen == Screen::EDIT_FLIGHT) // editing flight status
        {
            sf::Text title("Update Flight Status", font, 40);
            title.setPosition(450, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            sf::Text instructions("Valid statuses: Scheduled, Boarding, Departed, Cancelled", font, 16);
            instructions.setPosition(200, 170);
            instructions.setFillColor(sf::Color(200, 200, 200));
            window.draw(instructions);

            inputEditFlightID.draw(window); // taking inputs
            inputNewStatus.draw(window);
            btnSubmitStatus.draw(window);
            btnBack.draw(window);

            sf::Text flightListTitle("Current Flights:", font, 20);
            flightListTitle.setPosition(600, 295);
            flightListTitle.setFillColor(sf::Color::White);
            window.draw(flightListTitle);

            sf::RectangleShape flightListBox;
            flightListBox.setSize({550, 450});
            flightListBox.setPosition({600, 330});
            flightListBox.setFillColor(sf::Color(20, 20, 20));
            flightListBox.setOutlineColor(sf::Color(100, 100, 100));
            flightListBox.setOutlineThickness(2);
            window.draw(flightListBox);

            sf::Text flightListText;
            flightListText.setFont(font);
            flightListText.setCharacterSize(14);
            flightListText.setFillColor(sf::Color::White);
            flightListText.setPosition(610, 340);
            flightListText.setString(getFlightListText(airport));
            window.draw(flightListText);

            if (btnSubmitStatus.isClicked(window))
            {
                int flightID = inputEditFlightID.getInt();
                string status = inputNewStatus.getText();

                if (flightID > 0 && !status.empty())
                {
                    if (status == "Scheduled" || status == "Boarding" ||
                        status == "Departed" || status == "Cancelled")
                    {
                        airport.updateFlightStatus(flightID, status);
                        msgBox.show("Flight " + to_string(flightID) + " status updated!");
                    }
                    else
                        msgBox.show("Invalid status. Use Scheduled/Boarding/Departed/Cancelled", true);
                }
                else
                    msgBox.show("Please enter Flight ID and Status!", true);
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::FLIGHT_MANAGEMENT;
        }

        else if (currentScreen == Screen::VIEW_FLIGHTS) // viewing flights
        {
            sf::Text title("Flight Information", font, 40);
            title.setPosition(480, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            displayArea.draw(window);
            btnBack.draw(window);

            if (btnBack.isClicked(window))
                currentScreen = Screen::FLIGHT_MANAGEMENT;
        }

        else if (currentScreen == Screen::ASSIGN_CREW_TO_FLIGHT) // assigning crew to flights
        {
            sf::Text title("Assign Crew to Flight", font, 40);
            title.setPosition(450, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            sf::Text instructions("Enter the Flight ID and Crew Team ID to assign", font, 16);
            instructions.setPosition(200, 170);
            instructions.setFillColor(sf::Color(200, 200, 200));
            window.draw(instructions);

            inputAssignFlightID.draw(window); // taking inputs
            inputAssignCrewID.draw(window);
            btnSubmitAssign.draw(window);
            btnBack.draw(window);

            sf::Text flightListTitle("Available Flights:", font, 18);
            flightListTitle.setPosition(600, 220);
            flightListTitle.setFillColor(sf::Color::White);
            window.draw(flightListTitle);

            sf::RectangleShape flightListBox;
            flightListBox.setSize({550, 250});
            flightListBox.setPosition({600, 250});
            flightListBox.setFillColor(sf::Color(20, 20, 20));
            flightListBox.setOutlineColor(sf::Color(100, 100, 100));
            flightListBox.setOutlineThickness(2);
            window.draw(flightListBox);

            sf::Text flightListText;
            flightListText.setFont(font);
            flightListText.setCharacterSize(13);
            flightListText.setFillColor(sf::Color::White);
            flightListText.setPosition(610, 260);
            flightListText.setString(getFlightListText(airport));
            window.draw(flightListText);

            sf::Text crewListTitle("Available Crew Teams:", font, 18);
            crewListTitle.setPosition(600, 520);
            crewListTitle.setFillColor(sf::Color::White);
            window.draw(crewListTitle);

            sf::RectangleShape crewListBox;
            crewListBox.setSize({550, 200});
            crewListBox.setPosition({600, 550});
            crewListBox.setFillColor(sf::Color(20, 20, 20));
            crewListBox.setOutlineColor(sf::Color(100, 100, 100));
            crewListBox.setOutlineThickness(2);
            window.draw(crewListBox);

            sf::Text crewListText;
            crewListText.setFont(font);
            crewListText.setCharacterSize(13);
            crewListText.setFillColor(sf::Color::White);
            crewListText.setPosition(610, 560);
            crewListText.setString(getCrewListText(airport));
            window.draw(crewListText);

            if (btnSubmitAssign.isClicked(window))
            {
                int flightID = inputAssignFlightID.getInt();
                string crewID = inputAssignCrewID.getText();

                if (flightID > 0 && !crewID.empty())
                {
                    CrewTeam *crew = airport.findCrewTeam(crewID);
                    if (crew)
                    {
                        bool flightFound = false; // check if the flight is created
                        for (int i = 0; i < airport.getFlightCount(); i++)
                        {
                            if (airport.getFlight(i)->getFlightID() == flightID)
                            {
                                flightFound = true;
                                break;
                            }
                        }

                        if (flightFound)
                        {
                            airport.AssignCrewToFlight(flightID, crew);
                            msgBox.show("Crew '" + crew->getTeamName() + "' assigned to Flight " + to_string(flightID));
                            inputAssignFlightID.clear();
                            inputAssignCrewID.clear();
                        }
                        else
                        {
                            msgBox.show("Flight " + to_string(flightID) + " not found!", true);
                        }
                    }
                    else
                    {
                        msgBox.show("Crew team '" + crewID + "' not found!", true);
                    }
                }
                else
                {
                    msgBox.show("Please enter Flight ID and Crew Team ID!", true);
                }
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::FLIGHT_MANAGEMENT;
        }

        else if (currentScreen == Screen::PASSENGER_SERVICES) // passenger services
        {
            sf::Text title("Passenger Services", font, 40);
            title.setPosition(480, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            btnAddPassenger.draw(window);
            btnViewPassengers.draw(window);
            btnBack.draw(window);

            if (btnAddPassenger.isClicked(window))
            {
                inputPassFlightID.clear();
                inputPassName.clear();
                inputPassEmail.clear();
                inputPassPhone.clear();
                inputPassport.clear();
                inputNationality.clear();
                currentScreen = Screen::ADD_PASSENGER;
            }

            else if (btnViewPassengers.isClicked(window))
            {
                inputViewPassFlightID.clear();
                displayArea.clear();
                currentScreen = Screen::VIEW_PASSENGERS;
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::MAIN;
        }

        else if (currentScreen == Screen::ADD_PASSENGER) // add passenger on the flight
        {
            sf::Text title("Add Passenger to Flight", font, 40);
            title.setPosition(420, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            inputPassFlightID.draw(window);
            inputPassName.draw(window);
            inputPassEmail.draw(window);
            inputPassPhone.draw(window);
            inputPassport.draw(window);
            inputNationality.draw(window);
            btnSubmitPassenger.draw(window);
            btnBack.draw(window);

            if (btnSubmitPassenger.isClicked(window))
            {
                int flightID = inputPassFlightID.getInt();
                string name = inputPassName.getText();
                string email = inputPassEmail.getText();
                string phone = inputPassPhone.getText();
                string passport = inputPassport.getText();
                string nationality = inputNationality.getText();

                if (flightID > 0 && !name.empty() && !passport.empty())
                {
                    Passenger *p = new Passenger(name, "", email, phone, passport, nationality);
                    airport.addPassengerToFlight(flightID, p);
                    msgBox.show("Passenger " + name + " added to flight " + to_string(flightID));
                    inputPassFlightID.clear();
                    inputPassName.clear();
                    inputPassEmail.clear();
                    inputPassPhone.clear();
                    inputPassport.clear();
                    inputNationality.clear();
                }
                else
                    msgBox.show("Please fill Flight ID, Name, and Passport!", true);
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::PASSENGER_SERVICES;
        }

        else if (currentScreen == Screen::VIEW_PASSENGERS) // view passengers on a flight
        {
            sf::Text title("View Flight Passengers", font, 40);
            title.setPosition(440, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            sf::Text instructions("Enter a Flight ID and click 'View Passengers'", font, 16);
            instructions.setPosition(200, 170);
            instructions.setFillColor(sf::Color(200, 200, 200));
            window.draw(instructions);

            inputViewPassFlightID.draw(window);
            btnViewPassSubmit.draw(window);
            displayArea.draw(window);
            btnBack.draw(window);

            if (btnViewPassSubmit.isClicked(window))
            {
                int flightID = inputViewPassFlightID.getInt();

                if (flightID <= 0)
                {
                    displayArea.setText("Please enter a valid Flight ID!");
                }
                else
                {
                    Flight *f = nullptr;

                    for (int i = 0; i < airport.getFlightCount(); i++) // finding flight through id
                    {
                        if (airport.getFlight(i)->getFlightID() == flightID)
                        {
                            f = airport.getFlight(i);
                            break;
                        }
                    }

                    if (f == nullptr)
                    {
                        displayArea.setText("Flight " + to_string(flightID) + " not found!\n\nPlease check the flight ID and try again.");
                    }
                    else
                    {
                        displayArea.setText(getPassengerListText(f)); // show the passengers
                    }
                }
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::PASSENGER_SERVICES;
        }

        else if (currentScreen == Screen::EMPLOYEE_MANAGEMENT) // employee management
        {
            sf::Text title("Employee Management", font, 40);
            title.setPosition(460, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            btnAddEmployee.draw(window);
            btnViewEmployees.draw(window);
            btnViewFlightCrew.draw(window);
            btnBack.draw(window);

            if (btnAddEmployee.isClicked(window))
            {
                inputEmpName.clear();
                inputEmpID.clear();
                inputEmpEmail.clear();
                inputEmpPhone.clear();
                inputempnumber.clear();
                inputEmpSalary.clear();
                inputEmpPosition.clear();
                currentScreen = Screen::ADD_EMPLOYEE;
            }
            else if (btnViewEmployees.isClicked(window))
            {
                displayArea.setText(getEmployeeListText(airport));
                currentScreen = Screen::VIEW_EMPLOYEES;
            }
            else if (btnViewFlightCrew.isClicked(window))
            {
                inputViewCrewFlightID.clear();
                displayArea.clear();
                currentScreen = Screen::VIEW_FLIGHT_CREW;
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::MAIN;
        }

        else if (currentScreen == Screen::ADD_EMPLOYEE) // add employee
        {
            sf::Text title("Add New Employee", font, 40);
            title.setPosition(480, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            inputEmpName.draw(window);
            inputEmpID.draw(window);
            inputEmpEmail.draw(window);
            inputEmpPhone.draw(window);
            inputempnumber.draw(window);
            inputEmpSalary.draw(window);
            inputEmpPosition.draw(window);
            btnSubmitEmployee.draw(window);
            btnBack.draw(window);

            if (btnSubmitEmployee.isClicked(window))
            {
                string name = inputEmpName.getText();
                string id = inputEmpID.getText();
                string email = inputEmpEmail.getText();
                string phone = inputEmpPhone.getText();
                string empnum = inputempnumber.getText();
                double salary = inputEmpSalary.getDouble();
                string position = inputEmpPosition.getText();

                if (!name.empty() && !empnum.empty() && !position.empty())
                {
                    Employee *e = new Employee(name, id, email, phone, empnum, salary, position);
                    airport.assignEmployee(e);
                    msgBox.show("Employee " + name + " added successfully!");

                    inputEmpName.clear();
                    inputEmpID.clear();
                    inputEmpEmail.clear();
                    inputEmpPhone.clear();
                    inputempnumber.clear();
                    inputEmpSalary.clear();
                    inputEmpPosition.clear();
                }
                else
                    msgBox.show("Please fill Name, Employee Number, and Position!", true);
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::EMPLOYEE_MANAGEMENT;
        }

        else if (currentScreen == Screen::VIEW_EMPLOYEES) // view employees
        {
            sf::Text title("All Employees", font, 40);
            title.setPosition(520, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            displayArea.draw(window);
            btnBack.draw(window);

            if (btnBack.isClicked(window))
                currentScreen = Screen::EMPLOYEE_MANAGEMENT;
        }

        else if (currentScreen == Screen::VIEW_FLIGHT_CREW) // view crew
        {
            sf::Text title("View Flight Crew", font, 40);
            title.setPosition(480, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            sf::Text instructions("Enter a Flight ID and click 'View Crew'", font, 16);
            instructions.setPosition(200, 170);
            instructions.setFillColor(sf::Color(200, 200, 200));
            window.draw(instructions);

            inputViewCrewFlightID.draw(window);
            btnViewCrewSubmit.draw(window);
            displayArea.draw(window);
            btnBack.draw(window);

            if (btnViewCrewSubmit.isClicked(window))
            {
                int flightID = inputViewCrewFlightID.getInt();

                if (flightID <= 0)
                {
                    displayArea.setText("Please enter a valid Flight ID!");
                }
                else
                {
                    Flight *f = nullptr;

                    for (int i = 0; i < airport.getFlightCount(); i++) // find the flight thru id
                    {
                        if (airport.getFlight(i)->getFlightID() == flightID)
                        {
                            f = airport.getFlight(i);
                            break;
                        }
                    }

                    if (f == nullptr)
                    {
                        displayArea.setText("Flight " + to_string(flightID) + " not found!\n\nPlease check the flight ID and try again.");
                    }
                    else
                    {
                        displayArea.setText(getFlightCrewText(f)); // crew info display
                    }
                }
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::EMPLOYEE_MANAGEMENT;
        }

        else if (currentScreen == Screen::CREW_MANAGEMENT) // crew management
        {
            sf::Text title("Crew Management", font, 40);
            title.setPosition(490, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            btnCreateCrew.draw(window);
            btnAddEmployeeToCrew.draw(window);
            btnViewCrews.draw(window);
            btnViewCrewMembers.draw(window);
            btnBack.draw(window);

            if (btnCreateCrew.isClicked(window))
            {
                inputCrewID.clear();
                inputCrewName.clear();
                currentScreen = Screen::CREATE_CREW;
            }
            else if (btnAddEmployeeToCrew.isClicked(window))
            {
                inputAddEmpCrewID.clear();
                inputAddEmpID.clear();
                currentScreen = Screen::ADD_EMPLOYEE_TO_CREW;
            }
            else if (btnViewCrews.isClicked(window))
            {
                displayArea.setText(getCrewListText(airport));
                currentScreen = Screen::VIEW_CREWS;
            }
            else if (btnViewCrewMembers.isClicked(window))
            {
                displayArea.setText(formatCrewMembersText(airport));
                currentScreen = Screen::VIEW_CREW_MEMBERS;
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::MAIN;
        }

        else if (currentScreen == Screen::CREATE_CREW) // create the crew
        {
            sf::Text title("Create Crew Team", font, 40);
            title.setPosition(480, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);
            inputCrewID.draw(window);
            inputCrewName.draw(window);
            btnSubmitCrew.draw(window);
            btnBack.draw(window);

            if (btnSubmitCrew.isClicked(window))
            {
                string crewID = inputCrewID.getText();
                string crewName = inputCrewName.getText();

                if (!crewID.empty() && !crewName.empty())
                {
                    airport.createCrewTeam(crewID, crewName);
                    msgBox.show("Crew team " + crewName + " created!");
                    inputCrewID.clear();
                    inputCrewName.clear();
                }
                else
                    msgBox.show("Please fill Team ID and Name!", true);
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::CREW_MANAGEMENT;
        }

        else if (currentScreen == Screen::ADD_EMPLOYEE_TO_CREW)
        {
            sf::Text title("Add Employee to Crew Team", font, 40);
            title.setPosition(420, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            inputAddEmpCrewID.draw(window);
            inputAddEmpID.draw(window);
            btnSubmitAddEmpToCrew.draw(window);
            btnBack.draw(window);

            sf::RectangleShape rightDisplayBox;
            rightDisplayBox.setSize({580, 500});
            rightDisplayBox.setPosition({650, 200});
            rightDisplayBox.setFillColor(sf::Color(20, 20, 20));
            rightDisplayBox.setOutlineColor(sf::Color(100, 100, 100));
            rightDisplayBox.setOutlineThickness(2);
            window.draw(rightDisplayBox);

            sf::Text displayText;
            displayText.setFont(font);
            displayText.setCharacterSize(14);
            displayText.setFillColor(sf::Color::White);
            displayText.setPosition(660, 210);

            stringstream ss;
            ss << getEmployeeListText(airport) << "\n" << getCrewListText(airport);
            displayText.setString(ss.str());
            window.draw(displayText);

            if (btnSubmitAddEmpToCrew.isClicked(window))
            {
                string crewID = inputAddEmpCrewID.getText();
                string empID = inputAddEmpID.getText();

                if (!crewID.empty() && !empID.empty())
                {
                    CrewTeam *crew = airport.findCrewTeam(crewID);

                    if (!crew)
                    {
                        msgBox.show("Crew team not found!", true);
                    }
                    else
                    {
                        Employee *emp = nullptr;
                        for (int i = 0; i < airport.getemployeecount(); i++) // finding employee through the employee num
                        {
                            if (airport.getEmployee(i)->getEmployeeID() == empID)
                            {
                                emp = airport.getEmployee(i);
                                break;
                            }
                        }

                        if (!emp)
                        {
                            msgBox.show("Employee not found!", true);
                        }
                        else
                        {
                            crew->addMember(emp); // adding emp to the crew
                            msgBox.show("Employee " + emp->getName() + " added to crew " + crew->getTeamName());
                            inputAddEmpCrewID.clear();
                            inputAddEmpID.clear();
                        }
                    }
                }
                else
                {
                    msgBox.show("Please fill Crew ID and Employee Number!", true);
                }
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::CREW_MANAGEMENT;
        }

        else if (currentScreen == Screen::VIEW_CREWS) // view crew
            {
                sf::Text title("All Crew Teams", font, 40);
                title.setPosition(500, 100);
                title.setFillColor(sf::Color::White);
                window.draw(title);

                displayArea.draw(window);
                btnBack.draw(window);

                if (btnBack.isClicked(window))
                    currentScreen = Screen::CREW_MANAGEMENT;
            }



        else if (currentScreen == Screen::ASSIGN_CREW) // assign crew
        {
            sf::Text title("Add employee to crew", font, 40);
            title.setPosition(450, 100);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            inputAssignFlightID.draw(window);
            inputAssignCrewID.draw(window);
            btnSubmitAssign.draw(window);
            btnBack.draw(window);

            sf::RectangleShape rightDisplayBox;
            rightDisplayBox.setSize({580, 500});
            rightDisplayBox.setPosition({650, 200});
            rightDisplayBox.setFillColor(sf::Color(20, 20, 20));
            rightDisplayBox.setOutlineColor(sf::Color(100, 100, 100));
            rightDisplayBox.setOutlineThickness(2);
            window.draw(rightDisplayBox);

            sf::Text displayText;
            displayText.setFont(font);
            displayText.setCharacterSize(14);
            displayText.setFillColor(sf::Color::White);
            displayText.setPosition(660, 210);

            stringstream ss;
            ss << getFlightListText(airport) << "\n"
               << getCrewListText(airport);
            displayText.setString(ss.str());
            window.draw(displayText);

            if (btnSubmitAssign.isClicked(window))
            {
                int flightID = inputAssignFlightID.getInt();
                string crewID = inputAssignCrewID.getText();

                if (flightID > 0 && !crewID.empty())
                {
                    CrewTeam *crew = airport.findCrewTeam(crewID);
                    if (crew)
                    {
                        airport.AssignCrewToFlight(flightID, crew);
                        msgBox.show("Crew assigned to flight " + to_string(flightID));
                    }
                    else
                        msgBox.show("Crew team not found!", true);
                }
                else
                    msgBox.show("Please fill Flight ID and Crew ID!", true);
            }
            else if (btnBack.isClicked(window))
                currentScreen = Screen::CREW_MANAGEMENT;
        }

else if (currentScreen == Screen::VIEW_CREW_MEMBERS)  //view members
{
    sf::Text title("Crew Team Members", font, 40);
    title.setPosition(450, 50);
    title.setFillColor(sf::Color::White);
    window.draw(title);

    sf::RectangleShape displayBox;
    displayBox.setSize({850, 700});     
    displayBox.setPosition({350, 120});   
    displayBox.setFillColor(sf::Color(20, 20, 20));
    displayBox.setOutlineColor(sf::Color(100, 100, 100));
    displayBox.setOutlineThickness(2);
    window.draw(displayBox);

    sf::Text displayText;
    displayText.setFont(font);
    displayText.setCharacterSize(16);
    displayText.setFillColor(sf::Color::White);
    displayText.setPosition(360, 130);    
    
    string formattedText = formatCrewMembersText(airport);
    displayText.setString(formattedText);
    window.draw(displayText);

    btnBack.draw(window);

    if (btnBack.isClicked(window))
        currentScreen = Screen::CREW_MANAGEMENT;
}

        msgBox.draw(window);
        window.display();
    }

    return 0;
}
