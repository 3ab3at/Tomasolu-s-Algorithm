#include <bits/stdc++.h>

using namespace std;

class ReservationStation {
public:
    bool busy;
    uint rA, rB, A;
    string wA, wB;  

public:
    ReservationStation();
    ReservationStation(bool, int, int, int, string, string);

    //Attribute getters
    bool isReady();
    pair<int,int> getSourceRegisters();
    pair<string, string> getSourceWaitingReservationStations();
};

ReservationStation::ReservationStation() {
    this->busy = false;
    this->rA = 0; 
    this->rB = 0;
    this->A = 0;
    this->wA = "";
    this->wB = "";
}

ReservationStation::ReservationStation(bool busy, int rA, int rB, int A, string wA, string wB) {
    this->busy = busy;
    this->rA = rA; 
    this->rB = rB;
    this->A = A;
    this->wA = wA;
    this->wB = wB;
}

bool ReservationStation::isReady() {
    return !(this->busy);
}

pair<int,int> ReservationStation::getSourceRegisters() {
    return make_pair(rA, rB);
}

pair<string, string> ReservationStation::getSourceWaitingReservationStations() {
    return make_pair(wA, wB);
}
