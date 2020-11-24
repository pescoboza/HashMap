// Pedro Escoboza
// A01251531
// TCB1004.500
// 21/11/2020

#include <iostream>
#include <fstream>
#include <vector>

#include "Timer.hpp"
#include "fileio.hpp"
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

const size_t IP_MAP_SIZE{ PRIMES[1] };

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

// Helper to parse the ip from the file line
std::string parseIpStr(const std::string& line) {
	// Get the file line into a stream to output tokens
	std::istringstream fullLine{ line };


	// Throw away unsused information
	{
		std::string str;
		fullLine >> str;
		fullLine >> str;
		fullLine >> str;
	}

	// Extract the ip string
	std::string ipStr;
	fullLine >> ipStr;

	return ipStr;
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

struct AccessSummary {
	Ip m_ip;
	unsigned m_numConnections;

	AccessSummary(const Ip& ip) : m_ip{ ip }, m_numConnections{ 0U }{}
	void operator++() { m_numConnections++; }

	friend std::ostream operator<<(std::ostream& out, const AccessSummary& as) {
		out << "IP: " << as.m_ip << " CONNECTIONS: " << as.m_numConnections;
	}
};

int main() {
	using IpMap = HashMapInternalChaining<Ip, unsigned>;
	using PortMap = HashMapInternalChaining<Port, IpMap>;

	
	std::vector<std::string> lines{ fio::readLines("bitacora3.txt")};


	PortMap hashMap{getBucketCount(lines.size())};

	for (const auto& line : lines) {
		auto entry{ getIpAndPortFromAccess(parseIpStr(line)) };
		Port& port{ entry.first };
		Ip& ip{ entry.second };


		auto res{hashMap.find(port)};

		// Port not found, create it
		if (res == nullptr) {
			IpMap ipMap{IP_MAP_SIZE};
			ipMap.insert(ip, 1U);
			hashMap.insert(port, ipMap);
		}
		// Port found
		else {
			// Get the ip map of the port
			auto& ipMap{ res->second };

			// Attempt to emplace new ip
			auto res{ipMap.insert(ip, 1U)};

			// Check if the element was not emplaced
			if (!res.first) {
				// Increment the access count
				res.second->second++;
			}
		}
	}

	
	
	



	std::cout << "Tests done. Press enter to exit.";
	std::cin.get();
}
