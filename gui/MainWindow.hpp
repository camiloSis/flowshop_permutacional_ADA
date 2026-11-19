#pragma once

#include <QMainWindow>
#include <vector>

#include "GanttWindow.hpp"
#include "../include/makespan.hpp"

class QTableWidget;
class QSpinBox;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QTableWidget* tabla;
    QSpinBox* trabajos;
    QSpinBox* maquinas;
    QLabel* estado;
    GanttWindow* gantt;
    fs::Matriz instancia;
    fs::Permutacion secuenciaActual;

    void crearInstanciaVacia();
    fs::Matriz leerTabla() const;
    void actualizarSecuencia(const fs::Permutacion& secuencia, const char* metodo);

private slots:
    void cargarArchivo();
    void redimensionarTabla();
    void ejecutarNEH();
    void ejecutarBruta();
    void ejecutarJohnson();
};
