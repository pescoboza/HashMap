// Pedro Escoboza
// A01251531
// TCB1004.500
// 21/11/2020

#include <iostream>
#include <fstream>
#include <vector>

#include "Timer.hpp"
#include "IpAddress.hpp"
#include "HashMapInternalChaining.hpp"

template <class K, class T>
using MapContent = std::vector<std::pair<K, T>>;

template <class K>
using Lookups = std::vector<K>;

const std::vector<size_t> PRIMES{
	 7U,
	 63U,
	 511U,
	 1023U,
	 2047U,
	 4095U,
	 8191U,
	 16383U,
	 32767U,
	 65535U,
	 131071U,
	 262143U,
	 524287U,
	 1048575U,
	 2097151U,
	 4194303U,
	 8388607U,
	 16777215U
};

/**
* Calculates the size needed for the bucket count.
* Time: O(n)
* Space: O(1)
* 
* @param size Number of elements to store in the hash map
* @param table Vector of prime sizes for bucket count
* @return Minimum prime bucket count from the table given that matches the size
*/
unsigned getBucketCount(size_t size, const std::vector<size_t>& table = PRIMES ) {

	for (const auto& s : table) {
		if (s >= size) {
			return s;
		}
	}

	return table[table.size() - 1];
}


class Ip : public IpAddress {

public:
	Ip(unsigned part_1, unsigned part_2, unsigned part_3, unsigned part_4) : IpAddress{ part_1, part_2, part_3, part_4, 0U } {}

	friend std::ostream& operator<<(std::ostream& out, const Ip& ip) {
		out << ip.m_part1 << '.' << ip.m_part2 << '.' << ip.m_part3 << '.' << ip.m_part4;
		return out;
	}
};

class Port : public IpAddress {

public:
	Port(unsigned port) : IpAddress{ 0U, 0U, 0U, 0U, port} {}

	friend std::ostream& operator<<(std::ostream& out, const Port& port) {
		out << port.m_port;
		return out;
	}

	friend bool operator==(const Port& l, const Port& r) {
		return l.m_port == r.m_port;
	}
};

std::pair<Port, Ip> getIpAndPortFromAccess(const IpAddress& connection){
	return std::make_pair<Port, Ip>( connection.m_port, { connection.m_part1, connection.m_part2, connection.m_part3, connection.m_part4 });
}


int main() {
	using InputMap = HashMapInternalChaining<Port, Ip>;
	
	



	std::cout << "Tests done. Press enter to exit.";
	std::cin.get();
}
