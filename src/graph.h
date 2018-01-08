#ifndef GRAPH_H
#define GRAPH_H
#include "qcustomplot/qcustomplot.h"
#include "cluster.h"

#include <QMainWindow>

namespace Ui {
class Graph;
}

class Graph : public QMainWindow
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = 0);
    ~Graph();
    void SetView(QList<Cluster> InSwarm, int time);

private slots:

    void on_spinBox_valueChanged(int arg1);

    void on_YCombo_currentIndexChanged(int index);

    void on_XCombo_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_SaveJpg_clicked();

private:
    Ui::Graph *ui;
    void PopModel();
    int currentS;
    int currentX;
    int currentY;
    int timeT;
    QList<QColor> rcolors;
    enum tableX{Time, DTX, Lon};
    enum tableY{Magnitude, Depth, DDY, DMY, DTY, Lat};

    QList<Cluster> MyClusters;
};

#endif // GRAPH_H
