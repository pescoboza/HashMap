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


const char* INPUT_FILE{ "bitacora3.txt" };
const char* MOST_ACCESSED_PORT_OUTFILE{"most_accessed_port.json"};
const char* NET_MAP_OUTPUT_FILE{ "net_map.txt" };


// Vector of prime numbers to use as dynamic bucket counts for the port map
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

// Bucket count of the ip maps
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
size_t getBucketCount(size_t size, const std::vector<size_t>& table = PRIMES ) {

	for (const auto& s : table) {
		if (s >= size) {
			return s;
		}
	}

	return table[table.size() - 1];
}

/**
* Helper to parse the ip from the file line
* 
* @return String of the ip and port from the file line
*/ 
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


/**
* Extends the IpAddress class to represent an input connection from an ipv4.
* 
* @return Ip
*/
class Ip : public IpAddress {

public:
	struct Hasher {
		
		size_t operator()(const Ip& ip) const {
			return s_hasher(ip.str());
		}

	private:
		static std::hash<std::string> s_hasher;
	};
	
	Ip(unsigned part_1, unsigned part_2, unsigned part_3, unsigned part_4) : IpAddress{ part_1, part_2, part_3, part_4, 0U } {}

	friend std::ostream& operator<<(std::ostream& out, const Ip& ip) {
		out << ip.m_part1 << '.' << ip.m_part2 << '.' << ip.m_part3 << '.' << ip.m_part4;
		return out;
	}
};

std::hash<std::string> Ip::Hasher::s_hasher{};

/**
* Extends the IpAddress to represent an access port in the server.
* 
* @return Port
*/
class Port : public IpAddress {

public:
	struct Hasher {

		size_t operator()(const Port& ip) const {
			return s_hasher(ip.str());
		}

	private: 
		static std::hash<std::string> s_hasher;
	};


	Port(unsigned port) : IpAddress{ 0U, 0U, 0U, 0U, port} {}

	friend std::ostream& operator<<(std::ostream& out, const Port& port) {
		out << port.m_port;
		return out;
	}

	friend bool operator==(const Port& l, const Port& r) {
		return l.m_port == r.m_port;
	}
};

std::hash<std::string> Port::Hasher::s_hasher{};

/**
* Splits the full ip addresss in to ipv4 and port.
* Time: O(1)
* Space: O(1)
* 
* @param connection IpAddress of the the connection
* @return pair of Port and Ip
*/
std::pair<Port, Ip> getIpAndPortFromAccess(const IpAddress& connection){
	return std::make_pair<Port, Ip>( connection.m_port, { connection.m_part1, connection.m_part2, connection.m_part3, connection.m_part4 });
}


/**
* Helper class extending the hash map to add a counter and manage input connections.
* 
* @reutrn IpMap
*/
class IpMap : public HashMapInternalChaining<Ip, unsigned, Ip::Hasher>{
	unsigned m_numConnections;
	
public:
	IpMap() : HashMapInternalChaining<Ip, unsigned, Ip::Hasher>{ IP_MAP_SIZE } {}

	void incNumConnections() {
		m_numConnections++;
	}
	
	unsigned getNumConnections()const {
		return m_numConnections;
	}

};



void run() {
	using PortMap = HashMapInternalChaining<Port, IpMap, Port::Hasher>;

	// Read the log file and store the lines for ease of iteration
	std::vector<std::string> lines{ fio::readLines("bitacora3.txt")};

	// Intialize the port map with the number of buckets corresponding to the lines
	PortMap portMap{getBucketCount(lines.size())};

	// Iterate through each line
	for (const auto& line : lines) {

		// Parse the full ip from the line string
		auto entry{ getIpAndPortFromAccess(parseIpStr(line)) };
		
		// Get the port and ip from the entry
		Port& port{ entry.first };
		Ip& ip{ entry.second };

		// Look for the port in the port hash map
		auto res{portMap.find(port)};

		// Port not found, create it
		if (res == nullptr) {
			// Create new ip map
			IpMap ipMap;
			
			// Add the new ip with frequency of one
			ipMap.insert(ip, 1U);

			// Increment the number of total connections
			ipMap.incNumConnections();

			// Insert the ip map into the 
			portMap.insert(port, ipMap);

		}
		// Port found
		else {
			// Get the ip map of the port
			auto& ipMap{ res->second };

			// Attempt to emplace new ip
			auto res{ipMap.insert(ip, 1U)};

			// Increment the number of total connections
			ipMap.incNumConnections();

			// Check if the element was not emplaced
			if (!res.first) {
				// Increment the access count
				res.second->second++;
			}
			
		}
		
	}

	// Destroy the lines, they are not needed anymore and they take memory
	lines.~vector();
	
	// Open a file to print the map
	std::ofstream netMapOutFile{ NET_MAP_OUTPUT_FILE };
	if (!netMapOutFile.is_open()) {
		std::cerr << "[ERROR] Could not open file '" << NET_MAP_OUTPUT_FILE << "'" << std::endl;
		std::exit(1);
	}

	// Display the built hash map
	netMapOutFile << portMap;
	netMapOutFile.close();


	// Scan the map for the most vulnerable port and store it to a reference
	size_t maxNumConnections{ 0U };
	const std::pair<const Port, IpMap>* mostAccessedPortEntry{ nullptr };
	auto reducerCallback{ 
		[&maxNumConnections, &mostAccessedPortEntry](const PortMap::Entry& entry) {
			size_t numConnections{entry.second.getNumConnections()};

			if (numConnections > maxNumConnections) {
				maxNumConnections = numConnections;
				mostAccessedPortEntry = &entry;
			}
		}
	};

	// Run the callback on each element
	portMap.forEach(reducerCallback);

	std::ofstream portOutFile{ MOST_ACCESSED_PORT_OUTFILE };
	if (!portOutFile.is_open()) {
		std::cerr << "[ERROR] Could not open file '" << MOST_ACCESSED_PORT_OUTFILE << "'" << std::endl;
		std::exit(1);
	}

	// Print the port summary to the file in json format
	portOutFile << "{\n" <<
		"    \"mostAccessedPort\": " << '\"'<<mostAccessedPortEntry->first << '\"' << ",\n" <<
		"    \"numberConnections\": " << '\"' << maxNumConnections << '\"' << ",\n" <<
		"    \"ips\": " << "{\n";
	size_t commaCounter{maxNumConnections};
	mostAccessedPortEntry->second.forEach(
		[&portOutFile, &commaCounter](const IpMap::Entry& entry) {
			commaCounter -= entry.second;
			portOutFile << "        \"" << entry.first << "\": " << entry.second << (commaCounter != 0 ? ",\n" : "\n");
		}
	);
	portOutFile << "    }\n}";
		
	// Close the outpu file
	portOutFile.close();
}

int main() {
	Timer timer;
	try {
		run();
	}
	catch (std::exception& e) {
		std::cerr << e.what(); 
	}
	
	std::cout << "Elapsed seconds: " << timer.elapsed() << std::endl;
	std::cout << "Tests done. Press enter to exit.";
	std::cin.get();
}
