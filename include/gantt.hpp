#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

#include "makespan.hpp"

namespace fs {

// Imprime cada operación como intervalo [inicio, fin) en cada máquina.
inline void imprimirGantt(const Matriz& p, const Permutacion& orden,
                          std::ostream& out = std::cout) {
    validarInstancia(p);

    std::vector<long long> finMaquina(p[0].size(), 0);
    out << "\nGANTT TEXTUAL (intervalos [inicio, fin))\n";
    out << std::left << std::setw(10) << "Maquina" << std::setw(10) << "Trabajo"
        << std::setw(12) << "Inicio" << std::setw(12) << "Fin" << "Duracion\n";
    out << std::string(54, '-') << '\n';

    for (int t : orden) {
        if (t < 0 || t >= static_cast<int>(p.size()))
            throw std::out_of_range("Trabajo fuera de rango.");

        long long finAnterior = 0;
        for (std::size_t j = 0; j < p[0].size(); ++j) {
            const long long inicio = std::max(finMaquina[j], finAnterior);
            const long long fin = inicio + p[t][j];
            out << std::left << std::setw(10) << ("M" + std::to_string(j + 1))
                << std::setw(10) << ("T" + std::to_string(t + 1))
                << std::setw(12) << inicio << std::setw(12) << fin << p[t][j] << '\n';
            finMaquina[j] = fin;
            finAnterior = fin;
        }
    }

    out << "Makespan: " << makespan(p, orden) << '\n';
}

} // namespace fs
