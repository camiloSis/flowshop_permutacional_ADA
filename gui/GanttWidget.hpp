#pragma once

#include <QWidget>
#include "../include/makespan.hpp"

class GanttWidget : public QWidget {
    Q_OBJECT

public:
    explicit GanttWidget(QWidget* parent = nullptr);
    void establecer(const fs::Matriz& instancia, const fs::Permutacion& secuencia);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    fs::Matriz p;
    fs::Permutacion orden;
};
