#ifndef IP_ADDRESS_HPP
#define IP_ADDRESS_HPP

#include <iostream>
#include <string>
#include <vector>


namespace ip {
	class IpAddress;

	class IpAddress {


	public:
		unsigned m_part1;
		unsigned m_part2;
		unsigned m_part3;
		unsigned m_part4;
		unsigned m_port;

		IpAddress();
		IpAddress(unsigned part1, unsigned part2, unsigned part3, unsigned part4, unsigned port);
		IpAddress(const std::string& ipStr);
		std::string str() const;

		friend bool operator==(const ip::IpAddress& rhs, const ip::IpAddress& lhs);
		friend bool operator>(const ip::IpAddress& rhs, const ip::IpAddress& lhs);
		friend bool operator<(const ip::IpAddress& rhs, const ip::IpAddress& lhs);
		friend std::ostream& operator<<(std::ostream& out, const ip::IpAddress& ip) {
			out << ip.str();
			return out;
		}
	};

};



#endif // !IP_ADDRESS_HPP