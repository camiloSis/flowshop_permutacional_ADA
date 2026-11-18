#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../include/fuerza_bruta.hpp"
#include "../include/gantt.hpp"
#include "../include/johnson.hpp"
#include "../include/neh.hpp"

using namespace fs;

namespace {

// Lee una instancia ingresada manualmente.
Matriz leerInstanciaManual() {
    int n, m;
    std::cout << "Numero de trabajos: ";
    std::cin >> n;
    std::cout << "Numero de maquinas: ";
    std::cin >> m;

    if (!std::cin || n <= 0 || m <= 0 || n > 1000 || m > 100)
        throw std::invalid_argument("Dimensiones invalidas.");

    Matriz p(n, std::vector<int>(m));
    std::cout << "Ingrese una fila de tiempos por cada trabajo:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "T" << i + 1 << ": ";
        for (int j = 0; j < m; ++j) {
            std::cin >> p[i][j];
            if (!std::cin || p[i][j] < 0)
                throw std::invalid_argument("Use enteros no negativos.");
        }
    }
    validarInstancia(p);
    return p;
}

// Lee una o varias instancias: comentarios y números separados por espacios.
std::vector<Matriz> cargarInstancias(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open())
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta);

    std::vector<int> numeros;
    std::string linea;
    while (std::getline(archivo, linea)) {
        const std::size_t comentario = linea.find('#');
        if (comentario != std::string::npos)
            linea.erase(comentario);

        std::stringstream datos(linea);
        int valor;
        while (datos >> valor)
            numeros.push_back(valor);
    }

    std::vector<Matriz> instancias;
    std::size_t pos = 0;
    while (pos < numeros.size()) {
        if (pos + 2 > numeros.size())
            throw std::runtime_error("Archivo incompleto: faltan dimensiones.");

        const int n = numeros[pos++];
        const int m = numeros[pos++];
        if (n <= 0 || m <= 0)
            throw std::runtime_error("Dimensiones invalidas en el archivo.");

        const std::size_t cantidad = static_cast<std::size_t>(n) * static_cast<std::size_t>(m);
        if (pos + cantidad > numeros.size())
            throw std::runtime_error("Archivo incompleto: faltan tiempos.");

        Matriz p(n, std::vector<int>(m));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                p[i][j] = numeros[pos++];

        validarInstancia(p);
        instancias.push_back(std::move(p));
    }

    if (instancias.empty())
        throw std::runtime_error("El archivo no contiene instancias.");
    return instancias;
}

void mostrarOrden(const Permutacion& secuencia) {
    std::cout << "Secuencia: ";
    for (std::size_t i = 0; i < secuencia.size(); ++i) {
        if (i) std::cout << " -> ";
        std::cout << "T" << secuencia[i] + 1;
    }
    std::cout << '\n';
}

} // namespace

int main() {
    Matriz p;
    Permutacion ordenNEH;
    Permutacion ordenBruta;
    Permutacion ordenJohnson;
    bool cargada = false;

    while (true) {
        std::cout << "\n===== FLOW SHOP PERMUTACIONAL =====\n"
                  << "1. Ingresar instancia manual\n"
                  << "2. Cargar instancias desde archivo\n"
                  << "3. Ejecutar NEH\n"
                  << "4. Ejecutar fuerza bruta\n"
                  << "5. Ejecutar Johnson (solo 2 maquinas)\n"
                  << "6. Mostrar diagrama Gantt\n"
                  << "7. Ver resumen\n"
                  << "0. Salir\nOpcion: ";

        int opcion;
        if (!(std::cin >> opcion)) {
            std::cout << "Entrada no valida.\n";
            return 1;
        }

        try {
            if (opcion == 0)
                break;

            if (opcion == 1) {
                p = leerInstanciaManual();
                ordenNEH.clear();
                ordenBruta.clear();
                ordenJohnson.clear();
                cargada = true;
                std::cout << "Instancia cargada.\n";
            } else if (opcion == 2) {
                std::string ruta;
                std::cout << "Ruta del archivo [datos/ejemplos.txt]: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, ruta);
                if (ruta.empty()) ruta = "datos/ejemplos.txt";

                auto instancias = cargarInstancias(ruta);
                std::cout << "Se encontraron " << instancias.size() << " instancias.\n";
                std::cout << "Seleccione el numero de instancia (1-" << instancias.size() << "): ";

                std::size_t seleccion;
                std::cin >> seleccion;
                if (seleccion < 1 || seleccion > instancias.size())
                    throw std::invalid_argument("Seleccion fuera de rango.");

                p = std::move(instancias[seleccion - 1]);
                ordenNEH.clear();
                ordenBruta.clear();
                ordenJohnson.clear();
                cargada = true;
                std::cout << "Instancia " << seleccion << " cargada.\n";
            } else if (opcion == 3) {
                if (!cargada)
                    throw std::invalid_argument("Primero cargue una instancia.");

                const auto inicio = std::chrono::steady_clock::now();
                ordenNEH = resolverNEH(p);
                const auto fin = std::chrono::steady_clock::now();

                mostrarOrden(ordenNEH);
                std::cout << "Makespan NEH: " << makespan(p, ordenNEH) << '\n';
                std::cout << "Tiempo NEH: "
                          << std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count()
                          << " microsegundos\n";
            } else if (opcion == 4) {
                if (!cargada)
                    throw std::invalid_argument("Primero cargue una instancia.");
                if (p.size() > 11)
                    throw std::invalid_argument("Fuerza bruta limitada a 11 trabajos para evitar una ejecucion excesiva.");

                const auto inicio = std::chrono::steady_clock::now();
                auto resultado = resolverFuerzaBruta(p);
                const auto fin = std::chrono::steady_clock::now();

                ordenBruta = std::move(resultado.second);
                mostrarOrden(ordenBruta);
                std::cout << "Makespan optimo: " << resultado.first << '\n';
                std::cout << "Tiempo fuerza bruta: "
                          << std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count()
                          << " milisegundos\n";
            } else if (opcion == 5) {
                if (!cargada)
                    throw std::invalid_argument("Primero cargue una instancia.");

                ordenJohnson = johnson(p, true);
                mostrarOrden(ordenJohnson);
                std::cout << "Makespan Johnson: " << makespan(p, ordenJohnson) << '\n';
            } else if (opcion == 6) {
                if (!cargada)
                    throw std::invalid_argument("Primero cargue una instancia.");

                if (ordenNEH.empty() && ordenBruta.empty() && ordenJohnson.empty())
                    throw std::invalid_argument("Ejecute primero NEH, fuerza bruta o Johnson.");

                const Permutacion& secuencia = !ordenNEH.empty() ? ordenNEH
                                               : !ordenBruta.empty() ? ordenBruta
                                               : ordenJohnson;
                mostrarOrden(secuencia);
                imprimirGantt(p, secuencia);
            } else if (opcion == 7) {
                if (!cargada)
                    throw std::invalid_argument("No hay instancia cargada.");

                std::cout << "Trabajos: " << p.size()
                          << ", maquinas: " << p[0].size() << '\n';
                if (!ordenNEH.empty()) {
                    mostrarOrden(ordenNEH);
                    std::cout << "Makespan NEH: " << makespan(p, ordenNEH) << '\n';
                }
                if (!ordenBruta.empty()) {
                    mostrarOrden(ordenBruta);
                    std::cout << "Makespan optimo: " << makespan(p, ordenBruta) << '\n';
                }
                if (!ordenJohnson.empty()) {
                    mostrarOrden(ordenJohnson);
                    std::cout << "Makespan Johnson: " << makespan(p, ordenJohnson) << '\n';
                }
                if (ordenNEH.empty() && ordenBruta.empty() && ordenJohnson.empty())
                    std::cout << "Aun no se ejecuto ningun algoritmo.\n";
            } else {
                std::cout << "Opcion no valida.\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << '\n';
        }
    }

    std::cout << "Programa finalizado.\n";
    return 0;
}