#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs {

// Matriz de tiempos por trabajo y máquina; permutación con índices base cero.
using Matriz = std::vector<std::vector<int>>;
using Permutacion = std::vector<int>;

// Valida dimensiones y tiempos antes de calcular una solución.
inline void validarInstancia(const Matriz& p) {
    if (p.empty() || p[0].empty())
        throw std::invalid_argument("Instancia vacia.");

    const std::size_t m = p[0].size();
    for (const auto& fila : p) {
        if (fila.size() != m)
            throw std::invalid_argument("Filas con distinto numero de maquinas.");
        for (int tiempo : fila)
            if (tiempo < 0)
                throw std::invalid_argument("Tiempos negativos.");
    }
}

inline std::vector<std::vector<int>> tablaC(const Matriz& p, const Permutacion& perm,
                                            bool imprimir = false) {
    validarInstancia(p);

    const int n = static_cast<int>(perm.size());
    const int m = static_cast<int>(p[0].size());
    std::vector<std::vector<int>> C(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        const int t = perm[i - 1];
        if (t < 0 || t >= static_cast<int>(p.size()))
            throw std::out_of_range("Trabajo fuera de rango.");

        for (int j = 1; j <= m; ++j) {
            const int anteriorMaquina = C[i - 1][j];
            const int anteriorTrabajo = C[i][j - 1];
            C[i][j] = std::max(anteriorMaquina, anteriorTrabajo) + p[t][j - 1];

            if (imprimir)
                std::cout << "C[" << i << "][" << j << "] (J" << t + 1 << ", M" << j
                          << ") = max(" << anteriorMaquina << ", " << anteriorTrabajo
                          << ") + " << p[t][j - 1] << " = " << C[i][j] << "\n";
        }
    }
    return C;
}

// Imprime la tabla C con filas de trabajos y columnas de máquinas.
inline void imprimirTablaC(const std::vector<std::vector<int>>& C,
                           const Permutacion& perm) {
    const int n = static_cast<int>(perm.size());
    const int m = static_cast<int>(C[0].size()) - 1;

    std::cout << std::setw(8) << "Trabajo";
    for (int j = 1; j <= m; ++j)
        std::cout << std::setw(6) << ("M" + std::to_string(j));
    std::cout << "\n";

    for (int i = 1; i <= n; ++i) {
        std::cout << std::setw(8) << ("J" + std::to_string(perm[i - 1] + 1));
        for (int j = 1; j <= m; ++j)
            std::cout << std::setw(6) << C[i][j];
        std::cout << "\n";
    }
}

// Función única usada por fuerza bruta, Johnson, NEH y Gantt.
inline int makespan(const Matriz& p, const Permutacion& perm) {
    validarInstancia(p);

    const int m = static_cast<int>(p[0].size());
    std::vector<int> fila(m + 1, 0);

    for (int t : perm) {
        if (t < 0 || t >= static_cast<int>(p.size()))
            throw std::out_of_range("Trabajo fuera de rango.");
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