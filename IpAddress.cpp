#include "IpAddress.hpp"

#include <sstream>

IpAddress::IpAddress() :
	m_part1{ 0U },
	m_part2{ 0U },
	m_part3{ 0U },
	m_part4{ 0U },
	m_port{ 0U }{}

IpAddress::IpAddress(unsigned part1, unsigned part2, unsigned part3, unsigned part4, unsigned port) :
	m_part1{ part1 }, 
	m_part2{ part2 }, 
	m_part3{ part3 }, 
	m_part4{ part4 }, 
	m_port{ port }{}


IpAddress::IpAddress(const std::string& ipStr) {
	std::string buff;
	for (const auto& ch : ipStr) {
		buff.push_back(std::isdigit(ch) ? ch : ' ');
	}

	std::istringstream iss{ buff };

	unsigned part1, part2, part3, part4, port;
	iss >> part1 >> part2 >> part3 >> part4 >> port;

	m_part1 = part1;
	m_part2 = part2;
	m_part3 = part3;
	m_part4 = part4;
	m_port = port;
}


std::string IpAddress::str() const {
	std::string buff{ std::to_string(m_part1) };
	buff.push_back('.');
	buff.append(std::to_string(m_part2)).push_back('.');
	buff.append(std::to_string(m_part3)).push_back('.');
	buff.append(std::to_string(m_part4)).push_back(':');
	buff.append(std::to_string(m_port));
	return buff;
}


bool operator==(const IpAddress& l, const IpAddress& r) {
	return (l.m_part1 == r.m_part1 &&
		l.m_part2 == r.m_part2 &&
		l.m_part3 == r.m_part3 &&
		l.m_part4 == r.m_part4 &&
		l.m_port == r.m_port);
}

bool operator>(const IpAddress& l, const IpAddress& r) {
	if (l.m_part1 > r.m_part1) { return true; }
	else if (l.m_part1 < r.m_part1) { return false; }
	else {

		if (l.m_part2 > r.m_part2) { return true; }
		else if (l.m_part2 < r.m_part2) { return false; }
		else {

			if (l.m_part3 > r.m_part3) { return true; }
			else if (l.m_part3 < r.m_part3) { return false; }
			else {

				if (l.m_part4 > r.m_part4) { return true; }
				else if (l.m_part4 < r.m_part4) { return false; }
				else {

					if (l.m_port > r.m_port) { return true; }

				}
			}
		}
	}

	return false;
}

bool operator<(const IpAddress& l, const IpAddress& r) {
	if (l.m_part1 < r.m_part1) { return true; }
	else if (l.m_part1 > r.m_part1) { return false; }
	else {

		if (l.m_part2 < r.m_part2) { return true; }
		else if (l.m_part2 > r.m_part2) { return false; }
		else {

			if (l.m_part3 < r.m_part3) { return true; }
			else if (l.m_part3 > r.m_part3) { return false; }
			else {

				if (l.m_part4 < r.m_part4) { return true; }
				else if (l.m_part4 > r.m_part4) { return false; }
				else {

					if (l.m_port < r.m_port) { return true; }

				}
			}
		}
	}

	return false;
}