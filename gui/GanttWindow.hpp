#pragma once

#include <QMainWindow>
#include "GanttWidget.hpp"

class QScrollArea;
class QLabel;

class GanttWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GanttWindow(QWidget* parent = nullptr);

    void mostrarResultado(const QString& metodo,
                          const fs::Matriz& instancia,
                          const fs::Permutacion& secuencia);

private:
    QScrollArea* scroll;
    GanttWidget* gantt;
    QLabel* datos;
};
