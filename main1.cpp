
#include <bits/stdc++.h>

using namespace std;

int main ()
{
    string label, op;
    string inst = "LOAD R1, 100(R0)";
    stringstream ss(inst);
    // ss >> op;
    string token = "";

    getline(ss, token, ':');
    if (token != inst)
    {
        label = token;
        ss >> op;
        // while (ss >> token)
        // {
        //     cout << token << "," << endl;
        // }
        // cout << "End:\n";
    }
    else
    {
        stringstream ss2(inst);
        ss2 >> op;
        label = "";
        token = "";
        // while (ss >> token)
        // {
        //     cout << token << "," << endl;
        // }
        // cout << "Endx:\n";
        getline(ss2, token, ',');
        cout << token << endl;
        int rd = token[2] - '0';
        getline(ss2, token, '(');
        cout << token << endl;
        int imm = stoi(token.substr(1));
        getline(ss2, token);
        cout << token << endl;
        int rs1 = token[1] - '0';
        int rs2 = 0;
        cout << label << " " << endl << op << " " << rd << " " << rs1 << " " << rs2 << " "  << imm <<  endl;

    }



    return 0;
}