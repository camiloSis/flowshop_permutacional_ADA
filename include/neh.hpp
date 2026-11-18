#ifndef FLOWSHOP_NEH_HPP
#define FLOWSHOP_NEH_HPP
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

namespace flowshop {
using MatrizTiempos = std::vector<std::vector<int>>;
using Secuencia = std::vector<int>;

inline void validarInstancia(const MatrizTiempos& p) {
    if (p.empty() || p[0].empty()) throw std::invalid_argument("Instancia vacia.");
    const auto m = p[0].size();
    for (const auto& fila : p) {
        if (fila.size() != m) throw std::invalid_argument("Filas con distinto numero de maquinas.");
        for (int x : fila) if (x < 0) throw std::invalid_argument("Tiempos negativos.");
    }
}

// Makespan para una secuencia; trabajos identificados con indices 0..n-1.
inline long long makespanNEH(const MatrizTiempos& p, const Secuencia& orden) {
    validarInstancia(p);
    std::vector<long long> fin(p[0].size(), 0);
    for (int t : orden) {
        if (t < 0 || static_cast<std::size_t>(t) >= p.size())
            throw std::out_of_range("Trabajo fuera de rango.");
        fin[0] += p[t][0];
        for (std::size_t j = 1; j < fin.size(); ++j)
            fin[j] = std::max(fin[j], fin[j-1]) + p[t][j];
    }
    return fin.back();
}

// NEH: ordenar por suma descendente e insertar en la posicion de menor makespan.
inline Secuencia resolverNEH(const MatrizTiempos& p) {
    validarInstancia(p);
    const int n = static_cast<int>(p.size());
    std::vector<int> trabajos(n);
    std::iota(trabajos.begin(), trabajos.end(), 0);
    std::vector<long long> suma(n, 0);
    for (int i = 0; i < n; ++i)
        for (int x : p[i]) suma[i] += x;

    std::stable_sort(trabajos.begin(), trabajos.end(),
        [&](int a, int b) { return suma[a] > suma[b]; });

    Secuencia orden;
    for (int t : trabajos) {
        long long mejor = -1;
        Secuencia mejorOrden;
        for (std::size_t pos = 0; pos <= orden.size(); ++pos) {
            Secuencia candidata = orden;
            candidata.insert(candidata.begin() + static_cast<std::ptrdiff_t>(pos), t);
            const long long valor = makespanNEH(p, candidata);
            if (mejor == -1 || valor < mejor) {
                mejor = valor;
                mejorOrden = std::move(candidata);
            }
        }
        orden = std::move(mejorOrden);
    }
    return orden;
}
}
#endif