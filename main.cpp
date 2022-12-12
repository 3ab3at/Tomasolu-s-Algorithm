#include "requires.h"

void load_instructions(void);
void issue(void);
vector<int> execute(void);
void write_back(vector<int>);
void init(void);

int main() {
    string inst_path, data_path;
    cout << "Enter the path of the instruction file: "; cin >> inst_path;
    cout << "Enter the path of the data memory file:"; cin >> data_path;
    load_instructions(inst_path);
    load_data(data_path);
    init();
    while (instructions.back().written_time == -1) {
        issue();
        write_back(execute());
    }
}

void init() {
    for (int i = 0; i < REG_NUM; i++) REGS_WAITING[i] = "";
    PC = 0;
    cycle = 0;
}
void load_instructions(string path) {
    fstream inst_test(path);
    string line;
    while(getline(inst_test, line)) instructions.emplace_back(Instruction(line));
    reverse(instructions.begin(), instructions.end());
}

void load_data(string path) {
    fstream inst_test(path);
    string line;
    while(getline(inst_test, line)) instructions.emplace_back(Instruction(line));
    reverse(instructions.begin(), instructions.end());
}

void issue() {
    Instruction* cur_inst = &instructions[PC];
    if (cur_inst->op == "LOAD") {
        if (load_stations[0].busy && load_stations[1].busy) return;
        cur_inst->issued_time = cycle;
        ReservationStation* free_station = (load_stations[0].busy ? &load_stations[0] : &load_stations[1]);  
        free_station->A = cur_inst->imm;
        free_station->index = PC;
        if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
        else free_station->r1 = cur_inst->rs1;
    }
    else if (cur_inst->op == "STORE") {
        if (store_stations[0].busy && store_stations[1].busy) return;
        cur_inst->issued_time = cycle;
        ReservationStation* free_station = (store_stations[0].busy ? &store_stations[0] : &store_stations[1]);  
        free_station->A = cur_inst->imm;
        free_station->index = PC;
        if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
        else free_station->r1 = cur_inst->rs1;
        if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
        else free_station->r2 = cur_inst->rs2;
    }
    else if (cur_inst->op == "BEQ") {
        if (beq_stations[0].busy) return;
        cur_inst->issued_time = cycle;
        ReservationStation* free_station = &beq_stations[0];
        free_station->A = cur_inst->imm;
        free_station->index = PC;   
        if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
        else free_station->r1 = cur_inst->rs1;
        if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
        else free_station->r2 = cur_inst->rs2;
    }
    else if (cur_inst->op == "JAL" || cur_inst->op == "RET") {
        if (jal_ret_stations[0].busy) return;
        cur_inst->issued_time = cycle;
        ReservationStation* free_station = &jal_ret_stations[0];
        free_station->index = PC;         
    }
    else if (cur_inst->op == "NEG") {
        if (neg_stations[0].busy) return;
        cur_inst->issued_time = cycle;
        ReservationStation* free_station = &neg_stations[0];
        free_station->index = PC;
        if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
        else free_station->r1 = cur_inst->rs1;
    }
    else if (cur_inst->op == "NOR") {
        if (nor_stations[0].busy) return;
        cur_inst->issued_time = cycle;
        ReservationStation* free_station = &nor_stations[0];
        free_station->index = PC;
        if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
        else free_station->r1 = cur_inst->rs1;
        if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
        else free_station->r2 = cur_inst->rs2;
    }
    else if (cur_inst->op == "MUL") {
        if (mult_stations[0].busy) return;
        cur_inst->issued_time = cycle;
        ReservationStation* free_station = &mult_stations[0];
        free_station->index = PC;
        if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
        else free_station->r1 = cur_inst->rs1;
        if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
        else free_station->r2 = cur_inst->rs2;
    }
    PC += 4;
}

vector<int> execute() {

}

void write_back(vector<int> executed_instructions) {
    for (auto u : executed_instructions) {
        instructions[u].written_time = cycle;
        Instruction cur_inst = instructions[u];
        if (cur_inst.op == "LOAD") REGS[cur_inst.rd] = MEM[cur_inst.rs1 + cur_inst.imm];
        else if (cur_inst.op == "STORE") MEM[cur_inst.imm + cur_inst.rs2] = REGS[cur_inst.rs1];
        else if (cur_inst.op == "JAL") REGS[1] = u >> 2;
        else if (cur_inst.op == "ADD") REGS[cur_inst.rd] = REGS[cur_inst.rs1] + REGS[cur_inst.rs2];
        else if (cur_inst.op == "ADDI") REGS[cur_inst.rd] = REGS[cur_inst.rs1] + cur_inst.imm;
        else if (cur_inst.op == "NEG") REGS[cur_inst.rd] = ~REGS[cur_inst.rs1] + 1;
        else if (cur_inst.op == "NOR") REGS[cur_inst.rd] = ~(REGS[cur_inst.rs1] | REGS[cur_inst.rs2]);
        else if (cur_inst.op == "MUL") REGS[cur_inst.rd] = REGS[cur_inst.rs1] * REGS[cur_inst.rs2];
    }
}