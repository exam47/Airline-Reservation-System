
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

class Airline {
private:
    string flightNo, departure, destination;
    int seats;

public:
    Airline(string fNo, string dep, string des, int s)
        : flightNo(fNo), departure(dep), destination(des), seats(s) {}

    string getFlight() const { return flightNo; }
    string getDep() const { return departure; }
    string getDes() const { return destination; }
    int getSeats() const { return seats; }

    void display() const {
        cout << setw(10) << flightNo << setw(15) << departure
             << setw(15) << destination << setw(10) << seats << endl;
    }

    void saveToFile(ofstream& out) const {
        out << flightNo << ":" << departure << ":" << destination << ":" << seats << endl;
    }

    void reserveSeat() { if (seats > 0) seats--; }

    void updateSeatsInFile();
};

vector<Airline> flights;

void initializeFlights() {
    flights.push_back(Airline("F101", "UAE", "England", 50));
    flights.push_back(Airline("F202", "UAE", "UAE", 40));
    flights.push_back(Airline("F303", "UAE", "Canada", 2));

    ofstream out("Flight.txt");
    for (const auto& flight : flights) {
        flight.saveToFile(out);
    }
    out.close();
}

void displayFlights() {
    ifstream in("Flight.txt");
    if (!in) {
        cout << "Error: Cannot open file.
";
        return;
    }

    string line;
    cout << setw(10) << "Flight" << setw(15) << "Departure"
         << setw(15) << "Destination" << setw(10) << "Seats" << endl;
    cout << "------------------------------------------------------
";

    while (getline(in, line)) {
        stringstream ss(line);
        string flightNo, dep, des, seatStr;
        getline(ss, flightNo, ':');
        getline(ss, dep, ':');
        getline(ss, des, ':');
        getline(ss, seatStr);

        cout << setw(10) << flightNo << setw(15) << dep
             << setw(15) << des << setw(10) << seatStr << endl;
    }

    in.close();
}

bool reserveSeatInFile(const string& flightNo) {
    ifstream in("Flight.txt");
    ofstream out("Temp.txt");

    bool found = false;
    string line;

    while (getline(in, line)) {
        stringstream ss(line);
        string fNo, dep, des, seatStr;
        getline(ss, fNo, ':');
        getline(ss, dep, ':');
        getline(ss, des, ':');
        getline(ss, seatStr);

        int seats = stoi(seatStr);

        if (fNo == flightNo && seats > 0) {
            seats--;
            found = true;
            cout << "Seat reserved successfully!
";
        }

        out << fNo << ":" << dep << ":" << des << ":" << seats << endl;
    }

    in.close();
    out.close();

    remove("Flight.txt");
    rename("Temp.txt", "Flight.txt");

    return found;
}

int main() {
    initializeFlights();

    int choice;
    do {
        cout << "
=== Airline Reservation System ===
";
        cout << "1. Display All Flights
";
        cout << "2. Reserve Seat
";
        cout << "3. Exit
";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            displayFlights();
        } else if (choice == 2) {
            string fNo;
            cout << "Enter flight number to reserve seat: ";
            cin >> fNo;
            if (!reserveSeatInFile(fNo)) {
                cout << "Seat not available or flight not found.
";
            }
        } else if (choice == 3) {
            cout << "Thank you for using the Airline Reservation System.
";
        } else {
            cout << "Invalid choice. Try again.
";
        }
    } while (choice != 3);

    return 0;
}
