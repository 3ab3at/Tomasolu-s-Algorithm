#include <bits/stdc++.h>

using namespace std;

class Instruction {
public:
    uint rd, rs1, rs2;
    int imm;
    uint issued_time, started_execution_time, final_execution_time, written_time, total_execution_time;
    string op, inst;
    
public:

    //Constructors
    Instruction();
    Instruction(string);

    // Attribute getters

};

Instruction::Instruction () {
    inst = "";
    started_execution_time = -1;
    written_time = -1;
};

Instruction::Instruction (string instruction) {
    inst = instruction;
    started_execution_time = -1;
    written_time = -1;
    istringstream ss(inst);
    ss >> op;
    string token;
    if (op == "LOAD") {
        getline(ss, token, ',');
        rd = token[2] - '0';
        getline(ss, token, '(');
        imm = stoi(token.substr(1));
        getline(ss, token);
        rs1 = token[1] - '0';
        rs2 = 0;
    }   
    else if (op == "STORE") {
        getline(ss, token, ',');
        rs2 = token[2] - '0';
        getline(ss, token, '(');
        imm = stoi(token.substr(1));
        getline(ss, token);
        rs1 = token[1] - '0';
        rd = 0;        
    }
    else if (op == "BEQ") {
        getline(ss, token, ',');
        rs1 = token[2] - '0';   
        getline(ss, token, ',');
        rs2 = token[2] - '0';  
        getline(ss, token);
        imm = stoi(token.substr(1));
        rd = 0;
    }
    else if (op == "ADDI") {
        getline(ss, token, ',');
        rd = token[2] - '0';   
        getline(ss, token, ',');
        rs1 = token[2] - '0';  
        getline(ss, token);
        imm = stoi(token.substr(1));
        rs2 = 0;       
    }
    else if (op == "JAL") {
        // HANDLE THE JAL OPERAION
    }
    else if (op == "RET") {
        rd = 1;
    }
    else if (op == "ADD" || op == "NOR" || op == "MUL") {
        getline(ss, token, ',');
        rd = token[2] - '0';  
        getline(ss, token, ',');
        rs1 = token[2] - '0';  
        getline(ss, token, ',');
        rs2 = token[2] - '0';  
        imm = 0;
    }
    else if (op == "NEG") {
        getline(ss, token, ',');
        rd = token[2] - '0';  
        getline(ss, token, ',');
        rs1 = token[2] - '0';  
        rs2 = 0;
        imm = 0;
    }
    else {
        cout << "Invalid Instruction.\n";
    }
};