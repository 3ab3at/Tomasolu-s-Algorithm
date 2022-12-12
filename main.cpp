#include "requires.h"

void load_instructions(void);
void issue(void);
vector<int> execute(void);
void write_back(void);
void init(void);
void init_mem(void);

int main() {
    // HANDLE THE INPUT
    // init();
    // load_instructions();
    // issue();
    init_mem();

    cout << MEM[0] << endl;
    cout << MEM[5] << endl;
    cout << MEM[6] << endl;
    cout << MEM[100] << endl;
    cout << MEM[23] << endl;



}

void init() {
    for (int i = 0; i < REG_NUM; i++) REGS_WAITING[i] = "";
    PC = 0;
    cycle = 0;
}

void init_mem()
{
    fstream inst_test("mem.txt");
    string line;
    int index;
    int value;
    while(getline(inst_test, line))
    {
        stringstream ss (line);
        ss >> index;
        ss >> value;
        MEM[index] = value;
    }
}


void load_instructions() {
    fstream inst_test("./tests/test1.txt");
    string line;
    while(getline(inst_test, line)) instructions.push_back(Instruction(line));
    reverse(instructions.begin(), instructions.end());
}

void issue() {
    Instruction cur_inst = instructions.back();
    instructions.pop_back();
    if (cur_inst.op == "LOAD") {
        if (load_stations[0].busy && load_stations[0].busy) {
            // DO NOT ISSUE IF NOT FREE RESERVATION STATION
            reverse(instructions.begin(), instructions.end());
            instructions.push_back(cur_inst);
            reverse(instructions.begin(), instructions.end());
            return;
        }
        ReservationStation* free_station = (load_stations[0].busy ? &load_stations[0] : &load_stations[1]);  
        free_station->A = cur_inst.imm;
        if(REGS_WAITING[cur_inst.rs1] != "") {
            free_station->A = cur_inst.imm;
            free_station->r1 = -1;
            free_station->w1 = REGS_WAITING[cur_inst.rs1];
            return;
        }
        free_station->A = cur_inst.imm;
        free_station->r1 = cur_inst.rs1;
        // free_station->curr = cur_inst;

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

vector<int> execute() 
{
    vector <int> wr;
    // ReservationStation beq_stations[BEQ_STATIONS_NUM];
    // ReservationStation jal_ret_stations[JAL_RET_STATIONS_NUM];  
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
                    if (((instructions[load_stations[i].index].final_execution_time - instructions[load_stations[i].index].started_execution_time )+1) == 4)
                    {
                        // instructions[load_stations[i].index].final_execution_time = cycle;
                        wr.push_back(load_stations[i].index);
                        load_stations[i].busy = 0;
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
            if (instructions[store_stations[i].index].issued_time != cycle)
            {
                if (instructions[store_stations[i].index].started_execution_time == -1)
                {
                    if (store_stations[i].w1 == "" && store_stations[i].w2 == "")
                    {
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
                }
            }
            else
            {
                instructions[neg_stations[0].index].final_execution_time++;
                if (((instructions[neg_stations[0].index].final_execution_time - instructions[neg_stations[0].index].started_execution_time) + 1) == 1)
                {
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
                }
            }
            else
            {
                instructions[nor_stations[0].index].final_execution_time++;
                if (((instructions[nor_stations[0].index].final_execution_time - instructions[nor_stations[0].index].started_execution_time) + 1) == 1)
                {
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
                if (((instructions[mult_stations[0].index].final_execution_time - instructions[mult_stations[0].index].started_execution_time) + 1) == 1)
                {
                    wr.push_back(mult_stations[0].index);
                    mult_stations[0].busy = 0;
                }
            }
        }
    }


    return wr;
}

void write_back() 
{

}