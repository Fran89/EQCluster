#include "EQCluster.h"
#include "ui_EQCluster.h"

EQCluster::EQCluster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EQCluster)
{
    ui->setupUi(this);
    this->setWindowTitle("EQCluster");
    Debug = false;
    model = new QStandardItemModel(0,0);
    connect(&cw,SIGNAL(sendParams(double,double,int,int,bool)),this,SLOT(addParams(double,double,int,int,bool)));\
    connect(&Ident,SIGNAL(dbstat(QString)),this,SLOT(updateTB(QString)));
}

EQCluster::~EQCluster()
{
    delete ui;
    delete model;
}

void EQCluster::on_actionLoad_CSV_triggered()
{
    model->clear();
    ui->tableView->setModel(model);
    QString fileName = QFileDialog::getOpenFileName (this, "Open CSV file",
                                                     QDir::currentPath(), "CSV (*.csv)");
    if(Debug)
        ui->textBrowser->append("Loading Data");
    bool First = true;
    QFile file (fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QString data = file.readAll();
        data.remove( QRegExp("\r") ); //remove all ocurrences of CR (Carriage Return)
        QString temp;
        QChar character;
        QTextStream textStream(&data);
        while (!textStream.atEnd()) {
            textStream >> character;
            if (character == ',') {
                checkString(temp, character);
            } else if (character == '\n') {
                checkString(temp, character, First);
                First = false;
            } else if (textStream.atEnd()) {
                temp.append(character);
                checkString(temp);
            } else {
                temp.append(character);
            }
        }
    }
    if(Debug)
        ui->textBrowser->append("Finished loading data");

    // Convert
    convert_to_customData();
    // Add Defaults
    addParams(1.3,50,12*3600000,10, true); // Time is in milliseconds (Default Params)
}

void EQCluster::convert_to_customData(){
    if(Debug)
        ui->textBrowser->append("Changing Data");
    
    QList<QStandardItem*> tempL;
    QStandardItem* temp;
    QDateTime tempdt;
    QGeoCoordinate tempgeo;
    double tempdouble;
    int h, m, s, ms, Y, M, D;

    newmodel = new QStandardItemModel(0,2);
    newmodel->setHorizontalHeaderItem(0,new QStandardItem("Date Time"));
    newmodel->setHorizontalHeaderItem(1,new QStandardItem("Coordinate"));
    newmodel->setHorizontalHeaderItem(2,new QStandardItem("Magnitude"));

    for (int i=0; i < model->rowCount(); i++) {
        Event etemp;
        h = m = s = ms = Y = M = D = 0;
        tempdouble = 0;
        tempL.clear();
        for(int j=0; j < model->columnCount(); j++){
            temp = new QStandardItem;
            temp = model->takeItem(i,j);
            switch(j){
            case 0:
                tempgeo.setLongitude(temp->text().toDouble());
                break;
            case 1:
                tempgeo.setLatitude(temp->text().toDouble());
                break;
            case 2:
                Y = temp->text().toDouble();
                break;
            case 3:
                M = temp->text().toInt();
                break;
            case 4:
                D = temp->text().toInt();
                break;
            case 5:
                tempdouble = temp->text().toDouble();
                break;
            case 6:
                tempgeo.setAltitude(temp->text().toDouble()*-1000);
                break;
            case 7:
                h = temp->text().toInt();
                break;
            case 8:
                m = temp->text().toInt();
                break;
            case 9:
                if(temp->text().contains('.')){
                    QStringList sms= temp->text().split('.');
                    s = sms.at(0).toInt();
                    if (sms.size() > 0){
                        int msint = sms.at(1).toInt();

                        if(1 > msint && msint <= 9)
                            ms = sms.at(1).toInt()*10;
                        if(10 > msint && msint <= 99)
                            ms = sms.at(1).toInt()*100;
                        if( sms.at(1).toInt() == 0)
                            ms = 0;
                    }
                    break;
                }
                else{
                    ms = 0;
                }
            }
            delete temp;
        }
        tempdt.setTime(QTime(h,m,s,ms));
        tempdt.setDate(QDate(Y,M,D));
        etemp.EventTime = tempdt;
        etemp.EventCord = tempgeo;
        etemp.Magnitude = tempdouble;
        Events.append(etemp);
        tempL.append(new QStandardItem(tempdt.toString()));
        tempL.append(new QStandardItem(tempgeo.toString()));
        tempL.append(new QStandardItem(QString::number(tempdouble)));
        newmodel->appendRow(tempL);
    }
    ui->tableView->setModel(newmodel);
    ui->tableView->resizeColumnsToContents();
    if (Debug){
        for (int i = 0 ; i < Events.size(); i++){
            ui->textBrowser->append(
                        QString::number(i)+". "+
                        Events[i].EventTime.toString()+" "+
                        Events[i].EventCord.toString()+" "+
                        QString::number(Events.at(i).Magnitude));
        }
    }
}

void EQCluster::checkString(QString &temp, QChar character, bool First)
{
    First = false; // don't skip first line.
    if(temp.count("\"")%2 == 0) {
        //if (temp.size() == 0 && character != ',') //problem with line endings
        //    return;
        if (temp.startsWith( QChar('\"')) && temp.endsWith( QChar('\"') ) ) {
             temp.remove( QRegExp("^\"") );
             temp.remove( QRegExp("\"$") );
        }
        //FIXME: will possibly fail if there are 4 or more reapeating double quotes
        temp.replace("\"\"", "\"");
        QStandardItem *item = new QStandardItem(temp);
        standardItemList.append(item);
        if (character != QChar(',') && First){
            for (int i = 0; i < standardItemList.size(); i++){
                model->setHorizontalHeaderItem(i,standardItemList.at(i));
            }
            standardItemList.clear();
        }
        if (character != QChar(',') && !First) {
            model->appendRow(standardItemList);
            standardItemList.clear();
        }
        temp.clear();
    } else {
        temp.append(character);
    }
}

void EQCluster::addParams(double Mag, double Dist, int Time, int MinS, bool twoPass)
{
    Ident.setMagT(Mag);
    Ident.setDistT(Dist);
    Ident.setTimeT(Time*3600000);
    Ident.setMinEV(MinS);
    Ident.setMethod(twoPass);
}

void EQCluster::on_actionDebug_Mode_triggered()
{
    Debug = !Debug;
    if (Debug)
        ui->textBrowser->append("Debug Mode ON");
    else
        ui->textBrowser->append("Debug Mode Off");
}

void EQCluster::on_actionSet_Search_Params_triggered()
{
    cw.show();
}

void EQCluster::on_actionSearch_for_Clusters_triggered()
{
    Ident.identifyCluster(Events);
    ui->textBrowser->append(QString::number(Ident.getNumCluster()) +" clusters Found");
    ui->textBrowser->append(QString::number(Ident.getNumFCluster()) +" clusters used (Filtered > " +
                            QString::number(Ident.getMinEV()) +
                            " )");

}

void EQCluster::updateTB(QString Status) {
    ui->textBrowser->append(Status);
}

void EQCluster::on_actionCluster_Viewer_triggered()
{
    cv.SetView(Ident.getClusters(),Ident.getMagT());
    cv.show();
}
