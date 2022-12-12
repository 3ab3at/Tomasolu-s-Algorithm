#include <bits/stdc++.h>
#include <vector>
// #include "Instruction.cpp"

using namespace std;

class ReservationStation {
public:
    bool busy;
    uint r1, r2, A, index;
    string w1, w2;
    // Instruction curr;

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
    this->r1 = 0; 
    this->r2 = 0;
    this->A = 0;
    this->w1 = "";
    this->w2 = "";
}

ReservationStation::ReservationStation(bool busy, int r1, int r2, int A, string w1, string w2) {
    this->busy = busy;
    this->r1 = r1; 
    this->r2 = r2;
    this->A = A;
    this->w1 = w1;
    this->w2 = w2;
}

bool ReservationStation::isReady() {
    return !(this->busy);
}

pair<int,int> ReservationStation::getSourceRegisters() {
    return make_pair(r1, r2);
}

pair<string, string> ReservationStation::getSourceWaitingReservationStations() {
    return make_pair(w1, w2);
}
