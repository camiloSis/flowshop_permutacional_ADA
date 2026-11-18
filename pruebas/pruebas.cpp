#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>

#include "../include/fuerza_bruta.hpp"

using namespace std;

// Carga instancias en el formato n m seguido de los tiempos por trabajo.
vector<fs::Matriz> cargarInstancias(const string& ruta) {
    ifstream archivo(ruta);
    vector<fs::Matriz> instancias;
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir " << ruta << '\n';
        return instancias;
    }

    string linea;
    vector<int> numeros;
    while (getline(archivo, linea)) {
        const size_t comentario = linea.find('#');
        if (comentario != string::npos) linea.erase(comentario);

        stringstream ss(linea);
        int valor;
        while (ss >> valor) numeros.push_back(valor);
    }

    size_t pos = 0;
    while (pos < numeros.size()) {
        const int n = numeros[pos++];
        const int m = numeros[pos++];
        fs::Matriz matriz(n, vector<int>(m));

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                matriz[i][j] = numeros[pos++];

        instancias.push_back(std::move(matriz));
    }
    return instancias;
}

int main() {
    auto instancias = cargarInstancias("datos/ejemplos.txt");
    if (instancias.empty()) {
        cout << "No se encontraron instancias cargadas.\n";
        return 1;
    }

    for (size_t k = 0; k < instancias.size(); ++k) {
        const int n = static_cast<int>(instancias[k].size());
        const int m = static_cast<int>(instancias[k][0].size());
        cout << "=== Instancia " << k + 1 << " (" << n << " trabajos, "
             << m << " maquinas) ===\n";

        const auto inicio = chrono::high_resolution_clock::now();
        auto resultado = fs::resolverFuerzaBruta(instancias[k]);
        const auto fin = chrono::high_resolution_clock::now();

        cout << "Makespan optimo: " << resultado.first << '\n';
        cout << "Orden optimo (base 0): ";
        for (int trabajo : resultado.second) cout << "J" << trabajo + 1 << ' ';

        const double tiempoMs = chrono::duration<double, milli>(fin - inicio).count();
        cout << "\nTiempo de ejecucion: " << tiempoMs << " ms\n\n";
    }
    return 0;
}