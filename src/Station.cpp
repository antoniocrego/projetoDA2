#include "Station.h"

Station::Station() {
    this->name = "";
    this->district = "";
    this->municipality = "";
    this->township = "";
    this->line = "";
}

Station::Station(string name, string district, string municipality, string township, string line) {
    this->name=name;
    this->district=district;
    this->municipality=municipality;
    this->township=township;
    this->line=line;
}

string Station::getName() {
    return this->name;
}

string Station::getDistrict() {
    return this->district;
}

string Station::getLine() {
    return this->line;
}

string Station::getMunicipality() {
    return this->municipality;
}

string Station::getTownship() {
    return this->township;
}

bool Station::operator<(Station s1) const {
    if(this->township == s1.township){
        if(this->municipality == s1.municipality){
            if(this->district == s1.district){
                return this->name < s1.name;
            }
            return this->district < s1.district;
        }
        return this->municipality < s1.municipality;
    }
    return this->township < s1.township;
}