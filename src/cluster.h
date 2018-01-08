#ifndef SWARMCLASS_H
#define SWARMCLASS_H
#include <QString>
#include <QStandardItemModel>
#include <QList>
#include <QDateTime>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QGeoCoordinate>

struct Event {
    QDateTime       EventTime;
    QGeoCoordinate  EventCord;
    double          Magnitude;
};

struct Cluster {
    int ID;
    bool Sequence; // false if not a sequence
    QList<Event>          Events;
    QList<qint64>         DT;
    QList<double>         DM;
    QList<double>         DD;
    QDateTime             LatestTime;
    QDateTime             FirstTime;
    QGeoCoordinate        FirstCoord;
    double                LatestMag;
    QList<int>            HistogramData;
    int                   numberofEvents;
};

class ClusterF : public QObject {
    Q_OBJECT

public:
    void identifyCluster(QList<Event> Events);
    void setDistT(double Dist);
    void setMagT(double Mag);
    void setTimeT(qint64 Time);
    void setMinEV(int Min);
    void setMethod(bool Pass);
    int getNumCluster();
    int getNumFCluster();
    QList<Cluster> getClusters();
    int getMinEV();
    double getMagT();

signals:
    void dbstat(QString);

private:
    void addEvent(Event event);
    void identifySequences();
    void removeEmpty();
    void createHist();
    static bool emptyfilter5(const Cluster &t);
    static bool emptyfilter10(const Cluster &t);
    static bool emptyfilter15(const Cluster &t);
    static bool emptyfilter20(const Cluster &t);
    static bool emptyfilter25(const Cluster &t);
    QList<Cluster> Clusters;
    QFuture<Cluster> Filtered;
    int    MinEv;
    double MagT;
    double DistT;
    bool   twoPass;
    qint64 TimeT;
};

#endif // SWARMCLASS_H
