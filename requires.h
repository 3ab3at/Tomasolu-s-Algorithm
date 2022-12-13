#include "macros.h"
#include "ReservationStation.cpp"
#include "Instruction.cpp"

int MEM[MEMORY_SIZE];
short REGS[REG_NUM];
map<int, string> REGS_WAITING;
// bool term;
int last_issued;
// Instruction* last;

ReservationStation load_stations[LOAD_STATIONS_NUM];
ReservationStation store_stations[STORE_STATIONS_NUM];
ReservationStation beq_stations[BEQ_STATIONS_NUM];
ReservationStation jal_ret_stations[JAL_RET_STATIONS_NUM];
ReservationStation add_addi_stations[ADD_ADDI_STATIONS_NUM];
ReservationStation neg_stations[NEG_STATIONS_NUM];
ReservationStation nor_stations[NOR_STATIONS_NUM];
ReservationStation mult_stations[MULT_STATIONS_NUM];

vector<Instruction> instructions;
int PC, cycle;