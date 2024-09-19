#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

class Airplane {
private:
    std::map<std::string, bool> seats;
    std::map<int, int> seatPrices;
    std::string FlightDate;
    std::string FlightNumber;
    int SeatsPerRow;

public:
    Airplane(int seatsPerRow, const std::string& date, const std::string& number, const std::map<int, int>& prices)
        : SeatsPerRow(seatsPerRow), FlightDate(date), FlightNumber(number), seatPrices(prices) {

        int rows = seatPrices.rbegin()->first;
        for (int i = 1; i <= rows; ++i) {
            for (char j = 'A'; j < 'A' + seatsPerRow; ++j) {
                std::string seat = std::to_string(i) + j;
                seats[seat] = false;
            }
        }
    }

    bool isSeatAvailable(const std::string& seat) const {
        return seats.find(seat) != seats.end() && !seats.at(seat);
    }

    bool bookSeat(const std::string& seat) {
        if (isSeatAvailable(seat)) {
            seats[seat] = true;
            return true;
        }
        return false;
    }

    bool cancelSeat(const std::string& seat) {
        if (seats.find(seat) != seats.end() && seats[seat]) {
            seats[seat] = false;
            return true;
        }
        return false;
    }

    std::string getAvailableSeats() const {
        std::string availableSeats;
        for (const auto& seat : seats) {
            if (!seat.second) {
                availableSeats += seat.first + " ";
            }
        }
        return availableSeats.empty() ? "No available seats" : availableSeats;
    }

    int getSeatPrice(const std::string& seat) const {
        int row = std::stoi(seat.substr(0, seat.size() - 1));
        for (const auto& priceRange : seatPrices) {
            if (row <= priceRange.first) {
                return priceRange.second;
            }
        }
        return 0;
    }

    std::string getFlightDate() const { return FlightDate; }
    std::string getFlightNumber() const { return FlightNumber; }

    friend class Reader;
};


class Ticket {
private:
    std::string Name;
    std::string SeatNumber;
    std::string FlightNumber;
    std::string FlightDate;
    std::string ConfirmationID;

public:
    Ticket(const std::string& name, const std::string& seat, const std::string& flight, const std::string& date, const std::string& confirmationID)
        : Name(name), SeatNumber(seat), FlightNumber(flight), FlightDate(date), ConfirmationID(confirmationID) {}

    std::string getTicketInfo() const {
        return "Name: " + Name + ", Seat: " + SeatNumber + ", Flight: " + FlightNumber + ", Confirmation ID: " + ConfirmationID;
    }

    std::string getFlightNumber() const { return FlightNumber; }
    std::string getFlightDate() const { return FlightDate; }
    std::string getSeatNumber() const { return SeatNumber; }
    std::string getConfirmationID() const { return ConfirmationID; }
    std::string getName() const { return Name; }

    friend class Reader;
};

class Reader {
private:
    std::vector<Ticket> tickets;

public:
    std::vector<Airplane> readConfig(const std::string& filename);

    bool bookSeat(const std::string& date, const std::string& flightNo, const std::string& seat, const std::string& username, std::vector<Airplane>& airplanes);

    bool returnTicket(const std::string& confirmationID, std::vector<Airplane>& airplanes);

    std::string viewTicketDetails(const std::string& confirmationID, const std::vector<Airplane>& airplanes);

    std::string viewTickets(const std::string& username);

    std::string viewFlightTickets(const std::string& flightNumber, const std::string& flightDate);
};

std::vector<Airplane> Reader::readConfig(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Airplane> airplanes;

    if (!file.is_open()) {
        std::cout << "Could not open the file" << std::endl;
        return airplanes;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string flightDate, flightNumber;
        int seatsPerRow;
        std::map<int, int> seatPrices;
        iss >> flightDate >> flightNumber >> seatsPerRow;

        std::string range;
        while (iss >> range) {
            std::string priceStr;
            iss >> priceStr;
            size_t hyphenPos = range.find('-');
            int startRow = std::stoi(range.substr(0, hyphenPos));
            int endRow = std::stoi(range.substr(hyphenPos + 1));
            int price = std::stoi(priceStr.substr(0, priceStr.size() - 1));
            for (int row = startRow; row <= endRow; ++row) {
                seatPrices[row] = price;
            }
        }

        airplanes.emplace_back(seatsPerRow, flightDate, flightNumber, seatPrices);
    }

    file.close();
    return airplanes;
}

bool Reader::bookSeat(const std::string& date, const std::string& flightNo, const std::string& seat, const std::string& username, std::vector<Airplane>& airplanes) {
    for (auto& airplane : airplanes) {
        if (airplane.getFlightNumber() == flightNo && airplane.getFlightDate() == date) {
            if (airplane.bookSeat(seat)) {
                std::string confirmationID = "ID" + std::to_string(tickets.size() + 1);
                tickets.emplace_back(username, seat, flightNo, date, confirmationID);
                std::cout << "Success! " << confirmationID << "\n";
                return true;
            }
            else {
                std::cout << "Seat is not available.\n";
                return false;
            }
        }
    }
    std::cout << "Flight not found.\n";
    return false;
}

bool Reader::returnTicket(const std::string& confirmationID, std::vector<Airplane>& airplanes) {
    for (auto it = tickets.begin(); it != tickets.end(); ++it) {
        if (it->getConfirmationID() == confirmationID) {
            for (auto& airplane : airplanes) {
                if (airplane.getFlightNumber() == it->getFlightNumber() && airplane.getFlightDate() == it->getFlightDate()) {
                    if (airplane.cancelSeat(it->getSeatNumber())) {
                        std::cout << "Ticket is returned. Price of this seat: " << airplane.getSeatPrice(it->getSeatNumber()) << "\n";
                        tickets.erase(it);
                        return true;
                    }
                }
            }
        }
    }
    std::cout << "This ticket was not found";
    return false;
}

std::string Reader::viewTicketDetails(const std::string& confirmationID, const std::vector<Airplane>& airplanes) {
    for (const auto& ticket : tickets) {
        if (ticket.getConfirmationID() == confirmationID) {
            for (const auto& airplane : airplanes) {
                if (airplane.getFlightNumber() == ticket.getFlightNumber() && airplane.getFlightDate() == ticket.getFlightDate()) {
                    return "Flight Number: " + ticket.getFlightNumber() +
                        ", Flight Date: " + ticket.getFlightDate() +
                        ", Seat Number: " + ticket.getSeatNumber() +
                        ", Seat Price: " + std::to_string(airplane.getSeatPrice(ticket.getSeatNumber()));
                }
            }
        }
    }
    return "This ticket was not found";
}

std::string Reader::viewTickets(const std::string& username) {
    std::string result;
    for (const auto& ticket : tickets) {
        if (ticket.getName() == username) {
            result += ticket.getTicketInfo() + "\n";
        }
    }
    return result.empty() ? "This person did not book any tickets" : result;
}

std::string Reader::viewFlightTickets(const std::string& flightNumber, const std::string& flightDate) {
    std::string result;
    for (const auto& ticket : tickets) {
        if (ticket.getFlightNumber() == flightNumber && ticket.getFlightDate() == flightDate) {
            result += ticket.getTicketInfo() + "\n";
        }
    }
    return result.empty() ? "No tickets booked for this flight." : result;
}

int main() {
    Reader reader;
    std::string FilePath = "../config.txt";
    std::vector<Airplane> airplanes = reader.readConfig(FilePath);

    std::cout << "1. Type 'check' to see available seats" << "\n";
    std::cout << "2. Type 'book' to buy the ticket" << "\n";
    std::cout << "3. Type 'return' to return the ticket" << "\n";
    std::cout << "4. Type 'view' to view the booking information" << "\n";
    std::cout << "5. Type 'viewuser' to view all tickets booked by a user" << "\n";
    std::cout << "6. Type 'viewflight' to view all of the booked tickets for a particular flight" << "\n";
    std::cout << "7. Type 'exit' to exit" << "\n";

    std::string command;
    while (true) {
        std::cin >> command;

        if (command == "check") {
            std::string flightNo, date;
            std::cout << "Enter the flight number: ";
            std::cin >> flightNo;
            std::cout << "Enter the flight date: ";
            std::cin >> date;

            bool found = false;
            for (const auto& airplane : airplanes) {
                if (airplane.getFlightNumber() == flightNo && airplane.getFlightDate() == date) {
                    std::cout << "Available seats: " << airplane.getAvailableSeats() << "\n";
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Flight not found" << "\n";
            }
        }
        else if (command == "book") {
            std::string flightNo, date, seat, username;
            std::cout << "Enter the flight date: ";
            std::cin >> date;
            std::cout << "Enter the flight number: ";
            std::cin >> flightNo;
            std::cout << "Type a seat number you want to book: ";
            std::cin >> seat;
            std::cout << "Enter your name: ";
            std::cin >> username;

            reader.bookSeat(date, flightNo, seat, username, airplanes);
        }
        else if (command == "return") {
            std::string confirmationID;
            std::cout << "Enter your ID: ";
            std::cin >> confirmationID;

            reader.returnTicket(confirmationID, airplanes);
        }
        else if (command == "view") {
            std::string confirmationID;
            std::cout << "Enter the ID: ";
            std::cin >> confirmationID;

            std::cout << reader.viewTicketDetails(confirmationID, airplanes) << "\n";
        }
        else if (command == "viewuser") {
            std::string username;
            std::cout << "Enter the name: ";
            std::cin >> username;

            std::cout << reader.viewTickets(username) << "\n";
        }
        else if (command == "viewflight") {
            std::string flightNo, date;
            std::cout << "Enter the flight number: ";
            std::cin >> flightNo;
            std::cout << "Enter the flight date: ";
            std::cin >> date;

            std::cout << reader.viewFlightTickets(flightNo, date) << "\n";
        }
        else if (command == "exit") {
            break;
        }
    }

    return 0;
}
