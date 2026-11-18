#ifndef FUERZA_BRUTA_HPP
#define FUERZA_BRUTA_HPP

#include <vector>
#include <numeric>
#include <algorithm>
#include <climits>
#include <utility>

// Función para calcular el makespan de un orden dado (0..n-1)
inline int calcular_makespan_fb(const std::vector<std::vector<int>>& tiempos, const std::vector<int>& orden) {
    int n = orden.size();
    if (n == 0) return 0;
    int m = tiempos[0].size();
    
    std::vector<std::vector<int>> C(n, std::vector<int>(m, 0));
    
    for (int i = 0; i < n; ++i) {
        int trabajo = orden[i];
        for (int j = 0; j < m; ++j) {
            int arriba = (i > 0) ? C[i - 1][j] : 0;
            int izquierda = (j > 0) ? C[i][j - 1] : 0;
            C[i][j] = std::max(arriba, izquierda) + tiempos[trabajo][j];
        }
    }
    return C[n - 1][m - 1];
}

// Resuelve el Flow Shop por Fuerza Bruta probando todas las permutaciones (n!)
inline std::pair<int, std::vector<int>> resolver_fuerza_bruta(const std::vector<std::vector<int>>& tiempos) {
    int n = tiempos.size();
    std::vector<int> orden(n);
    std::iota(orden.begin(), orden.end(), 0); // Rellena con [0, 1, 2, ..., n-1]
    
    int mejor_makespan = INT_MAX;
    std::vector<int> mejor_orden = orden;
    
    do {
        int actual = calcular_makespan_fb(tiempos, orden);
        if (actual < mejor_makespan) {
            mejor_makespan = actual;
            mejor_orden = orden;
        }
    } while (std::next_permutation(orden.begin(), orden.end()));
    
    return {mejor_makespan, mejor_orden};
}

#endif // FUERZA_BRUTA_HPP