#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <iomanip>

using namespace std;

// Struct to store flight details
struct Flight {
    string departure;  // Starting point of the flight
    string arrival;    // Destination of the flight
    int seats;         // Total seats in the flight
    int bookedSeats;   // Seats already booked
    double basePrice;  // Ticket price for the flight
    map<string, int> passengers; // Stores passenger names and how many tickets they booked
};

// Main class for managing the reservation system
class AirlineReservationSystem {
private:
    map<string, Flight> flights; // Map to store flight details using flight ID

    // Function to save flight data to a file
    void saveData() {
        ofstream file("flights.txt");
        if (!file.is_open()) {
            // If file doesn't open, show error
            cout << "Error: Could not save flight data." << endl;
            return;
        }
        // Write flight details to the file
        for (auto& flight : flights) {
            file << flight.first << " " << flight.second.departure << " " << flight.second.arrival 
                 << " " << flight.second.seats << " " << flight.second.bookedSeats << " " 
                 << flight.second.basePrice << endl;
        }
        file.close(); // Close the file after writing
    }

    // Function to load flight data from a file
    void loadData() {
        ifstream file("flights.txt");
        if (!file.is_open()) {
            // If no file exists, it means no previous data is present
            cout << "No previous flight data found." << endl;
            return;
        }
        while (!file.eof()) {
            string flightId, departure, arrival;
            int seats, bookedSeats;
            double basePrice;
            file >> flightId >> departure >> arrival >> seats >> bookedSeats >> basePrice;
            if (file.fail()) break; // To avoid reading extra junk at the end of the file
            flights[flightId] = {departure, arrival, seats, bookedSeats, basePrice, {}};
        }
        file.close(); // Close the file after reading
    }

public:
    // Constructor to load data when the program starts
    AirlineReservationSystem() {
        loadData();
    }

    // Destructor to save data before the program ends
    ~AirlineReservationSystem() {
        saveData();
    }

    // Function to add a new flight
    void addFlight() {
        string flightId, departure, arrival;
        int seats;
        double basePrice;

        // Input details of the new flight
        cout << "Enter flight ID: ";
        cin >> flightId;
        cout << "Enter departure location: ";
        cin >> departure;
        cout << "Enter arrival location: ";
        cin >> arrival;
        cout << "Enter number of seats: ";
        cin >> seats;
        cout << "Enter base price per ticket: ";
        cin >> basePrice;

        // Add the flight to the map
        flights[flightId] = {departure, arrival, seats, 0, basePrice, {}};
        cout << "Flight added successfully!" << endl;
    }

    // Function to book tickets
    void bookSeats() {
        string flightId, passengerName;
        int numTickets;

        // Get flight ID from user
        cout << "Enter flight ID: ";
        cin >> flightId;

        if (flights.find(flightId) != flights.end()) { // Check if the flight exists
            Flight& flight = flights[flightId];
            cout << "Enter your name: ";
            cin >> passengerName;
            cout << "Enter number of tickets to book: ";
            cin >> numTickets;

            // Check if enough seats are available
            if (numTickets <= (flight.seats - flight.bookedSeats)) {
                flight.bookedSeats += numTickets;
                flight.passengers[passengerName] += numTickets;

                // Calculate the total price
                double totalPrice = flight.basePrice * numTickets;
                cout << "Booking successful! Total price: $" << fixed << setprecision(2) << totalPrice << endl;
            } else {
                // Not enough seats
                cout << "Not enough seats available. Only " << (flight.seats - flight.bookedSeats) << " seats left." << endl;
            }
        } else {
            // Flight ID not found
            cout << "Flight not found!" << endl;
        }
    }

    // Function to cancel tickets
    void cancelBooking() {
        string flightId, passengerName;
        int ticketsToCancel;

        // Get flight ID from user
        cout << "Enter flight ID: ";
        cin >> flightId;

        if (flights.find(flightId) != flights.end()) { // Check if the flight exists
            Flight& flight = flights[flightId];
            cout << "Enter the name used for booking: ";
            cin >> passengerName;

            // Check if passenger exists
            if (flight.passengers.find(passengerName) != flight.passengers.end()) {
                cout << "Enter number of tickets to cancel: ";
                cin >> ticketsToCancel;

                // Check if tickets to cancel are valid
                if (ticketsToCancel <= flight.passengers[passengerName]) {
                    flight.bookedSeats -= ticketsToCancel;
                    flight.passengers[passengerName] -= ticketsToCancel;
                    if (flight.passengers[passengerName] == 0) {
                        flight.passengers.erase(passengerName); // Remove passenger if no tickets left
                    }
                    cout << "Tickets canceled successfully!" << endl;
                } else {
                    cout << "You cannot cancel more tickets than you booked!" << endl;
                }
            } else {
                cout << "No booking found under the name " << passengerName << "." << endl;
            }
        } else {
            // Flight ID not found
            cout << "Flight not found!" << endl;
        }
    }

    // Function to display all flights
    void displayFlights() {
        if (flights.empty()) {
            // No flights available
            cout << "No flights available!" << endl;
            return;
        }

        // Show details of all flights
        cout << "Available Flights:" << endl;
        for (auto& flight : flights) {
            cout << "Flight ID: " << flight.first 
                 << ", From: " << flight.second.departure 
                 << ", To: " << flight.second.arrival 
                 << ", Seats: " << flight.second.seats 
                 << ", Booked: " << flight.second.bookedSeats 
                 << ", Price: $" << flight.second.basePrice << endl;
        }
    }
};

// Main function
int main() {
    AirlineReservationSystem system;
    int choice;

    // Menu-driven program
    while (true) {
        cout << "\nAirline Reservation System Menu:\n";
        cout << "1. Add Flight\n";
        cout << "2. Book Seats\n";
        cout << "3. Cancel Booking\n";
        cout << "4. Display Flights\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Perform actions based on user choice
        switch (choice) {
            case 1:
                system.addFlight();
                break;
            case 2:
                system.bookSeats();
                break;
            case 3:
                system.cancelBooking();
                break;
            case 4:
                system.displayFlights();
                break;
            case 5:
                cout << "Exiting system. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again!" << endl;
        }
    }
}