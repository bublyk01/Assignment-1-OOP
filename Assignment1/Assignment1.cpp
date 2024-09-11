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
};

//class Reader {

//};
