# Optimización de Secuenciación en Líneas de Producción
## Flow Shop Permutacional ($F_m \mid \text{prmu} \mid C_{\max}$)

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Build](https://img.shields.io/badge/build-clang%2B%2B%20%7C%20g%2B%2B-brightgreen.svg)
![Licencia](https://img.shields.io/badge/uso-acad%C3%A9mico-lightgrey.svg)

Implementación modular en **C++17** de algoritmos **exactos** y **heurísticos** para resolver el *Permutation Flow Shop Scheduling Problem* (PFSP), cuyo objetivo es minimizar el tiempo total de finalización de todos los trabajos, conocido como *makespan* ($C_{\max}$).

> Proyecto desarrollado para el curso de **Análisis y Diseño de Algoritmos**, Escuela Profesional de Ingeniería de Sistemas, **Universidad Nacional de San Agustín de Arequipa (UNSA)**.

---

## Tabla de contenidos

1. [Descripción del problema](#1-descripción-del-problema)
2. [Algoritmos implementados](#2-algoritmos-implementados)
3. [Estructura del repositorio](#3-estructura-del-repositorio)
4. [Requisitos y compilación](#4-requisitos-y-compilación)
5. [Uso del menú interactivo](#5-uso-del-menú-interactivo)
6. [Formato de los archivos de datos](#6-formato-de-los-archivos-de-datos)
7. [Ejemplo de uso](#7-ejemplo-de-uso)
8. [Limitaciones](#8-limitaciones)
9. [Equipo de desarrollo](#9-equipo-de-desarrollo)
10. [Referencias bibliográficas](#10-referencias-bibliográficas)

---

## 1. Descripción del problema

En un sistema productivo de tipo **Flow Shop Permutacional** se dispone de un conjunto de $n$ trabajos que deben procesarse en $m$ máquinas ($M_1, M_2, \ldots, M_m$).

- Todos los trabajos siguen la **misma ruta**: pasan por las máquinas en el mismo orden.
- Se mantiene una **permutación invariable**: el orden de los trabajos es el mismo en todas las máquinas.

$$\pi = (\pi(1), \pi(2), \ldots, \pi(n)) \in \Pi_n$$

**Objetivo:** encontrar la permutación $\pi$ que minimice el *makespan*, es decir, el instante en que termina el último trabajo en la última máquina:

$$C_{\max}(\pi) = C_{n,m}$$

### Relación de recurrencia

$$C_{i,k} = \max\left\{ C_{i-1,k},\ C_{i,k-1} \right\} + p_{\pi(i),k}$$

con $C_{0,k} = 0$ y $C_{i,0} = 0$.

| Símbolo | Significado |
| ------- | ----------- |
| $C_{i,k}$ | Tiempo de finalización del trabajo en la posición $i$ en la máquina $k$ |
| $p_{\pi(i),k}$ | Tiempo de procesamiento del trabajo $\pi(i)$ en la máquina $k$ |
| $C_{i-1,k}$ | Finalización del trabajo anterior en la misma máquina |
| $C_{i,k-1}$ | Finalización del mismo trabajo en la máquina anterior |

El espacio de búsqueda tiene tamaño $n!$, por lo que el problema es **NP-difícil** para $m \geq 3$.

---

## 2. Algoritmos implementados

Todos los módulos están bajo el espacio de nombres `namespace flowshop`, con una arquitectura *header-only* modular y reutilizable.

| Algoritmo / Módulo | Archivo | Alcance | Complejidad temporal | Garantía |
| ------------------ | ------- | ------- | -------------------- | -------- |
| **Cálculo de Makespan** | `include/makespan.hpp` | Evaluación de secuencias | $\Theta(n \cdot m)$ | Exacto; espacio auxiliar $\mathcal{O}(m)$ |
| **Fuerza Bruta** | `include/fuerza_bruta.hpp` | General ($m \geq 1$) | $\Theta(n! \cdot n \cdot m)$ | Óptimo global (práctico hasta $n \leq 12$) |
| **Regla de Johnson** | `include/johnson.hpp` | Dos máquinas ($m = 2$) | $\Theta(n \log n)$ | Óptimo global |
| **Heurística NEH** | `include/neh.hpp` | General ($m \geq 3$) | $\mathcal{O}(n^3 \cdot m)$ | Heurística de alta calidad (sin garantía de optimalidad) |
| **Diagrama de Gantt** | `include/gantt.hpp` | Visualización | $\Theta(n \cdot m)$ | Auditoría de intervalos de procesamiento |

### 2.1. Cálculo del Makespan
Evalúa una secuencia con la recurrencia del PFSP. Mantiene solo una fila de tiempos de finalización, logrando espacio auxiliar $\mathcal{O}(m)$. Es la función de evaluación base de los demás algoritmos.

### 2.2. Fuerza Bruta
Genera todas las permutaciones con `std::next_permutation`, calcula el *makespan* de cada una y conserva la mejor. Garantiza el óptimo global, pero su costo factorial limita su uso a instancias pequeñas.

### 2.3. Regla de Johnson
Resuelve de forma óptima el caso $m = 2$:

1. Se separan los trabajos en $U = \{j : p_{j,1} \le p_{j,2}\}$ y $V = \{j : p_{j,1} > p_{j,2}\}$.
2. $U$ se ordena de forma **creciente** por $p_{j,1}$.
3. $V$ se ordena de forma **decreciente** por $p_{j,2}$.
4. La secuencia final es $U$ seguido de $V$.

### 2.4. Heurística NEH (Nawaz, Enscore y Ham)
Heurística constructiva para $m \geq 3$:

1. Ordena los trabajos por tiempo total de procesamiento, de mayor a menor.
2. Toma los dos primeros y conserva el mejor orden parcial.
3. Para cada trabajo restante, lo prueba en **todas las posiciones** de la secuencia parcial y lo inserta donde el *makespan* sea menor.

### 2.5. Diagrama de Gantt
Genera una representación tabular del cronograma, con los intervalos de inicio y fin de cada trabajo por máquina, útil para verificar la secuencia obtenida.

---

## 3. Estructura del repositorio

```text
flowshop_permutacional_ADA/
│
├── datos/
│   ├── ejemplos.txt              # Instancias base de validación (4x2, 4x3, 9x3)
│   ├── fuerza_bruta_escala.txt   # Escalabilidad de Fuerza Bruta
│   ├── johnson_escala.txt        # Instancias para Johnson
│   └── neh_escala.txt            # Instancias grandes para NEH
│
├── gui/                          # Interfaz gráfica opcional (Qt/CMake)
│
├── include/
│   ├── fuerza_bruta.hpp
│   ├── gantt.hpp
│   ├── johnson.hpp
│   ├── makespan.hpp
│   └── neh.hpp
│
├── pruebas/
│   └── pruebas.cpp               # Pruebas unitarias y medición de tiempos
│
├── src/
│   └── main.cpp                  # Programa principal y menú de consola
│
└── README.md
```

---

## 4. Requisitos y compilación

### 4.1. Requisitos

- Compilador compatible con **C++17**: Clang++ 5+ o GCC 7+.
- Terminal (Linux, macOS o Windows con MinGW/WSL).

### 4.2. Programa principal

```bash
# Con Clang
clang++ -std=c++17 -O2 -Iinclude src/main.cpp -o flowshop

# Con GCC
g++ -std=c++17 -O2 -Iinclude src/main.cpp -o flowshop

# Ejecutar
./flowshop
```

### 4.3. Pruebas unitarias

```bash
clang++ -std=c++17 -O2 -Iinclude pruebas/pruebas.cpp -o pruebas_flowshop
./pruebas_flowshop
```

---

## 5. Uso del menú interactivo

```text
===== FLOW SHOP PERMUTACIONAL =====

1. Ingresar instancia manual
2. Cargar instancias desde archivo
3. Ejecutar NEH
4. Ejecutar fuerza bruta
5. Ejecutar Johnson (solo 2 maquinas)
6. Mostrar diagrama Gantt
7. Ver resumen
0. Salir

Opcion:
```

| Opción | Descripción |
| ------ | ----------- |
| **1** | Ingresa una matriz de tiempos $n \times m$ manualmente. |
| **2** | Carga instancias desde la carpeta `datos/`. |
| **3** | Ejecuta NEH y muestra la secuencia y su *makespan*. |
| **4** | Ejecuta Fuerza Bruta (evalúa las $n!$ permutaciones). |
| **5** | Ejecuta Johnson (solo si la instancia tiene 2 máquinas). |
| **6** | Muestra el diagrama de Gantt de la última secuencia obtenida. |
| **7** | Muestra un cuadro comparativo: secuencias, *makespan* y tiempos. |
| **0** | Sale del programa. |

### Medición del rendimiento
Se usa `std::chrono` para medir los tiempos de ejecución (en milisegundos), lo que permite comparar experimentalmente métodos exactos y heurísticos según el tamaño de la instancia.

---

## 6. Formato de los archivos de datos

Cada instancia indica el número de trabajos $n$, el número de máquinas $m$ y la matriz de tiempos de procesamiento (una fila por trabajo, una columna por máquina).

```text
4 2
3 6
5 2
1 2
6 6
```

> Ajusta este ejemplo si tu formato de `datos/*.txt` difiere (por ejemplo, si incluye nombres o separadores de instancias).

---

## 7. Ejemplo de uso

Instancia de 4 trabajos y 2 máquinas (la de la sección anterior):

```text
Opcion: 5
Secuencia Johnson: J3 - J1 - J4 - J2
Makespan: 19
Tiempo: 0.01 ms
```

Para $m = 2$, Johnson y Fuerza Bruta deben coincidir en el *makespan*; para $m \geq 3$, NEH suele quedar muy cerca del óptimo de Fuerza Bruta en una fracción del tiempo.

> Los valores mostrados son ilustrativos: reemplázalos por la salida real de tu programa.

---

## 8. Limitaciones

- **Fuerza Bruta** es impracticable para $n > 12$ debido a su crecimiento factorial $n!$.
- **Johnson** solo es aplicable a $m = 2$ (y a casos especiales de $m = 3$ que no están implementados).
- **NEH** no garantiza el óptimo; es una heurística constructiva.
- Solo se considera el caso **permutacional** (mismo orden en todas las máquinas), sin *buffers* ni tiempos de preparación.

---

## 9. Equipo de desarrollo

| Integrante | Responsabilidades |
| ---------- | ----------------- |
| **Alvarez Molina, Karen Alexandra** | Modelado matemático, análisis de complejidad combinatoria, módulo de Fuerza Bruta (`fuerza_bruta.hpp`) y banco de pruebas. |
| **Capatinta Almanza, Camilo Bladimir Alexander** | Cálculo optimizado del Makespan (`makespan.hpp`) y Regla de Johnson (`johnson.hpp`). |
| **Hallasi Cusi, Santiago Misael** | Heurística NEH (`neh.hpp`), diagramas de Gantt (`gantt.hpp`) e integración del sistema (`main.cpp`). |

---

## 10. Referencias bibliográficas

1. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to Algorithms* (4th ed.). MIT Press.
2. Fernandez-Viagas, V., & Framinan, J. M. (2022). Exploring the benefits of scheduling with advanced and delayed due dates in permutation flow shops. *Computers & Industrial Engineering, 169*, 108228.
3. Pinedo, M. L. (2022). *Scheduling: Theory, Algorithms, and Systems* (6th ed.). Springer.
4. Rossit, D. A., Tohmé, F., & Frutos, M. (2021). The non-permutation flow-shop scheduling problem: A literature review. *Omega, 103*, 102434.
5. Tliba, K., et al. (2023). Digital twin-driven dynamic scheduling of a hybrid flow shop. *Journal of Intelligent Manufacturing, 34*(5), 2281–2306.

---

**Universidad Nacional de San Agustín de Arequipa (UNSA)** · Escuela Profesional de Ingeniería de Sistemas · Curso: Análisis y Diseño de Algoritmos