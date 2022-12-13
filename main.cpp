#include "requires.h"

void load_instructions(string);
void issue(void);
void execute(deque<int>&);
void write_back(deque<int>&);
void init(void);
void load_data(string);


int main() {
    string inst_path = "C:/Users/Roaa bahaa/Desktop/Tomasolu-s-Algorithm/tests/test1.txt";
    string data_path = "C:/Users/Roaa bahaa/Desktop/Tomasolu-s-Algorithm/tests/mem.txt";
    // cout << "Enter the path of the instruction file: "; cin >> inst_path;
    // cout << "Enter the path of the data memory file:"; cin >> data_path;
    load_instructions(inst_path);
    load_data(data_path);
    init();
    // // cout << instructions.size();
    // // cout << instructions[6].written_time;
    // deque<int> exec_inst;
    // while (instructions.back().written_time == -1) {
    //     // cout <<  "Cycle " << cycle << ":\n";
        
    //     issue();
    //   //  cout << "issue done \n";
    //     // cout << load_stations[0].index << " " <<load_stations[0].busy << "\n";
    //     // cout << PC << "\n";
    //     execute(exec_inst);
    //     // if (term) break;
    //     //cout << "exec done \n";
    //     write_back(exec_inst);
    //     //cout << "wb done \n";

    //     // cout << instructions[0].started_execution_time << " " << instructions[0].final_execution_time << "\n";
    //     cycle++;
    //     // cout << "Executed inst: ";
    //     // for (auto u : exec_inst) cout << u << " ";
    //     // cout << "\n";
    // }
    // cout << REGS[1];
    // cout << "Size: " << instructions.size();
    // for (auto u : instructions) cout << u.op << " " << u.issued_time << " " << u.started_execution_time << " " <<u.final_execution_time << " " << u.written_time << "\n";
    for (auto u : instructions) cout << u.label << " " << u.op << "\n";
    // for (auto u : address_to_index) cout << u.first << " " << u.second << "\n";
    return 0;
}

void init() {
    for (int i = 0; i < REG_NUM; i++) REGS_WAITING[i] = "";
    PC = 0;
    // term = false;
    last_issued = -1;
    cycle = 1;
    // last = NULL;
    REGS[0] = 0;
    for (auto u : load_stations) u.busy = false;
    for (auto u : store_stations) u.busy = false;
    for (auto u : beq_stations) u.busy = false;
    for (auto u : jal_ret_stations) u.busy = false;
    for (auto u : add_addi_stations) u.busy = false;
    for (auto u : neg_stations) u.busy = false; 
    for (auto u : nor_stations) u.busy = false;
    for (auto u : mult_stations) u.busy = false;
}

void load_data(string path) {
    fstream inst_test(path);
    if (inst_test.fail()) cout << "Failed to open data memory file.\n";
    string line;
    int index;
    int value;
    while(getline(inst_test, line)) {
        stringstream ss(line);
        ss >> index;
        ss >> value;
        MEM[index] = value;
    }
}

void load_instructions(string path) {
    fstream inst_test(path);
    if (inst_test.fail()) cout << "Failed to open data memory file.\n";
    string line;
    while(getline(inst_test, line)) instructions.push_back(Instruction(line));
    int index = 0;
    for ( auto u : instructions) address_to_index[u.label] = index++;
}

void issue() {
    // cout << PC;
    Instruction* cur_inst = &instructions[PC >> 2];
    if (PC > (int)instructions.size() * 4) 
    {
        // last = &instructions[last_issued];
        return;
    } 
    // last_issued = PC >> 2;
    // cout <<  cur_inst->op << endl;
    if (cur_inst->op == "LOAD") {
        ReservationStation* free_station = NULL;
        for (int i = 0; i < LOAD_STATIONS_NUM; i++) if(load_stations[i].busy) continue; else { free_station = &load_stations[i]; break;}
        if (free_station != NULL) {
            free_station->w2 = "";
            cur_inst->issued_time = cycle;
            free_station->A = cur_inst->imm;
            free_station->index = PC >> 2;
            free_station->busy = true;
            if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
            else free_station->r1 = cur_inst->rs1, free_station->w1 = "";
        }
    }
    else if (cur_inst->op == "STORE") {
        ReservationStation* free_station = NULL;
        for (int i = 0; i < STORE_STATIONS_NUM; i++) if(store_stations[i].busy) continue; else free_station = &store_stations[i];
        if (free_station != NULL) {
            cur_inst->issued_time = cycle;
            free_station->A = cur_inst->imm;
            free_station->index = PC >> 2;
            free_station->busy = true;
            if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
            else free_station->r1 = cur_inst->rs1, free_station->w1 = "";
            if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
            else free_station->r2 = cur_inst->rs2, free_station->w2 = "";
        }
    }
    else if (cur_inst->op == "BEQ") {
        if (!beq_stations[0].busy) {
            cur_inst->issued_time = cycle;
            ReservationStation* free_station = &beq_stations[0];
            free_station->A = cur_inst->imm;
            free_station->index = PC >> 2; 
            free_station->busy = true;  
            if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
            else free_station->r1 = cur_inst->rs1, free_station->w1 = "";
            if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
            else free_station->r2 = cur_inst->rs2, free_station->w2 = "";
        }
    }
    else if (cur_inst->op == "JAL" || cur_inst->op == "RET") {
        if (!jal_ret_stations[0].busy) {
            cur_inst->issued_time = cycle;
            ReservationStation* free_station = &jal_ret_stations[0];
            free_station->index = PC >> 2;   
            free_station->busy = true;    
        }  
    }
    else if (cur_inst->op == "NEG") {
        if (!neg_stations[0].busy) {
            ReservationStation* free_station = &neg_stations[0];
            free_station->w2 = "";
            cur_inst->issued_time = cycle;
            free_station->index = PC >> 2;
            free_station->busy = true;
            if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
            else free_station->r1 = cur_inst->rs1, free_station->w1 = "";
        }
    }
    else if (cur_inst->op == "NOR") {
        if (!nor_stations[0].busy) {
            cur_inst->issued_time = cycle;
            ReservationStation* free_station = &nor_stations[0];
            free_station->index = PC >> 2;
            free_station->busy = true;
            if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
            else free_station->r1 = cur_inst->rs1, free_station->w1 = "";
            if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
            else free_station->r2 = cur_inst->rs2, free_station->w2 = "";
        }
    }
    else if (cur_inst->op == "MUL") {
        // cout << "test \n";
        if (!mult_stations[0].busy) {
            cur_inst->issued_time = cycle;
            ReservationStation* free_station = &mult_stations[0];
            free_station->index = PC >> 2;
            free_station->busy = true;
            if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
            else free_station->r1 = cur_inst->rs1, free_station->w1 = "";
            if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
            else free_station->r2 = cur_inst->rs2, free_station->w2 = "";
        }
    }
    else if (cur_inst->op == "ADD") {
        ReservationStation* free_station = NULL;
        for (int i = 0; i < ADD_ADDI_STATIONS_NUM; i++) if(add_addi_stations[i].busy) continue; else free_station = &add_addi_stations[i];
        if (free_station != NULL) {
            cur_inst->issued_time = cycle;
            free_station->index = PC >> 2;
            free_station->busy = true;
            if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
            else free_station->r1 = cur_inst->rs1, free_station->w1 = "";
            if(REGS_WAITING[cur_inst->rs2] != "") free_station->w2 = REGS_WAITING[cur_inst->rs2];
            else free_station->r2 = cur_inst->rs2, free_station->w2 = "";
        }
    }
    else if (cur_inst->op == "ADDI") {
        ReservationStation* free_station = NULL;
        for (int i = 0; i < ADD_ADDI_STATIONS_NUM; i++) if(add_addi_stations[i].busy) continue; else free_station = &add_addi_stations[i];
        if (free_station != NULL) {
            free_station->w2 = "";
            cur_inst->issued_time = cycle;
            free_station->index = PC >> 2;
            free_station->busy = true;
            free_station->A = cur_inst->imm;
            if(REGS_WAITING[cur_inst->rs1] != "") free_station->w1 = REGS_WAITING[cur_inst->rs1];
            else free_station->r1 = cur_inst->rs1, free_station->w1 = "";
        }
    }
    PC += 4;
}

void execute(deque<int> &wr) 
{
 
    for (int i = 0; i < LOAD_STATIONS_NUM; i++)
    {
        if (load_stations[i].busy)
        {
            if (instructions[load_stations[i].index].issued_time != cycle)
            {
                if (instructions[load_stations[i].index].started_execution_time == -1)
                {
                    if (load_stations[i].w1 == "")
                    {
                        instructions[load_stations[i].index].started_execution_time = instructions[load_stations[i].index].final_execution_time = cycle;
                        load_stations[i].A = instructions[load_stations[i].index].imm + REGS[instructions[load_stations[i].index].rs1];
                    }
                    else
                        continue;
                }
                else
                {
                    instructions[load_stations[i].index].final_execution_time++;
                    // cout << "H:" << instructions[load_stations[i].index].final_execution_time << "\n";
                    if (((instructions[load_stations[i].index].final_execution_time - instructions[load_stations[i].index].started_execution_time) + 1) == 4)
                    {
                        // cout << "HEREEEEE \n";
                        // instructions[load_stations[i].index].final_execution_time = cycle;
                        wr.push_back(load_stations[i].index);
                        load_stations[i].busy = false;
                        // write back next cycle
                    }
                    else
                        continue;
                }
            }
            else
                continue;
        }
        else 
            continue;
    }

    for (int i = 0; i < STORE_STATIONS_NUM; i++)
    {
        if (store_stations[i].busy)
        {
            // cout << "HERE0\n";
            if (instructions[store_stations[i].index].issued_time != cycle)
            {
                // cout << "HERE1\n";
                if (instructions[store_stations[i].index].started_execution_time == -1)
                {
                    // cout << "HERE2\n";
                    if (store_stations[i].w1 == "" && store_stations[i].w2 == "")
                    {
                        // cout << "HERE3\n";
                        instructions[store_stations[i].index].started_execution_time = instructions[store_stations[i].index].final_execution_time = cycle;
                        store_stations[i].A = instructions[store_stations[i].index].imm + REGS[instructions[store_stations[i].index].rs1];
                    }
                    else
                        continue;
                }
                else
                {
                    instructions[store_stations[i].index].final_execution_time++;
                    if (((instructions[store_stations[i].index].final_execution_time - instructions[store_stations[i].index].started_execution_time) + 1) == 3)
                    {
                        // instructions[store_stations[i].index].final_execution_time = cycle;
                        wr.push_back(store_stations[i].index);
                        store_stations[i].busy = 0;
                        // write back next cycle
                    }
                    else
                        continue;
                }
            }
            else
                continue;
        }
        else 
            continue;
    }


    for (int i = 0; i < ADD_ADDI_STATIONS_NUM; i++)
    {
        if (add_addi_stations[i].busy)
        {
            if (instructions[add_addi_stations[i].index].issued_time != cycle)
            {
                if (instructions[add_addi_stations[i].index].started_execution_time == -1)
                {
                    if (add_addi_stations[i].w1 == "" && add_addi_stations[i].w2 == "")
                    {
                        instructions[add_addi_stations[i].index].started_execution_time = instructions[add_addi_stations[i].index].final_execution_time = cycle;
                    }
                    else
                        continue;
                }
                else
                {
                    instructions[add_addi_stations[i].index].final_execution_time++;
                    if (((instructions[add_addi_stations[i].index].final_execution_time - instructions[add_addi_stations[i].index].started_execution_time) + 1) == 2)
                    {
                        wr.push_back(add_addi_stations[i].index);
                        add_addi_stations[i].busy = 0;
                    }
                    else
                        continue;
                }
            }
            else
                continue;
        }
        else 
            continue;
    }

    if (neg_stations[0].busy)
    {
        if (instructions[neg_stations[0].index].issued_time != cycle)
        {
            if (instructions[neg_stations[0].index].started_execution_time == -1)
            {
                if (neg_stations[0].w1 == "")
                {
                    instructions[neg_stations[0].index].started_execution_time = instructions[neg_stations[0].index].final_execution_time = cycle;
                    wr.push_back(neg_stations[0].index);
                    neg_stations[0].busy = 0;                    
                }
            }
        }
    }

    if (nor_stations[0].busy)
    {
        if (instructions[nor_stations[0].index].issued_time != cycle)
        {
            if (instructions[nor_stations[0].index].started_execution_time == -1)
            {
                if ((nor_stations[0].w1 == "") && (nor_stations[0].w2 == ""))
                {
                    instructions[nor_stations[0].index].started_execution_time = instructions[nor_stations[0].index].final_execution_time = cycle;
                    wr.push_back(nor_stations[0].index);
                    nor_stations[0].busy = 0;
                }
            }
        }
    }

    if (mult_stations[0].busy)
    {
        if (instructions[mult_stations[0].index].issued_time != cycle)
        {
            if (instructions[mult_stations[0].index].started_execution_time == -1)
            {
                if ((mult_stations[0].w1 == "") && (mult_stations[0].w2 == ""))
                {
                    instructions[mult_stations[0].index].started_execution_time = instructions[mult_stations[0].index].final_execution_time = cycle;
                }
            }
            else
            {
                instructions[mult_stations[0].index].final_execution_time++;
                if (((instructions[mult_stations[0].index].final_execution_time - instructions[mult_stations[0].index].started_execution_time) + 1) == 8)
                {
                    wr.push_back(mult_stations[0].index);
                    mult_stations[0].busy = 0;
                }
            }
        }
    }

    if (beq_stations[0].busy)
    {
        if (instructions[beq_stations[0].index].issued_time != cycle)
        {
            if (instructions[beq_stations[0].index].started_execution_time == -1)
            {
                if ((beq_stations[0].w1 == "") && (beq_stations[0].w2 == ""))
                {
                    instructions[beq_stations[0].index].started_execution_time = instructions[beq_stations[0].index].final_execution_time = cycle;

                    if (REGS[instructions[beq_stations[0].index].rs1] == REGS[instructions[beq_stations[0].index].rs2])
                    {
                        int i = beq_stations[0].index + 1;
                        while (i < (PC/4))
                        {
                            if (mult_stations[0].busy && mult_stations[0].index == i) 
                            {
                                mult_stations[0].busy = false;
                                instructions[i].issued_time = -1;
                            }
                            else if (nor_stations[0].busy && nor_stations[0].index == i)
                            {
                                nor_stations[0].busy = false;
                                instructions[i].issued_time = -1;
                            } 
                            else if (neg_stations[0].busy && neg_stations[0].index == i)
                            {
                                neg_stations[0].busy = false;
                                instructions[i].issued_time = -1;
                            } 
                            else if (jal_ret_stations[0].busy && jal_ret_stations[0].index == i) 
                            {
                                jal_ret_stations[0].busy = false;
                                instructions[i].issued_time = -1;
                            }
                            else 
                            {
                                for (int j = 0; j < LOAD_STATIONS_NUM; j++)
                                {
                                    if (load_stations[j].busy && load_stations[i].index == i) 
                                    {
                                        load_stations[j].busy = false;
                                        instructions[j].issued_time = -1;
                                        break;
                                    }
                                }

                                for (int j = 0; j < STORE_STATIONS_NUM; j++)
                                {
                                    if (store_stations[j].busy && store_stations[j].index == i) 
                                    {
                                        store_stations[j].busy = false;
                                        instructions[j].issued_time = -1;
                                        break;
                                    }
                                }

                                for (int j = 0; j < ADD_ADDI_STATIONS_NUM; j++)
                                {
                                    if (add_addi_stations[j].busy && add_addi_stations[j].index == i) 
                                    {
                                        add_addi_stations[j].busy = false;
                                        instructions[j].issued_time = -1;
                                        break;
                                    }
                                }
                            }
                            i++;
                        } 

                        PC = PC + instructions[beq_stations[0].index].imm * 4 - 4;  
                        if (PC > ((instructions.size()-1)*4));
                           // term = true;  
                           last_issued = beq_stations[0].index;

                    }
                }
            }
        }
    }
    if (jal_ret_stations[0].busy)
    {
        if (instructions[jal_ret_stations[0].index].issued_time != cycle)
        {
            if (instructions[jal_ret_stations[0].index].started_execution_time == -1)
            {
                instructions[jal_ret_stations[0].index].started_execution_time = instructions[jal_ret_stations[0].index].final_execution_time = cycle;
                int i = jal_ret_stations[0].index + 1;
                if (mult_stations[0].busy && mult_stations[0].index == i) 
                {
                    mult_stations[0].busy = false;
                    instructions[i].issued_time = -1;
                }
                else if (nor_stations[0].busy && nor_stations[0].index == i)
                {
                    nor_stations[0].busy = false;
                    instructions[i].issued_time = -1;
                } 
                else if (neg_stations[0].busy && neg_stations[0].index == i)
                {
                    neg_stations[0].busy = false;
                    instructions[i].issued_time = -1;
                } 
                else if (beq_stations[0].busy && beq_stations[0].index == i) 
                {
                    beq_stations[0].busy = false;
                    instructions[i].issued_time = -1;
                }
                else 
                {
                    for (int j = 0; j < LOAD_STATIONS_NUM; j++)
                    {
                        if (load_stations[j].busy && load_stations[i].index == i) 
                        {
                            load_stations[j].busy = false;
                            instructions[j].issued_time = -1;
                            break;
                        }
                    }

                    for (int j = 0; j < STORE_STATIONS_NUM; j++)
                    {
                        if (store_stations[j].busy && store_stations[j].index == i) 
                        {
                            store_stations[j].busy = false;
                            instructions[j].issued_time = -1;
                            break;
                        }
                    }

                    for (int j = 0; j < ADD_ADDI_STATIONS_NUM; j++)
                    {
                        if (add_addi_stations[j].busy && add_addi_stations[j].index == i) 
                        {
                            add_addi_stations[j].busy = false;
                            instructions[j].issued_time = -1;
                            break;
                        }
                    }
                }
                PC = address_to_index[instructions[jal_ret_stations[0].index].address];      
            }
        }
    }
}

void write_back(deque<int> &executed_instructions) {
    deque<int> new_exec_inst;
    for (auto u : executed_instructions) {
        // cout << "LOOK: " << instructions[u].final_execution_time << " ";
        if (instructions[u].final_execution_time == cycle) {
            new_exec_inst.push_back(u);
            executed_instructions.pop_front();
            // cout << "LOOK2: " << u << " " << new_exec_inst[0] << "\n";
          //  executed_instructions = new_exec_inst;
            continue;
        }
        executed_instructions.pop_front();
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
    executed_instructions = new_exec_inst;

}