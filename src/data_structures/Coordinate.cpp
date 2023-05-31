#include <cmath>
#include "Coordinate.h"

/**
 * @brief Default Constructor of Coordinate
 */
Coordinate::Coordinate()
{
    this->latitude = 0;
    this->longitude = 0;
}

/**
 * Constructor of Coordinate
 * @param latitude
 * @param longitude
 */
Coordinate::Coordinate(float latitude, float longitude)
{
    this->latitude = latitude;
    this->longitude = longitude;
}

/**
 * @return coordinate's longitude
 */
float Coordinate::getLongitude() const
{
    return longitude;
}

/**
 * @return coordinate's latitude
 */
float Coordinate::getLatitude() const
{
    return latitude;
}

/**
 * @brief Calculates the distance between two coordinates
 * @param coordinate2
 * @return the distance between the two coordinates
 * Complexity: Time complexity of the powf, sin, cos, sqrt and asin functions of the Math library from the Standard Library
 */
float Coordinate::distance(Coordinate coordinate2) const
{
    float distLongitude = (this->longitude - coordinate2.getLongitude()) * M_PI / 180.0;
    float distLatitude = (this->latitude - coordinate2.getLatitude()) * M_PI / 180.0;

    float radLatitude1 = this->latitude * M_PI / 180.0;
    float radLatitude2 = coordinate2.getLatitude() * M_PI / 180.0;

    float alpha = powf(sin(distLatitude / 2), 2) + powf(sin(distLongitude / 2), 2) * cos(radLatitude1) * cos(radLatitude2);

    float dist = 6371.0 * 2.0 * asin(sqrt(alpha));
    return dist;
}