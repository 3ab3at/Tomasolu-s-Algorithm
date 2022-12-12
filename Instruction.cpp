#include <bits/stdc++.h>

using namespace std;

class Instruction {
public:
    uint rA, rB, rC;
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
};

Instruction::Instruction (string instruction) {
    inst = instruction;
    istringstream ss(inst);
    ss >> op;
    string token;
    if (op == "LOAD" || op == "STORE") {
        getline(ss, token, ',');
        rA = token[2] - '0';
        getline(ss, token, '(');
        imm = stoi(token.substr(1));
        getline(ss, token);
        rB = token[1] - '0';
        rC = 0;
    }    
    else if (op == "BEQ" || op == "ADDI") {
        getline(ss, token, ',');
        rA = token[2] - '0';   
        getline(ss, token, ',');
        rB = token[2] - '0';  
        getline(ss, token);
        imm = stoi(token.substr(1));
        rC = 0;
    }
    else if (op == "JAL") {
        // HANDLE THE JAL OPERAION
    }
    else if (op == "RET") {
        rA = 1;
    }
    else if (op == "ADD" || op == "NOR" || op == "MUL") {
        getline(ss, token, ',');
        rA = token[2] - '0';  
        getline(ss, token, ',');
        rB = token[2] - '0';  
        getline(ss, token, ',');
        rC = token[2] - '0';  
        imm = 0;
    }
    else if (op == "NEG") {
        getline(ss, token, ',');
        rA = token[2] - '0';  
        getline(ss, token, ',');
        rB = token[2] - '0';  
        rC = 0;
        imm = 0;
    }
    else {
        cout << "Invalid Instruction.\n";
    }
};