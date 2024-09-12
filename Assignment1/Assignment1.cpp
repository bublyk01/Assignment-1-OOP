#include <iostream>

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
	Ticket(std::string name, std::string number, std::string flight, std::string status)
		:Name(name), SeatNumber(number), FlightNumber(flight), Status(status) {}

	friend class Reader; //the same goes here, too
};

class Reader {
public:
	std::string getNumberOfSeats(const Airplane& airplane);
	std::string getAvailableSeat(const Airplane& airplane);
	std::string getOccupiedSeat(const Airplane& airplane);
	std::string getFlightDate(const Airplane& airplane);
	std::string getFlightNumber(const Airplane& airplane);
	std::string getSeatPrice(const Airplane& airplane);

	std::string getName(const Ticket& ticket);
	std::string getSeatNumber(const Ticket& ticket);
	std::string getFlightNumebr(const Ticket& ticket);
	std::string getStatus(const Ticket& ticket);
};
