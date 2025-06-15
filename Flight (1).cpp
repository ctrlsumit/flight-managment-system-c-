#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <map>
#include <set>

using namespace std;

class Flight {
private:
    string flightNumber;
    string departure;
    string destination;
    string departureTime;
    string arrivalTime;
    int totalSeats;
    map<string, set<string>> bookedSeats;  // passport -> seats

public:
    Flight(string num, string dep, string dest, string depTime, string arrTime, int seats)
        : flightNumber(num), departure(dep), destination(dest), 
          departureTime(depTime), arrivalTime(arrTime), totalSeats(seats) {}

    // Getters
    string getFlightNumber() const { return flightNumber; }
    string getDeparture() const { return departure; }
    string getDestination() const { return destination; }
    string getDepartureTime() const { return departureTime; }
    string getArrivalTime() const { return arrivalTime; }
    int getTotalSeats() const { return totalSeats; }
    
    int getBookedCount() const {
        int count = 0;
        for (const auto& entry : bookedSeats) {
            count += entry.second.size();
        }
        return count;
    }
    
    int getAvailableSeats() const { 
        return totalSeats - getBookedCount(); 
    }

    bool isSeatAvailable(const string& seat) const {
        for (const auto& entry : bookedSeats) {
            if (entry.second.find(seat) != entry.second.end()) {
                return false;
            }
        }
        return true;
    }

    void displayAvailableSeats() const {
        cout << "\nAvailable Seats: ";
        for (int i = 1; i <= totalSeats; i++) {
            string seat = to_string(i);
            if (isSeatAvailable(seat)) {
                cout << seat << " ";
            }
        }
        cout << "\n";
    }

    bool bookSeat(const string& passport, const string& seat) {
        if (!isSeatAvailable(seat)) return false;
        bookedSeats[passport].insert(seat);
        return true;
    }

    set<string> getPassengerSeats(const string& passport) const {
        auto it = bookedSeats.find(passport);
        if (it != bookedSeats.end()) return it->second;
        return {};
    }
};

class Passenger {
private:
    string name;
    string passportNumber;
    string contact;
    vector<string> bookedFlights;

public:
    Passenger(string n, string passport, string cnt)
        : name(n), passportNumber(passport), contact(cnt) {}

    // Getters
    string getName() const { return name; }
    string getPassport() const { return passportNumber; }
    string getContact() const { return contact; }
    const vector<string>& getBookedFlights() const { return bookedFlights; }

    void addFlight(string flightNumber) {
        bookedFlights.push_back(flightNumber);
    }
};

class FlightManager {
private:
    vector<Flight> flights;
    vector<Passenger> passengers;

    // Helper functions
    Flight* findFlight(const string& flightNumber) {
        auto it = find_if(flights.begin(), flights.end(), 
            [&](const Flight& f) { return f.getFlightNumber() == flightNumber; });
        return (it != flights.end()) ? &(*it) : nullptr;
    }

    Passenger* findPassenger(const string& passport) {
        auto it = find_if(passengers.begin(), passengers.end(), 
            [&](const Passenger& p) { return p.getPassport() == passport; });
        return (it != passengers.end()) ? &(*it) : nullptr;
    }

    void generateTicket(const Passenger& passenger, Flight& flight, const set<string>& seats) {
        cout << "\n============= E-TICKET ==============\n";
        cout << "PASSENGER: " << passenger.getName() << "\n";
        cout << "PASSPORT: " << passenger.getPassport() << "\n";
        cout << "CONTACT: " << passenger.getContact() << "\n";
        cout << "FLIGHT: " << flight.getFlightNumber() << " | " 
             << flight.getDeparture() << " to " << flight.getDestination() << "\n";
        cout << "DEPARTURE: " << flight.getDepartureTime() << " | ";
        cout << "ARRIVAL: " << flight.getArrivalTime() << "\n";
        cout << "SEATS: ";
        for (const string& seat : seats) {
            cout << seat << " ";
        }
        cout << "\nSTATUS: CONFIRMED\n";
        cout << "====================================\n";
    }

public:
    // For direct adding
    void addFlight(string num, string dep, string dest, string depTime, string arrTime, int seats) {
        flights.emplace_back(num, dep, dest, depTime, arrTime, seats);
    }

    void displayAllFlights() {
        if (flights.empty()) {
            cout << "\nNo flights available!\n";
            return;
        }

        cout << "\n" << string(90, '-') << "\n";
        cout << "| " << setw(12) << left << "Flight No." 
             << "| " << setw(15) << "Departure" 
             << "| " << setw(15) << "Destination"
             << "| " << setw(12) << "Depart Time"
             << "| " << setw(12) << "Arrival Time"
             << "| " << setw(8) << "Booked"
             << "| " << setw(8) << "Available |\n";
        cout << string(90, '-') << "\n";

        for (const auto& flight : flights) {
            cout << "| " << setw(12) << flight.getFlightNumber()
                 << "| " << setw(15) << flight.getDeparture()
                 << "| " << setw(15) << flight.getDestination()
                 << "| " << setw(12) << flight.getDepartureTime()
                 << "| " << setw(12) << flight.getArrivalTime()
                 << "| " << setw(8) << flight.getBookedCount()
                 << "| " << setw(8) << flight.getAvailableSeats() << " |\n";
        }
        cout << string(90, '-') << "\n";
    }

    void searchFlight() {
        string num;
        cout << "Enter Flight Number: ";
        cin >> num;

        Flight* flight = findFlight(num);
        if (flight) {
            cout << "\nFlight Details:\n";
            cout << "Flight Number: " << flight->getFlightNumber() << "\n";
            cout << "Route: " << flight->getDeparture() << " to " << flight->getDestination() << "\n";
            cout << "Time: " << flight->getDepartureTime() << " - " << flight->getArrivalTime() << "\n";
            cout << "Seats Available: " << flight->getAvailableSeats() << "/" << flight->getTotalSeats() << "\n";
        } else {
            cout << "Flight not found!\n";
        }
    }

    void bookTicket() {
        string flightNum, name, passport, contact;
        
        cout << "\nEnter Flight Number: ";
        cin >> flightNum;
        
        Flight* flight = findFlight(flightNum);
        if (!flight) {
            cout << "Flight not found!\n";
            return;
        }
        
        if (flight->getAvailableSeats() == 0) {
            cout << "No seats available on this flight!\n";
            return;
        }
        
        cout << "Passenger Name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Passport Number: ";
        getline(cin, passport);
        
        cout << "Contact Info: ";
        getline(cin, contact);
        
        // Find or add passenger
        Passenger* passenger = findPassenger(passport);
        if (!passenger) {
            passengers.emplace_back(name, passport, contact);
            passenger = &passengers.back();
        }
        
        // Select seats
        set<string> selectedSeats;
        int numSeats;
        cout << "Number of seats to book: ";
        cin >> numSeats;
        
        if (numSeats <= 0 || numSeats > flight->getAvailableSeats()) {
            cout << "Invalid number of seats!\n";
            return;
        }
        
        flight->displayAvailableSeats();
        cin.ignore();
        
        for (int i = 0; i < numSeats; i++) {
            string seat;
            cout << "Select seat #" << i+1 << ": ";
            getline(cin, seat);
            
            if (!flight->isSeatAvailable(seat)) {
                cout << "Seat " << seat << " not available!\n";
                i--;
                continue;
            }
            
            if (flight->bookSeat(passport, seat)) {
                selectedSeats.insert(seat);
                cout << "Seat " << seat << " booked.\n";
            } else {
                cout << "Failed to book seat " << seat << "!\n";
                i--;
            }
        }
        
        passenger->addFlight(flightNum);
        generateTicket(*passenger, *flight, selectedSeats);
        cout << "Booking successful! E-ticket generated.\n";
    }

    void showPassengerInfo() {
        string passport;
        cout << "Enter Passport Number: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        getline(cin, passport); // Use getline to read full passport
        
        Passenger* passenger = findPassenger(passport);
        if (passenger) {
            cout << "\n========== PASSENGER INFORMATION ==========\n";
            cout << "NAME: " << passenger->getName() << "\n";
            cout << "PASSPORT: " << passenger->getPassport() << "\n";
            cout << "CONTACT: " << passenger->getContact() << "\n";
            
            if (!passenger->getBookedFlights().empty()) {
                cout << "\nBOOKED FLIGHTS:\n";
                cout << string(70, '-') << "\n";
                cout << "| " << setw(12) << left << "Flight No." 
                     << "| " << setw(15) << "Departure" 
                     << "| " << setw(15) << "Destination"
                     << "| " << setw(10) << "Seats"
                     << "|\n";
                cout << string(70, '-') << "\n";
                
                for (const string& flightNum : passenger->getBookedFlights()) {
                    Flight* flight = findFlight(flightNum);
                    if (flight) {
                        set<string> seats = flight->getPassengerSeats(passenger->getPassport());
                        string seatList;
                        for (const string& seat : seats) {
                            seatList += seat + " ";
                        }
                        
                        cout << "| " << setw(12) << flight->getFlightNumber()
                             << "| " << setw(15) << flight->getDeparture()
                             << "| " << setw(15) << flight->getDestination()
                             << "| " << setw(10) << seatList
                             << "|\n";
                    }
                }
                cout << string(70, '-') << "\n";
            } else {
                cout << "\nNo flights booked.\n";
            }
        } else {
            cout << "Passenger not found!\n";
        }
    }
};

void displayMenu() {
    cout << "\n===== FLIGHT MANAGEMENT SYSTEM =====\n";
    cout << "1. Display All Flights\n";
    cout << "2. Search Flight\n";
    cout << "3. Book Ticket\n";
    cout << "4. Show Passenger Information\n";
    cout << "5. Exit\n";
    cout << "===================================\n";
    cout << "Enter your choice: ";
}

int main() {
    FlightManager manager;
    int choice;

    // Add sample flights
    manager.addFlight("BA123", "London", "New York", "08:00", "14:30", 50);
    manager.addFlight("EK202", "Dubai", "Singapore", "15:45", "23:15", 40);
    manager.addFlight("AA756", "Chicago", "Los Angeles", "09:30", "12:15", 30);

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                manager.displayAllFlights();
                break;
            case 2:
                manager.searchFlight();
                break;
            case 3:
                manager.bookTicket();
                break;
            case 4:
                manager.showPassengerInfo();
                break;
            case 5:
                cout << "Exiting system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return 0;
}
