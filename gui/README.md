# Interfaz Qt - Flow Shop Permutacional

Interfaz gráfica escrita en C++17 y Qt 6 para crear o cargar instancias, ejecutar NEH, Johnson y fuerza bruta, y visualizar el diagrama de Gantt.

## Uso

Desde la raíz del proyecto:

```bash
cmake -S gui -B build-qt -DCMAKE_BUILD_TYPE=Release
cmake --build build-qt -j2
```

Ejecuta la interfaz:

```bash
./build-qt/flowshop_gui
```

En Linux con Wayland, usa XCB si fuera necesario:

```bash
QT_QPA_PLATFORM=xcb ./build-qt/flowshop_gui
```

### Pasos en la interfaz

1. Seleccione **Cargar archivo** y elija una instancia de `datos/`.
2. También puede indicar trabajos y máquinas, crear la matriz y editar sus tiempos.
3. Pulse **NEH**, **Fuerza bruta** o **Johnson**.
4. La ventana de resultados muestra el método, secuencia y makespan.
5. Para instancias grandes, use las barras horizontal y vertical del diagrama Gantt.

Johnson requiere exactamente dos máquinas. La fuerza bruta está limitada a 11 trabajos por el alto costo factorial.

## Implementación

- `MainWindow`: matriz editable, carga de archivos y ejecución de métodos.
- `GanttWindow`: ventana separada con datos y diagrama del resultado.
- `GanttWidget`: dibuja los bloques de las máquinas y calcula sus intervalos.
- `include/`: algoritmos compartidos bajo el namespace `fs`, sin duplicar el makespan.

El formato de archivo es `n m`, seguido de una fila de tiempos por trabajo. Los comentarios iniciados con `#` se ignoran.

Consulte `requirements.txt` para instalar las dependencias requeridas.
