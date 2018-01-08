#ifndef SWARMVIEWER_H
#define SWARMVIEWER_H
#include <QFrame>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include "cluster.h"
#include "graph.h"

namespace Ui {
class ClusterViewer;
}

class ClusterViewer : public QFrame
{
    Q_OBJECT

public:
    explicit ClusterViewer(QWidget *parent = 0);
    ~ClusterViewer();
    void SetView(QList<Cluster> InCluster, int time);

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_d2csv_clicked();

    void on_da2csv_clicked();

    void on_pushButton_clicked();

private:
    Graph *Cluster_Graph;
    QList<Cluster> MyCluster;
    Ui::ClusterViewer *ui;
    void PopModel(int arg1);
    QStandardItemModel *clmodel;
    int current;
};

#endif // SWARMVIEWER_H
