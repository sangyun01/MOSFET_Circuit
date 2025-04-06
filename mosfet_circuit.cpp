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

double calculate_Id(const Mosfet& m, double Vgs, double Vds, bool ideal = false) {
    if (Vgs < m.Vth) return 0;
    double lambda_eff = ideal ? 0.0 : m.lambda;
    if (Vds < Vgs - m.Vth) {
        return m.tran_factor() * ((Vgs - m.Vth) * Vds - 0.5 * Vds * Vds);
    } else {
        return 0.5 * m.tran_factor() * pow(Vgs - m.Vth, 2) * (1 + lambda_eff * Vds);
    }
}

int main() {
    // MOSFET 선택
    cout << "MOSFET analog circuit simulator\n";
    cout << "Available MOSFET models:\n";
    for (const auto& pair : MOSFET_MODELS) {
        cout << " [" << pair.first << "] " << pair.second.name << endl;
    }

    cout << "\nChoose MOSFET index: ";
    int index = 0;
    cin >> index;
    cin.ignore();  // flush newline

    if (MOSFET_MODELS.find(index) == MOSFET_MODELS.end()) {
        cerr << "Invalid index. Exiting.\n";
        return 1;
    }

    const Mosfet& m = MOSFET_MODELS.at(index);
    cout << "Selected MOSFET: " << m.name << "\n\n";

    cout << "If you want to skip entering a value, just press Enter\n\n";

    double Vdd = get_input("Vdd (default 5.0)", 5.0);
    double Vgs = get_input("Vgs");
    double Vds = get_input("Vds");
    double Rs = get_input("Rs (Ohm)");
    double Rd = get_input("Rd (Ohm)");
    double Vth = get_input("Vth (default from model)", m.Vth);

    map<string, double> vars;
    vars["Vdd"] = Vdd;
    vars["Vgs"] = Vgs;
    vars["Vds (input)"] = Vds;
    vars["Rs"] = Rs;
    vars["Rd"] = Rd;
    vars["Vth"] = Vth;

    double Id = -1, Vs = -1, Vd = -1;

    // Vds 예측
    if (Vds <= 0 && Vgs > 0 && Rd > 0) {
        double Id_guess = 0.5 * m.tran_factor() * pow(Vgs - m.Vth, 2);
        double Vs_guess = (Rs > 0) ? Id_guess * Rs : 0;
        double Vd_guess = Vdd - Id_guess * Rd;
        Vds = Vd_guess - Vs_guess;
        vars["Vds (est)"] = Vds;

        Id = calculate_Id(m, Vgs, Vds);
        vars["Id"] = Id;

        if (Rs > 0) {
            Vs = Id * Rs;
            vars["Vs"] = Vs;
        }
        if (Rd > 0) {
            Vd = Vdd - Id * Rd;
            vars["Vd"] = Vd;
        }

        if (Vds < Vgs - m.Vth) {
            cout << "[Warning] Vds < Vgs - Vth → not in saturation region!\n";
        }
    }
    else if (Vgs > 0 && Vds > 0) {
        Id = calculate_Id(m, Vgs, Vds);
        vars["Id"] = Id;

        if (Rs > 0) {
            Vs = Id * Rs;
            vars["Vs"] = Vs;
        }
        if (Rd > 0) {
            Vd = Vdd - Id * Rd;
            vars["Vd"] = Vd;
        }
    }

    if (Rs < 0 && Id > 0 && Vgs > 0) {
        Vs = Vdd - Vgs; // assume Vg = Vdd
        Rs = Vs / Id;
        vars["Rs (calc)"] = Rs;
        vars["Vs"] = Vs;
    }

    if (Id > 0 && Vgs > 0 && Vth > 0) {
        double gm = 2 * Id / (Vgs - Vth);
        double ro = 1 / (m.lambda * Id);
        double Rout = (Rd > 0) ? (ro * Rd) / (ro + Rd) : ro;
        double Av = -gm * Rout;

        vars["gm"] = gm;
        vars["ro"] = ro;
        vars["Rout"] = Rout;
        vars["Av"] = Av;
    }

    // CSV 저장
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

    // 결과 출력
    cout << "\n[Calculation Result Summary]\n";
    for (const auto& pair : vars) {
        cout << left << setw(15) << pair.first << " = " << pair.second << endl;
    }

    ofstream summary("summary.txt");
    summary << "MOSFET Circuit Summary for " << m.name << "\n";
    for (const auto& pair : vars) {
        summary << pair.first << " = " << pair.second << "\n";
    }
    summary.close();

    cout << "\n[done] CSV files saved: Vgs_vs_Id.csv, Vds_vs_Id.csv\n";
    cout << "[done] Summary saved: summary.txt\n";

    return 0;
}