#include <iostream>
#include <cmath>

using namespace std;

double getDistance(double latitude1, double longitude1, double latitude2, double longitude2)
{
	double radius = 6371, rad = 3.14159265 / 180;
	double latitude = (latitude2 - latitude1) * rad;
	double longitude = (longitude2 - longitude1) * rad;
	double rez = sin(latitude / 2) * sin(latitude / 2) + cos(latitude1 * rad) * cos(latitude2 * rad) * sin(longitude / 2) * sin(longitude / 2);
	double d = 2 * radius * atan2(sqrt(rez), sqrt(1 - rez));
	return d;
}
int main()
{
	cout << "Distance in km " << getDistance(32.17, -73.21, 34.77, -78.1) << endl;
}