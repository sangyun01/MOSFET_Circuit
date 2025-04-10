#pragma once

#include <string>
#include <map>

namespace MosfetLib {

struct Mosfet {
    std::string name;
    double Vth;        
    double Kn;         
    double lambda;     
    double W;          
    double L;          

    double tran_factor() const {
        return Kn * (W / L);
    }
};

const Mosfet MOSFET_2N7000 = {
    "2N7000",
    2.0,
    20e-6,
    0.02,
    1.0,
    1.0
};

const Mosfet MOSFET_HV_FAST = {
    "HV_FAST",
    0.9,
    150e-6,
    0.01,
    2.0,
    0.5
};

// ✅ 인덱스로 접근할 수 있도록 map 생성
const std::map<int, Mosfet> MOSFET_MODELS = {
    {0, MOSFET_2N7000},
    {1, MOSFET_HV_FAST}
};

}  // namespace MosfetLib
