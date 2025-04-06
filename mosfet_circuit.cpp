// mosfet_simulator.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include "mosfet_models.hpp"

using namespace MosfetLib;
using namespace std;

double calculate_Id(const Mosfet& m, double Vgs, double Vds, bool ideal = false) {
    if (Vgs < m.Vth) return 0;
    double lambda_eff = ideal ? 0.0 : m.lambda;

    if (Vds < Vgs - m.Vth) {
        return m.tran_factor() * ((Vgs - m.Vth) * Vds - 0.5 * Vds * Vds);  // triode
    } else {
        return 0.5 * m.tran_factor() * (Vgs - m.Vth) * (Vgs - m.Vth) * (1 + lambda_eff * Vds);  // saturation
    }
}

int main() {
    vector<Mosfet> mosfets = { MOSFET_2N7000 };

    const Mosfet& m = mosfets[0];
    double Vds = 5.0;
    double Vgs_start = 0.0, Vgs_end = 5.0, step = 0.1;

    ofstream file("Id_vs_Vgs.csv");
    file << "Vgs,Id_real,Id_ideal\n";

    for (double Vgs = Vgs_start; Vgs <= Vgs_end; Vgs += step) {
        double Id_real = calculate_Id(m, Vgs, Vds, false);
        double Id_ideal = calculate_Id(m, Vgs, Vds, true);
        file << Vgs << "," << Id_real << "," << Id_ideal << "\n";
    }

    file.close();
    cout << "CSV 저장 완료: Id_vs_Vgs.csv\n";

    return 0;
}
