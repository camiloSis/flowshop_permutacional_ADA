#include "MainWindow.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>

#include "../include/neh.hpp"
#include "../include/fuerza_bruta.hpp"
#include "../include/johnson.hpp"

#include <sstream>
#include <stdexcept>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Flow Shop Permutacional - Interfaz Qt");

    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    auto* controles = new QFormLayout();
    trabajos = new QSpinBox(this);
    maquinas = new QSpinBox(this);
    trabajos->setRange(1, 1000);
    maquinas->setRange(1, 100);
    trabajos->setValue(4);
    maquinas->setValue(3);
    controles->addRow("Trabajos:", trabajos);
    controles->addRow("Máquinas:", maquinas);

    auto* botones = new QWidget(this);
    auto* botonesLayout = new QVBoxLayout(botones);

    auto* cargar = new QPushButton("Cargar archivo", botones);
    auto* crear = new QPushButton("Crear / redimensionar matriz", botones);
    auto* neh = new QPushButton("Ejecutar NEH", botones);
    auto* bruta = new QPushButton("Ejecutar fuerza bruta", botones);
    auto* johnson = new QPushButton("Ejecutar Johnson", botones);

    botonesLayout->addWidget(cargar);
    botonesLayout->addWidget(crear);
    botonesLayout->addWidget(neh);
    botonesLayout->addWidget(bruta);
    botonesLayout->addWidget(johnson);

    auto* filaSuperior = new QWidget(this);
    auto* filaSuperiorLayout = new QHBoxLayout(filaSuperior);
    filaSuperiorLayout->addLayout(controles);
    filaSuperiorLayout->addWidget(botones);

    tabla = new QTableWidget(this);
    tabla->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    estado = new QLabel("Cargue un archivo o cree una matriz.", this);
    estado->setWordWrap(true);

    gantt = new GanttWindow(this);

    layout->addWidget(filaSuperior);
    layout->addWidget(tabla);
    layout->addWidget(estado);
    setCentralWidget(central);

    connect(cargar, &QPushButton::clicked, this, &MainWindow::cargarArchivo);
    connect(crear, &QPushButton::clicked, this, &MainWindow::redimensionarTabla);
    connect(neh, &QPushButton::clicked, this, &MainWindow::ejecutarNEH);
    connect(bruta, &QPushButton::clicked, this, &MainWindow::ejecutarBruta);
    connect(johnson, &QPushButton::clicked, this, &MainWindow::ejecutarJohnson);

    crearInstanciaVacia();
}

void MainWindow::crearInstanciaVacia() {
    redimensionarTabla();
}

void MainWindow::redimensionarTabla() {
    const int n = trabajos->value();
    const int m = maquinas->value();
    tabla->setRowCount(n);
    tabla->setColumnCount(m);

    for (int i = 0; i < n; ++i)
        tabla->setVerticalHeaderItem(i,
            new QTableWidgetItem(QString("T%1").arg(i + 1)));
    for (int j = 0; j < m; ++j)
        tabla->setHorizontalHeaderItem(j,
            new QTableWidgetItem(QString("M%1").arg(j + 1)));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (!tabla->item(i, j))
                tabla->setItem(i, j, new QTableWidgetItem("0"));
}

fs::Matriz MainWindow::leerTabla() const {
    fs::Matriz p(tabla->rowCount(), std::vector<int>(tabla->columnCount(), 0));
    for (int i = 0; i < tabla->rowCount(); ++i) {
        for (int j = 0; j < tabla->columnCount(); ++j) {
            bool valido = false;
            const int tiempo = tabla->item(i, j)->text().toInt(&valido);
            if (!valido || tiempo < 0)
                throw std::invalid_argument("Todos los tiempos deben ser enteros no negativos.");
            p[i][j] = tiempo;
        }
    }
    fs::validarInstancia(p);
    return p;
}

void MainWindow::actualizarSecuencia(const fs::Permutacion& secuencia,
                                     const char* metodo) {
    secuenciaActual = secuencia;
    instancia = leerTabla();

    QString texto = QString("%1: ").arg(metodo);
    for (std::size_t i = 0; i < secuencia.size(); ++i) {
        if (i) texto += " -> ";
        texto += QString("T%1").arg(secuencia[i] + 1);
    }
    texto += QString(" | Makespan: %1").arg(fs::makespan(instancia, secuencia));
    estado->setText(texto);
    gantt->mostrarResultado(QString::fromLatin1(metodo), instancia, secuencia);
}

void MainWindow::cargarArchivo() {
    const QString ruta = QFileDialog::getOpenFileName(
        this, "Cargar instancias", "datos",
        "Instancias (*.txt);;Todos los archivos (*.*)");
    if (ruta.isEmpty()) return;

    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Archivo", "No se pudo abrir el archivo.");
        return;
    }

    std::vector<int> numeros;
    QTextStream stream(&archivo);
    while (!stream.atEnd()) {
        QString linea = stream.readLine();
        const qsizetype comentario = linea.indexOf('#');
        if (comentario >= 0) linea = linea.left(comentario);

        std::stringstream datos(linea.toStdString());
        int valor;
        while (datos >> valor) numeros.push_back(valor);
    }

    if (numeros.size() < 2) {
        QMessageBox::warning(this, "Formato", "El archivo no contiene una instancia válida.");
        return;
    }

    const int n = numeros[0];
    const int m = numeros[1];
    if (n <= 0 || m <= 0 || numeros.size() < static_cast<std::size_t>(2 + n * m)) {
        QMessageBox::warning(this, "Formato", "Faltan dimensiones o tiempos.");
        return;
    }

    trabajos->setValue(n);
    maquinas->setValue(m);
    redimensionarTabla();

    std::size_t pos = 2;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            tabla->item(i, j)->setText(QString::number(numeros[pos++]));

    instancia = leerTabla();
    secuenciaActual.clear();
    estado->setText(QString("Instancia cargada: %1 trabajos, %2 máquinas.").arg(n).arg(m));
}

void MainWindow::ejecutarNEH() {
    try {
        instancia = leerTabla();
        actualizarSecuencia(fs::resolverNEH(instancia), "NEH");
    } catch (const std::exception& error) {
        QMessageBox::warning(this, "NEH", error.what());
    }
}

void MainWindow::ejecutarBruta() {
    try {
        instancia = leerTabla();
        if (instancia.size() > 11) {
            QMessageBox::warning(this, "Fuerza bruta",
                                 "Limite: 11 trabajos para evitar una ejecucion excesiva.");
            return;
        }
        auto resultado = fs::resolverFuerzaBruta(instancia);
        actualizarSecuencia(resultado.second, "Fuerza bruta");
    } catch (const std::exception& error) {
        QMessageBox::warning(this, "Fuerza bruta", error.what());
    }
}

void MainWindow::ejecutarJohnson() {
    try {
        instancia = leerTabla();
        actualizarSecuencia(fs::johnson(instancia), "Johnson");
    } catch (const std::exception& error) {
        QMessageBox::warning(this, "Johnson", error.what());
    }
}
