#include "../include/neh.hpp"
#include "../include/gantt.hpp"
#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;
using namespace flowshop;

MatrizTiempos leerInstancia() {
    int n, m;
    cout << "Numero de trabajos: "; cin >> n;
    cout << "Numero de maquinas: "; cin >> m;
    if (!cin || n <= 0 || m <= 0 || n > 1000 || m > 100) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Dimensiones invalidas.");
    }
    MatrizTiempos p(n, vector<int>(m));
    cout << "Ingrese una fila de tiempos por cada trabajo:\n";
    for (int i=0; i<n; ++i) {
        cout << "T" << i+1 << ": ";
        for (int j=0; j<m; ++j) {
            cin >> p[i][j];
            if (!cin || p[i][j] < 0) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Use enteros no negativos.");
            }
        }
    }
    return p;
}
void mostrarOrden(const Secuencia& s) {
    cout << "Secuencia: ";
    for (size_t i=0; i<s.size(); ++i) {
        if (i) cout << " -> ";
        cout << "T" << s[i]+1;
    }
    cout << '\n';
}
int main() {
    MatrizTiempos p;
    Secuencia orden;
    bool cargada = false;
    while (true) {
        cout << "\n===== FLOW SHOP PERMUTACIONAL =====\n"
             << "1. Ingresar instancia\n2. Ejecutar NEH\n"
             << "3. Mostrar diagrama Gantt\n4. Ver resumen\n0. Salir\nOpcion: ";
        int op; if (!(cin >> op)) { cout << "Entrada no valida.\n"; return 1; }
        try {
            if (op == 0) break;
            if (op == 1) { p=leerInstancia(); orden.clear(); cargada=true; cout<<"Instancia cargada.\n"; }
            else if (op == 2) {
                if (!cargada) { cout<<"Primero ingrese una instancia.\n"; continue; }
                auto a=chrono::steady_clock::now();
                orden=resolverNEH(p);
                auto b=chrono::steady_clock::now();
                auto us=chrono::duration_cast<chrono::microseconds>(b-a).count();
                mostrarOrden(orden);
                cout<<"Makespan NEH: "<<makespanNEH(p,orden)<<'\n';
                cout<<"Tiempo NEH: "<<us<<" microsegundos\n";
            } else if (op == 3) {
                if (!cargada || orden.empty()) { cout<<"Ejecute NEH primero.\n"; continue; }
                imprimirGantt(p,orden);
            } else if (op == 4) {
                if (!cargada) { cout<<"No hay instancia cargada.\n"; continue; }
                cout<<"Trabajos: "<<p.size()<<", maquinas: "<<p[0].size()<<'\n';
                if (!orden.empty()) { mostrarOrden(orden); cout<<"Makespan: "<<makespanNEH(p,orden)<<'\n'; }
                else cout<<"NEH aun no se ejecuto.\n";
            } else cout<<"Opcion no valida.\n";
        } catch (const exception& e) { cout<<"Error: "<<e.what()<<'\n'; }
    }
    cout<<"Programa finalizado.\n";
}