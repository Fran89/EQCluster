#ifndef SWARM_DETECT_H
#define SWARM_DETECT_H
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QStandardItemModel>
#include <QList>
#include <QDateTime>
#include <QGeoCoordinate>
#include "cluster.h"
#include "clusterviewer.h"
#include "settings.h"

namespace Ui {
class EQCluster;
}

class EQCluster : public QMainWindow
{
    Q_OBJECT

public:
    explicit EQCluster(QWidget *parent = 0);
    ~EQCluster();

private slots:
    void on_actionLoad_CSV_triggered();
    void checkString(QString &temp, QChar character = 0, bool First = false);

    void on_actionDebug_Mode_triggered();

    void on_actionSet_Search_Params_triggered();

    void on_actionSearch_for_Clusters_triggered();

    void addParams(double Mag, double Dist, int Time, int MinS, bool twoPass);

    void updateTB(QString Status);

    void on_actionCluster_Viewer_triggered();

private:
    Ui::EQCluster *ui;
    QList<QStringList> csv;
    QStandardItemModel *model;
    QStandardItemModel *newmodel;
    QList<QStandardItem*> standardItemList;

    // Settings
    Settings cw;

    // Swarm Viewer
    ClusterViewer cv;

    // This stores the model individually
    QList<Event>      Events;

    //SwarmFind
    ClusterF Ident;

    bool Debug;

    void convert_to_customData();
};

#endif // SWARM_DETECT_H
