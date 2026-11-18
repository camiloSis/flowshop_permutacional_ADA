#pragma once
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "makespan.hpp"

namespace fs {

inline Permutacion johnson(const Matriz& p, bool imprimir = false) {
    if (p.empty() || p[0].size() != 2)
        throw std::invalid_argument("Johnson solo aplica a exactamente 2 maquinas");

    const int n = static_cast<int>(p.size());
    Permutacion S1, S2;
    for (int t = 0; t < n; ++t) (p[t][0] <= p[t][1] ? S1 : S2).push_back(t);  // O(n)

    std::sort(S1.begin(), S1.end(), [&](int a, int b) {
        return p[a][0] != p[b][0] ? p[a][0] < p[b][0] : a < b;
    });
    std::sort(S2.begin(), S2.end(), [&](int a, int b) {
        return p[a][1] != p[b][1] ? p[a][1] > p[b][1] : a < b;
    });

    if (imprimir) {
        auto mostrar = [](const char* nombre, const Permutacion& S) {
            std::cout << nombre << " = { ";
            for (int t : S) std::cout << "J" << t + 1 << " ";
            std::cout << "}\n";
        };
        mostrar("S1 (p1 <= p2, p1 creciente)  ", S1);
        mostrar("S2 (p1 >  p2, p2 decreciente)", S2);
    }

    Permutacion seq = S1;
    seq.insert(seq.end(), S2.begin(), S2.end());

    if (imprimir) {
        std::cout << "Secuencia de Johnson: ";
        for (int t : seq) std::cout << "J" << t + 1 << " ";
        std::cout << "\n";
    }
    return seq;
}

} 