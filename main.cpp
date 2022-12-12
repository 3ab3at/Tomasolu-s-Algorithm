#include "requires.h"

void load_instructions(void);
void issue(void);
void execute(void);
void write_back(void);
void init(void);

int main() {
    // HANDLE THE INPUT
    init();
    load_instructions();
    issue();

}

void init() {
    for (int i = 0; i < REG_NUM; i++) REGS_WAITING[i] = "";
    PC = 0;
}
void load_instructions() {
    fstream inst_test("./tests/test1.txt");
    string line;
    while(getline(inst_test, line)) instructions.emplace_back(Instruction(line));
    reverse(instructions.begin(), instructions.end());
}

void issue() {
    Instruction cur_inst = instructions.back();
    instructions.pop_back();
    if (cur_inst.op == "LOAD") {
        if (load_stations[0].busy && load_stations[0].busy) {
            // DO NOT ISSUE IF NOT FREE RESERVATION STATION
            reverse(instructions.begin(), instructions.end());
            instructions.emplace_back(cur_inst);
            reverse(instructions.begin(), instructions.end());
            return;
        }
        ReservationStation* free_station = (load_stations[0].busy ? &load_stations[0] : &load_stations[1]);  
        free_station->A = cur_inst.imm;
        if(REGS_WAITING[cur_inst.rA] != "") {
            free_station->A = cur_inst.imm;
            free_station->rA = -1;
            free_station->wA = REGS_WAITING[cur_inst.rA];
            return;
        }
        free_station->A = cur_inst.imm;
        free_station->rA = cur_inst.rA;

    }
    else if (cur_inst.op == "STORE") {

    }
    else if (cur_inst.op == "BEQ") {

    }
    else if (cur_inst.op == "JAL" || cur_inst.op == "RET") {

    }
    else if (cur_inst.op == "NEG") {

    }
    else if (cur_inst.op == "NOR") {

    }
    else if (cur_inst.op == "MUL") {

    }
}

void execute() {

}

void write_back() {

}