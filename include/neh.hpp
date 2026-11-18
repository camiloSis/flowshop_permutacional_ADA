#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

#include "makespan.hpp"

namespace fs {

// NEH: ordena trabajos por carga total y los inserta donde minimizan el makespan.
inline Permutacion resolverNEH(const Matriz& p) {
    validarInstancia(p);

    const int n = static_cast<int>(p.size());
    Permutacion trabajos(n);
    std::iota(trabajos.begin(), trabajos.end(), 0);

    std::vector<long long> carga(n, 0);
    for (int i = 0; i < n; ++i)
        for (int tiempo : p[i]) carga[i] += tiempo;

    std::stable_sort(trabajos.begin(), trabajos.end(),
                     [&](int a, int b) { return carga[a] > carga[b]; });

    Permutacion orden;
    for (int trabajo : trabajos) {
        int mejorMakespan = -1;
        Permutacion mejorOrden;

        for (std::size_t pos = 0; pos <= orden.size(); ++pos) {
            Permutacion candidata = orden;
            candidata.push_back(trabajo);
            for (std::size_t i = candidata.size() - 1; i > pos; --i)
                candidata[i] = candidata[i - 1];
            candidata[pos] = trabajo;

            const int valor = makespan(p, candidata);

            if (mejorMakespan == -1 || valor < mejorMakespan) {
                mejorMakespan = valor;
                mejorOrden = std::move(candidata);
            }
        }
        orden = std::move(mejorOrden);
    }
    return orden;
}

}