#pragma once
#include <vector>
#include <set>

using std::vector;
using std::set;
using std::string;


struct Location
{
	int x, y, z;
	Location(int, int, int);
	Location(int, int);
	Location();
	string to_string() const;
	vector<Location> adjecentLocations() const;
	void move(int, int, int);
	void move(int, int);
	Location above() const;
};

inline bool operator==(const Location& lhs, const Location& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
inline bool operator!=(const Location& lhs, const Location& rhs) { return !operator==(lhs, rhs); }
inline bool operator< (const Location& lhs, const Location& rhs) { return (lhs.x + lhs.y * 10 + lhs.z * 100) < (rhs.x + rhs.y * 10 + rhs.z * 100); }
inline bool operator> (const Location& lhs, const Location& rhs) { return  operator< (rhs, lhs); }
inline bool operator<=(const Location& lhs, const Location& rhs) { return !operator> (lhs, rhs); }
inline bool operator>=(const Location& lhs, const Location& rhs) { return !operator< (lhs, rhs); }

namespace std {

	template <>
	struct hash<Location>
	{
		std::size_t operator()(const Location& l) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			//x & y range from -50 to 50, z ranges from 0-10
			return l.x + l.y * 10 + l.z * 100;
		}
	};
}