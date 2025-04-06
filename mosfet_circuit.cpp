#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <map>
#include "mosfet_models.hpp"

using namespace std;
using namespace MosfetLib;

// Function of user input : when the user input only enter then using default value
double get_input(const string& prompt, double default_val = -1) {
    string input;
    cout << prompt << " (Enter to skip): ";
    getline(cin, input);

    if (input.empty()) return default_val;

    stringstream ss(input);
    double val;
    ss >> val;
    return val;
}

// Calculate Id function
double calculate_Id(const Mosfet& m, double Vgs, double Vds, bool ideal = false) {
    if (Vgs < m.Vth) return 0;
    double lambda_eff = ideal ? 0.0 : m.lambda;

    if (Vds < Vgs - m.Vth) { // Triode - region
        return m.tran_factor() * ((Vgs - m.Vth) * Vds - 0.5 * Vds * Vds);
    } else {                // Saturation - region
        return 0.5 * m.tran_factor() * pow(Vgs - m.Vth, 2) * (1 + lambda_eff * Vds);
    }
}

int main() {
    const Mosfet& m = MOSFET_2N7000;

    cout << "MOSFET analog circuit simul" << endl;
    cout << "If you want no enter the value, only push the button enter\n\n";

    double Vdd  = get_input("Vdd (default 5.0)", 5.0);
    double Vgs  = get_input("Vgs");
    double Vds  = get_input("Vds");
    double Rs   = get_input("Rs (Ohm)");
    double Rd   = get_input("Rd (Ohm)");
    double Vth  = get_input("Vth (default 2.0)", m.Vth);

    // store value map
    map<string, double> vars;

    vars["Vdd"] = Vdd;
    vars["Vgs"] = Vgs;
    vars["Vds"] = Vds;
    vars["Rs"] = Rs;
    vars["Rd"] = Rd;
    vars["Vth"] = Vth;

    double Id = -1, Vs = -1, Vd = -1;

    // Calculate Id
    if (Vgs > 0 && Vds > 0) {
        Id = calculate_Id(m, Vgs, Vds);
        vars["Id"] = Id;
    }

    // Calculate Vs
    if (Rs > 0 && Id > 0) {
        Vs = Id * Rs;
        vars["Vs"] = Vs;
    }

    // Calculate Vd
    if (Rd > 0 && Id > 0) {
        Vd = Vdd - Id * Rd;
        vars["Vd"] = Vd;
    }

    // Revere operate Rs
    if (Rs < 0 && Id > 0 && Vgs > 0) {
        Vs = vars["Vdd"] - Vgs; // assumption criticla Vg
        Rs = Vs / Id;
        vars["Rs (calc)"] = Rs;
        vars["Vs"] = Vs;
    }

    // using the graph to py -> save the file CSV type
    ofstream csv1("Vgs_vs_Id.csv");
    csv1 << "Vgs,Id\n";
    for (double v = 0.0; v <= 5.0; v += 0.1) {
        double id = calculate_Id(m, v, Vds > 0 ? Vds : 5.0);
        csv1 << v << "," << id << "\n";
    }
    csv1.close();

    ofstream csv2("Vds_vs_Id.csv");
    csv2 << "Vds,Id\n";
    for (double v = 0.0; v <= 5.0; v += 0.1) {
        double id = calculate_Id(m, Vgs > 0 ? Vgs : 2.5, v);
        csv2 << v << "," << id << "\n";
    }
    csv2.close();

    // summary output
    cout << "\n result of calculate summary\n";
    for (const auto& pair : vars) {
        cout << left << setw(10) << pair.first << " = " << pair.second << endl;
    }

    // 텍스트 파일 저장
    ofstream summary("summary.txt");
    summary << "MOSFET Circuit summary\n";
    for (const auto& pair : vars) {
        summary << pair.first << " = " << pair.second << "\n";
    }
    summary.close();

    cout << "\n[done] CSV save file : Vgs_vs_Id.csv, Vds_vs_Id.csv\n";
    cout << "[done] summary save : summary.txt\n";

    return 0;
}
