#include "GanttWidget.hpp"

#include <QPainter>
#include <algorithm>

GanttWidget::GanttWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(220);
    setStyleSheet("background: white;");
}

void GanttWidget::establecer(const fs::Matriz& instancia,
                             const fs::Permutacion& secuencia) {
    p = instancia;
    orden = secuencia;

    if (!p.empty() && !orden.empty()) {
        const long long cmax = fs::makespan(p, orden);
        const int ancho = std::max(1000, 95 + static_cast<int>(cmax * 12));
        const int alto = std::max(260, 75 + static_cast<int>(p[0].size()) * 42);
        setFixedSize(ancho, alto);
    } else {
        setFixedSize(1000, 260);
    }
    update();
}

void GanttWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (p.empty() || orden.empty()) {
        painter.drawText(rect(), Qt::AlignCenter,
                         "Ejecute un método para mostrar el diagrama de Gantt.");
        return;
    }

    const int maquinas = static_cast<int>(p[0].size());
    const int margenIzquierdo = 65;
    const int margenSuperior = 25;
    const int altoFila = 42;

    std::vector<long long> finMaquina(maquinas, 0);
    long long makespan = 0;
    struct Bloque { int maquina; long long inicio; long long fin; int trabajo; };
    std::vector<Bloque> bloques;

    for (int trabajo : orden) {
        long long finTrabajo = 0;
        for (int maquina = 0; maquina < maquinas; ++maquina) {
            const long long inicio = std::max(finMaquina[maquina], finTrabajo);
            const long long fin = inicio + p[trabajo][maquina];
            bloques.push_back({maquina, inicio, fin, trabajo});
            finMaquina[maquina] = fin;
            finTrabajo = fin;
        }
    }

    makespan = fs::makespan(p, orden);
    const int anchoDisponible = width() - margenIzquierdo - 30;
    const double escala = makespan > 0
        ? static_cast<double>(anchoDisponible) / makespan
        : 1.0;

    painter.setFont(QFont("Sans", 9));
    for (int maquina = 0; maquina < maquinas; ++maquina) {
        const int y = margenSuperior + maquina * altoFila;
        painter.drawText(8, y + 25, QString("M%1").arg(maquina + 1));
        painter.setPen(QPen(Qt::lightGray));
        painter.drawLine(margenIzquierdo, y + 18,
                         width() - 15, y + 18);
    }

    for (const Bloque& bloque : bloques) {
        const int x = margenIzquierdo + static_cast<int>(bloque.inicio * escala);
        const int ancho = std::max(1, static_cast<int>((bloque.fin - bloque.inicio) * escala));
        const int y = margenSuperior + bloque.maquina * altoFila + 5;

        QColor color = QColor::fromHsv((bloque.trabajo * 80) % 360, 110, 230);
        painter.setPen(Qt::black);
        painter.setBrush(color);
        painter.drawRect(x, y, ancho, altoFila - 10);
        painter.drawText(x, y, ancho, altoFila - 10,
                         Qt::AlignCenter,
                         QString("T%1\n[%2,%3)").arg(bloque.trabajo + 1)
                             .arg(bloque.inicio).arg(bloque.fin));
    }

    painter.setPen(Qt::red);
    const int xFin = margenIzquierdo + static_cast<int>(makespan * escala);
    painter.drawLine(xFin, margenSuperior,
                     xFin, margenSuperior + maquinas * altoFila);
    painter.drawText(xFin - 35, margenSuperior + maquinas * altoFila + 22,
                     QString("Cmax = %1").arg(makespan));
}
