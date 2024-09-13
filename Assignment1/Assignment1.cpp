#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

class Airplane {
private:
	std::string NumberOfSeats;
	std::string AvailableSeat;
	std::string OccupiedSeat;
	std::string FlightDate;
	std::string FlightNumber;
	std::string SeatPrice;
public:
	Airplane(std::string seats, std::string free, std::string taken, std::string date, std::string number, std::string price)
		:NumberOfSeats(seats), AvailableSeat(free), OccupiedSeat(taken), FlightDate(date), FlightNumber(number), SeatPrice(price) {}

	friend class Reader; //likely a temporary thing
};

class Ticket {
private:
	std::string Name;
	std::string SeatNumber;
	std::string FlightNumber;
	std::string Status;
public:
	Ticket(std::string name, std::string seat, std::string flight, std::string status)
		:Name(name), SeatNumber(seat), FlightNumber(flight), Status(status) {}

	friend class Reader; //the same goes here, too
};

class Reader {
public:
	std::string getNumberOfSeats(const Airplane& airplane) const;
	std::string getAvailableSeat(const Airplane& airplane) const;
	std::string getOccupiedSeat(const Airplane& airplane) const;
	std::string getFlightDate(const Airplane& airplane) const;
	std::string getFlightNumber(const Airplane& airplane) const;
	std::string getSeatPrice(const Airplane& airplane) const;

	std::string getName(const Ticket& ticket) const;
	std::string getSeatNumber(const Ticket& ticket) const;
	std::string getFlightNumber(const Ticket& ticket) const;
	std::string getStatus(const Ticket& ticket) const;

	std::vector<Airplane> readConfig(const std::string& filename);
};

std::string Reader::getNumberOfSeats(const Airplane& airplane) const {
	return airplane.NumberOfSeats;
}

std::string Reader::getAvailableSeat(const Airplane& airplane) const {
	return airplane.AvailableSeat;
}

std::string Reader::getOccupiedSeat(const Airplane& airplane) const {
	return airplane.OccupiedSeat;
}

std::string Reader::getFlightDate(const Airplane& airplane) const {
	return airplane.FlightDate;
}

std::string Reader::getFlightNumber(const Airplane& airplane) const {
	return airplane.FlightNumber;
}

std::string Reader::getSeatPrice(const Airplane& airplane) const {
	return airplane.SeatPrice;
}

std::string Reader::getName(const Ticket& ticket) const {
	return ticket.Name;
}

std::string Reader::getSeatNumber(const Ticket& ticket) const {
	return ticket.SeatNumber;
}

std::string Reader::getFlightNumber(const Ticket& ticket) const {
	return ticket.FlightNumber;
}

std::string Reader::getStatus(const Ticket& ticket) const {
	return ticket.Status;
}

std::vector<Airplane> Reader::readConfig(const std::string& filename) {
	std::ifstream file(filename);
	std::vector<Airplane> airplanes;

	if (!file.is_open()) {
		std::cout << "Could not open the file" << std::endl;
		return airplanes;
	}

	std::string line;
	int Records;

	std::getline(file, line);
	Records = std::stoi(line);

	for (int i = 0; i < Records; i++) {
		std::string flightDate, flightNumber, seatRows, firstPrice, secondPrice;
		std::getline(file, flightDate, ' ');
		std::getline(file, flightNumber, ' ');
		std::getline(file, seatRows, ' ');
		std::getline(file, firstPrice, ' ');
		std::getline(file, secondPrice, ' ');

		std::string seatPrice = firstPrice + " " + secondPrice;

		Airplane airplane(seatRows, "0", "0", flightDate, flightNumber, seatPrice);
		airplanes.push_back(airplane);
	}
	file.close();
	return airplanes;
}

int main() {
	Reader reader;
	std::string FilePath = "config.txt";
	std::vector<Airplane> airplanes = reader.readConfig(FilePath);

	std::cout << "1. Type 'check' to see available seats" << "\n";
	std::cout << "2. Type 'book' to buy the ticket" << "\n";
	std::cout << "3. Type 'return' to return the ticket" << "\n";
	std::cout << "4. Type 'view' to view the booking information" << "\n";
	std::cout << "5. Type 'view username' with your name to view information in more detail" << "\n";
	std::cout << "6. Type 'view flight' to view all of the booked tickets for a particular flight" << "\n";
	std::cout << "7. Type 'exit' to exit" << "\n";

	std::string command;

	while (true) {
		std::cout << "Type your choice: ";
		std::cin >> command;

		std::transform(command.begin(), command.end(), command.begin(), ::tolower);

		if (command == "check") {
			std::string FlightNumber, FlightDate;
			std::cout << "Enter the flight number: ";
			std::cin >> FlightNumber;
			std::cout << "Enter the date: ";
			std::cin >> FlightDate;
			
			bool found = false;
			for (const auto& airplane : airplanes) {
				if (reader.getFlightNumber(airplane) == FlightNumber && reader.getFlightDate(airplane) == FlightDate) {
					std::cout << "Available seats: " << reader.getAvailableSeat(airplane);
					found = true;
					break;
				}
				if (!found) {
					std::cout << "Flight not found";
				}
			}
		}

	}
}