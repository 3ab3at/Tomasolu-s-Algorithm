
#include <bits/stdc++.h>

using namespace std;

int main ()
{
    string label, op;
    string inst = "ADD R1, R2, R3";
    stringstream ss(inst);
    // ss >> op;
    string token = "";

    getline(ss, token, ':');
    if (token != inst)
    {
        label = token;
        ss >> op;
        while (ss >> token)
        {
            cout << token << "," << endl;
        }
        cout << "End:\n";
    }
    else
    {
        stringstream ss(inst);
        ss >> op;
        label = "";
        while (ss >> token)
        {
            cout << token << "," << endl;
        }
        cout << "Endx:\n";

    }

    cout << label << "," << endl << op << "," << endl;
    return 0;
}