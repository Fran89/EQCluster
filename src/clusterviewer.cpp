#include "clusterviewer.h"
#include "ui_clusterviewer.h"

ClusterViewer::ClusterViewer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ClusterViewer)
{
    ui->setupUi(this);
    this->setWindowTitle("Cluster Viewer Lite");
    Cluster_Graph = new Graph(this);
    clmodel = new QStandardItemModel(0,5);
    current = 0;
}

ClusterViewer::~ClusterViewer()
{
    Cluster_Graph->close();
    delete Cluster_Graph;
    delete ui;
}

void ClusterViewer::SetView(QList<Cluster> InCluster,int time){
    MyCluster = InCluster;
    Cluster_Graph->SetView(InCluster, time);
    ui->spinBox->setMinimum(0);
    ui->spinBox->setMaximum(MyCluster.length()-1);
    PopModel(0);
}

void ClusterViewer::PopModel(int arg1){

    current = arg1;
    QList<QStandardItem*> tempL;
    delete clmodel;
    clmodel = new QStandardItemModel(0,5);
    clmodel->setHorizontalHeaderItem(0,new QStandardItem("Date Time"));
    clmodel->setHorizontalHeaderItem(1,new QStandardItem("Coordinate"));
    clmodel->setHorizontalHeaderItem(2,new QStandardItem("Magnitude"));
    clmodel->setHorizontalHeaderItem(3,new QStandardItem("Delta Time (min from last)"));
    clmodel->setHorizontalHeaderItem(4,new QStandardItem("Delta Distance (in KM"));
    clmodel->setHorizontalHeaderItem(5,new QStandardItem("Delta Magnitude"));

    for (int i = 0; i < MyCluster[arg1].Events.size(); i++){
        tempL.clear();
        tempL.append(new QStandardItem(MyCluster[arg1].Events.at(i).EventTime.toString()));
        tempL.append(new QStandardItem(MyCluster[arg1].Events.at(i).EventCord.toString()));
        tempL.append(new QStandardItem(QString::number(MyCluster[arg1].Events.at(i).Magnitude)));
        tempL.append(new QStandardItem(QString::number(MyCluster[arg1].DT[i]*0.000016667)));
        tempL.append(new QStandardItem(QString::number(MyCluster[arg1].DD[i])));
        tempL.append(new QStandardItem(QString::number(MyCluster[arg1].DM[i])));
        clmodel->appendRow(tempL);
    }
    ui->tableView->setModel(clmodel);
    ui->tableView->resizeColumnsToContents();
}

void ClusterViewer::on_spinBox_valueChanged(int arg1)
{
    PopModel(arg1);
}

void ClusterViewer::on_d2csv_clicked()
{
    int current = ui->spinBox->value();
    QFile savef;
    QString fileName = QFileDialog::getSaveFileName(this,"Save as:",QDir::currentPath(),"*.csv");
    savef.setFileName(fileName);
    if (!savef.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&savef);

    out << "Latitude"
           << ", "
           << "Longitude"
           << ", "
           << "Depth"
           << ", "
           << "Date"
           << ", "
           << "Time"
           << ", "
           << "Magnitude"
           << "\n";

    for (int i=0; i < MyCluster.at(current).Events.size(); i++){
        out << MyCluster.at(current).Events.at(i).EventCord.latitude()
               << ", "
               << MyCluster.at(current).Events.at(i).EventCord.longitude()
               << ", "
               << MyCluster.at(current).Events.at(i).EventCord.altitude()
               << ", "
               << MyCluster.at(current).Events.at(i).EventTime.toString("yyyy/MM/dd")
               << ", "
               << MyCluster.at(current).Events.at(i).EventTime.toString("HH:mm:ss:zzz")
               << ", "
               << MyCluster.at(current).Events.at(i).Magnitude
               << "\n";
    }
    savef.close();
}

void ClusterViewer::on_da2csv_clicked()
{
    QMessageBox SelBox;
    SelBox.setText("Multiple Clusters Selected");
    SelBox.setInformativeText("Would you like to save as one file?");
    SelBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = SelBox.exec();
    switch (ret) {
    case QMessageBox::Yes:

        {
        QFile savef;
        QString fileName = QFileDialog::getSaveFileName(this,"Save as:",QDir::currentPath(),"*.csv");
        savef.setFileName(fileName);
        if (!savef.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&savef);
        out << "Latitude"
               << ", "
               << "Longitude"
               << ", "
               << "Depth"
               << ", "
               << "Date"
               << ", "
               << "Time"
               << ", "
               << "Magnitude"
               << ", "
               << "Date/Time for QGIS"
               << ", "
               << "Type"
               << ", "
               << "ClusterLatestTime(ID)"
               << ", "
               << "SNID"
               << "\n";
        for (int j=0; j < MyCluster.size(); j++) {
            out << MyCluster.at(j).FirstCoord.latitude()
                << ", "
                << MyCluster.at(j).FirstCoord.longitude()
                << ", "
                << MyCluster.at(j).FirstCoord.altitude()
                << ", "
                << MyCluster.at(j).FirstTime.toString("yyyy/MM/dd")
                << ", "
                << MyCluster.at(j).FirstTime.toString("HH:mm:ss:zzz")
                << ", "
                << MyCluster.at(j).LatestMag
                << ", "
                << MyCluster.at(j).LatestTime.toString("yyyy-MM-dd HH:mm:ss")
                << ", "
                << "CENTER"
                << ", "
                << MyCluster.at(j).FirstTime.toString("HH:mm:ss dd/MM/yy")
                << ", "
                << "-1"
                << "\n";
            for (int i=0; i < MyCluster.at(j).Events.size(); i++){
                out << MyCluster.at(j).Events.at(i).EventCord.latitude()
                    << ", "
                    << MyCluster.at(j).Events.at(i).EventCord.longitude()
                    << ", "
                    << MyCluster.at(j).Events.at(i).EventCord.altitude()
                    << ", "
                    << MyCluster.at(j).Events.at(i).EventTime.toString("yyyy/MM/dd")
                    << ", "
                    << MyCluster.at(j).Events.at(i).EventTime.toString("HH:mm:ss:zzz")
                    << ", "
                    << MyCluster.at(j).Events.at(i).Magnitude
                    << ", "
                    << MyCluster.at(j).Events.at(i).EventTime.toString("yyyy-MM-dd HH:mm:ss")
                    << ", "
                    << "EVENT"
                    << ", "
                    << MyCluster.at(j).FirstTime.toString("HH:mm:ss dd/MM/yy")
                    << ", "
                    << QString::number(j)
                    << "\n";
            }
        }
        savef.close();
        }
        break;
    case QMessageBox::No:

    {
    QFile savef;
    QString fileName = QFileDialog::getSaveFileName(this,"Save as:",QDir::currentPath(),"*.csv");
    for (int j=0; j < MyCluster.size(); j++) {
        savef.setFileName(fileName+"_Cluster"+QString::number(j)+".csv");
        if (!savef.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&savef);
        out << "Latitude"
               << ", "
               << "Longitude"
               << ", "
               << "Depth"
               << ", "
               << "Date"
               << ", "
               << "Time"
               << ", "
               << "Magnitude"
               << ", "
               << "Date/Time for QGIS"
               << "\n";
        for (int i=0; i < MyCluster.at(j).Events.size(); i++){
            out << MyCluster.at(j).Events.at(i).EventCord.latitude()
                << ", "
                << MyCluster.at(j).Events.at(i).EventCord.longitude()
                << ", "
                << MyCluster.at(j).Events.at(i).EventCord.altitude()
                << ", "
                << MyCluster.at(j).Events.at(i).EventTime.toString("yyyy/MM/dd")
                << ", "
                << MyCluster.at(j).Events.at(i).EventTime.toString("HH:mm:ss:zzz")
                << ", "
                << MyCluster.at(j).Events.at(i).Magnitude
                << ", "
                << MyCluster.at(j).Events.at(i).EventTime.toString("yyyy-MM-dd HH:mm:ss")
                << "\n";
        }
        savef.close();
    }
    }
        break;
    default:
        break;
    }
}

void ClusterViewer::on_pushButton_clicked()
{
    Cluster_Graph->show();
}
