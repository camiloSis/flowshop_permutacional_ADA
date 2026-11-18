#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include "../include/fuerza_bruta.hpp"

using namespace std;

// Carga las instancias desde el archivo de texto formato P1
vector<vector<vector<int>>> cargar_instancias(const string& ruta) {
    ifstream archivo(ruta);
    vector<vector<vector<int>>> instancias;
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir " << ruta << endl;
        return instancias;
    }

    string linea;
    vector<int> numeros;
    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        stringstream ss(linea);
        int val;
        while (ss >> val) {
            numeros.push_back(val);
        }
    }

    size_t idx = 0;
    while (idx < numeros.size()) {
        int n = numeros[idx++];
        int m = numeros[idx++];
        vector<vector<int>> matriz(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                matriz[i][j] = numeros[idx++];
            }
        }
        instancias.push_back(matriz);
    }
    return instancias;
}

int main() {
    auto instancias = cargar_instancias("datos/ejemplos.txt");
    if (instancias.empty()) {
        cout << "No se encontraron instancias cargadas." << endl;
        return 1;
    }

    for (size_t k = 0; k < instancias.size(); ++k) {
        int n = instancias[k].size();
        int m = instancias[k][0].size();
        cout << "=== Instancia " << (k + 1) << " (" << n << " trabajos, " << m << " maquinas) ===" << endl;

        auto inicio = chrono::high_resolution_clock::now();
        auto [optimo, mejor_orden] = resolver_fuerza_bruta(instancias[k]);
        auto fin = chrono::high_resolution_clock::now();

        double tiempo_ms = chrono::duration<double, milli>(fin - inicio).count();

        cout << "Makespan optimo: " << optimo << endl;
        cout << "Orden optimo (base 0): ";
        for (int job : mejor_orden) cout << "J" << (job + 1) << " ";
        cout << "\nTiempo de ejecucion: " << tiempo_ms << " ms\n\n";
    }

    return 0;
}