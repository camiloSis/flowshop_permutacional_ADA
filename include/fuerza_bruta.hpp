#ifndef FUERZA_BRUTA_HPP
#define FUERZA_BRUTA_HPP

#include <algorithm>
#include <limits>
#include <numeric>
#include <utility>

#include "makespan.hpp"

namespace fs {

// Prueba todas las permutaciones y devuelve el mejor makespan y su secuencia.
inline std::pair<int, Permutacion> resolverFuerzaBruta(const Matriz& tiempos) {
    validarInstancia(tiempos);

    const int n = static_cast<int>(tiempos.size());
    Permutacion orden(n);
    std::iota(orden.begin(), orden.end(), 0);

    int mejorMakespan = std::numeric_limits<int>::max();
    Permutacion mejorOrden = orden;

    do {
        const int actual = makespan(tiempos, orden);
        if (actual < mejorMakespan) {
            mejorMakespan = actual;
            mejorOrden = orden;
        }
    } while (std::next_permutation(orden.begin(), orden.end()));

    return {mejorMakespan, mejorOrden};
}

}

#endif // FUERZA_BRUTA_HPP