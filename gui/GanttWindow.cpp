#include "GanttWindow.hpp"

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

GanttWindow::GanttWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Diagrama de Gantt");
    resize(1000, 650);

    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    datos = new QLabel(central);
    datos->setWordWrap(true);

    gantt = new GanttWidget(central);

    scroll = new QScrollArea(central);
    scroll->setWidgetResizable(false);
    scroll->setWidget(gantt);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    layout->addWidget(datos);
    layout->addWidget(scroll);
    setCentralWidget(central);
}

void GanttWindow::mostrarResultado(const QString& metodo,
                                   const fs::Matriz& instancia,
                                   const fs::Permutacion& secuencia) {
    QString secuenciaTexto;
    for (std::size_t i = 0; i < secuencia.size(); ++i) {
        if (i) secuenciaTexto += " -> ";
        secuenciaTexto += QString("T%1").arg(secuencia[i] + 1);
    }

    datos->setText(
        QString("Método: %1\nSecuencia: %2\nMakespan: %3\nTrabajos: %4 | Máquinas: %5")
            .arg(metodo)
            .arg(secuenciaTexto)
            .arg(fs::makespan(instancia, secuencia))
            .arg(instancia.size())
            .arg(instancia[0].size()));

    gantt->establecer(instancia, secuencia);
    setWindowTitle(QString("Gantt - %1 - Cmax %2")
                       .arg(metodo)
                       .arg(fs::makespan(instancia, secuencia)));
    show();
    raise();
    activateWindow();
}
