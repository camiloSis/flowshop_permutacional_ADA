#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace fs {

// p[t][j] = tiempo de procesamiento del trabajo t en la maquina j 
using Matriz = std::vector<std::vector<int>>;
using Permutacion = std::vector<int>; 
inline std::vector<std::vector<int>> tablaC(const Matriz& p, const Permutacion& perm,
                                            bool imprimir = false) {
    const int n = static_cast<int>(perm.size());
    const int m = static_cast<int>(p[0].size());
    std::vector<std::vector<int>> C(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        const int t = perm[i - 1];
        for (int j = 1; j <= m; ++j) {
            const int arriba = C[i - 1][j];    // la maquina j termina el trabajo anterior
            const int izq = C[i][j - 1];       // este trabajo termina en la maquina j-1
            C[i][j] = std::max(arriba, izq) + p[t][j - 1];
            if (imprimir) {
                std::cout << "C[" << i << "][" << j << "] (J" << t + 1 << ", M" << j
                          << ") = max(" << arriba << ", " << izq << ") + " << p[t][j - 1]
                          << " = " << C[i][j] << "\n";
            }
        }
    }
    return C;
}

// Imprime la tabla C con filas = trabajos en orden, columnas = maquinas
inline void imprimirTablaC(const std::vector<std::vector<int>>& C, const Permutacion& perm) {
    const int n = static_cast<int>(perm.size());
    const int m = static_cast<int>(C[0].size()) - 1;
    std::cout << std::setw(8) << "Trabajo";
    for (int j = 1; j <= m; ++j) std::cout << std::setw(6) << ("M" + std::to_string(j));
    std::cout << "\n";
    for (int i = 1; i <= n; ++i) {
        std::cout << std::setw(8) << ("J" + std::to_string(perm[i - 1] + 1));
        for (int j = 1; j <= m; ++j) std::cout << std::setw(6) << C[i][j];
        std::cout << "\n";
    }
}

inline int makespan(const Matriz& p, const Permutacion& perm) {
    const int m = static_cast<int>(p[0].size());
    std::vector<int> fila(m + 1, 0);  // fila[j] = C[i-1][j], se actualiza a C[i][j]
    for (int t : perm) {
        for (int j = 1; j <= m; ++j)
            fila[j] = std::max(fila[j], fila[j - 1]) + p[t][j - 1];
    }
    return fila[m];
}

inline int makespanDetallado(const Matriz& p, const Permutacion& perm) {
    auto C = tablaC(p, perm, true);
    std::cout << "\n";
    imprimirTablaC(C, perm);
    const int ms = C[perm.size()][C[0].size() - 1];
    std::cout << "\nMakespan (Cmax) = " << ms << "\n";
    return ms;
}

} 