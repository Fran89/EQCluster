#include "graph.h"
#include "ui_graph.h"

Graph::Graph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Graph)
{
    ui->setupUi(this);
    this->setWindowTitle("Cluster Graph Viewer Lite");
    currentS = 0;
    currentX = 0;
    currentY = 0;
}

Graph::~Graph()
{
    delete ui;
}

void Graph::SetView(QList<Cluster> InCluster, int time){
    MyClusters = InCluster;
    ui->spinBox->setMinimum(-1);
    ui->spinBox->setValue(0);
    ui->spinBox->setMaximum(MyClusters.length()-1);
    for (int a = 0; a < MyClusters.size(); a++){
        rcolors.append(QColor((rand() % 255),(rand() % 255),(rand() % 255)));
    }
    PopModel();
    timeT=time;
}
void Graph::PopModel(){

    if (currentS >= 0){
        if(currentX == Time){
            QVector<double> time;
            ui->qgraph->clearGraphs();
            ui->qgraph->clearPlottables();
            QCPGraph *currentGraph = ui->qgraph->addGraph();
            ui->qgraph->xAxis->setLabel("Time");
            ui->qgraph->xAxis->setTickLabelType(QCPAxis::ltDateTime);
            ui->qgraph->xAxis->setDateTimeFormat("yy/MM/dd");
            currentGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
            currentGraph->setLineStyle(QCPGraph::lsNone);
            currentGraph->setPen(QPen(rcolors.at(currentS)));
            for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                time.append( (double) MyClusters.at(currentS).Events.at(i).EventTime.toTime_t() );

            if(currentY == Magnitude){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Magnitude");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).Magnitude);
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == Depth){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Depth (Km)");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).EventCord.altitude());
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DDY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Distance (Km)");
                var = MyClusters.at(currentS).DD.toVector();
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DMY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Magnitude");
                var = MyClusters.at(currentS).DM.toVector();
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DTY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Time");
                for(int i = 0; i < MyClusters.at(currentS).DT.size(); i++)
                    var.append( (double) (MyClusters.at(currentS).DT.at(i)*0.000016667) );
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == Lat){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Magnitude");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).EventCord.latitude());
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

        } else if(currentX == DTX){
            QVector<double> time;
            ui->qgraph->clearGraphs();
            ui->qgraph->clearPlottables();
            QCPGraph *currentGraph = ui->qgraph->addGraph();
            ui->qgraph->xAxis->setLabel("Delta Time (min)");
            ui->qgraph->xAxis->setTickLabelType(QCPAxis::ltNumber);
            currentGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
            currentGraph->setLineStyle(QCPGraph::lsNone);
            currentGraph->setPen(QPen(rcolors.at(currentS)));
            for(int i = 0; i < MyClusters.at(currentS).DT.size(); i++)
                time.append( (double) (MyClusters.at(currentS).DT.at(i)*0.000016667) );

            if(currentY == Magnitude){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Magnitude");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).Magnitude);
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == Depth){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Depth (Km)");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).EventCord.altitude());
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DDY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Distance (Km)");
                var = MyClusters.at(currentS).DD.toVector();
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DMY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Magnitude");
                var = MyClusters.at(currentS).DM.toVector();
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DTY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Time");
                for(int i = 0; i < MyClusters.at(currentS).DT.size(); i++)
                    var.append( (double) (MyClusters.at(currentS).DT.at(i)*0.000016667) );
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == Lat){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Magnitude");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).EventCord.latitude());
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

        }else if(currentX == Lon){
            QVector<double> time;
            ui->qgraph->clearGraphs();
            ui->qgraph->clearPlottables();
            QCPGraph *currentGraph = ui->qgraph->addGraph();
            ui->qgraph->xAxis->setLabel("Longitude");
            ui->qgraph->xAxis->setTickLabelType(QCPAxis::ltNumber);
            currentGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
            currentGraph->setLineStyle(QCPGraph::lsNone);
            currentGraph->setPen(QPen(rcolors.at(currentS)));
            for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                time.append( MyClusters.at(currentS).Events.at(i).EventCord.longitude() );

            if(currentY == Magnitude){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Magnitude");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).Magnitude);
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == Depth){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Depth (Km)");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).EventCord.altitude());
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DDY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Distance (Km)");
                var = MyClusters.at(currentS).DD.toVector();
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DMY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Magnitude");
                var = MyClusters.at(currentS).DM.toVector();
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == DTY){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Delta Time");
                for(int i = 0; i < MyClusters.at(currentS).DT.size(); i++)
                    var.append( (double) (MyClusters.at(currentS).DT.at(i)*0.000016667) );
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }

            if(currentY == Lat){
                QVector<double> var;
                ui->qgraph->yAxis->setLabel("Latitude");
                for(int i = 0; i < MyClusters.at(currentS).Events.size(); i++)
                    var.append(MyClusters.at(currentS).Events.at(i).EventCord.latitude());
                currentGraph->addData(time,var);
                ui->qgraph->rescaleAxes();
                ui->qgraph->replot();
            }
        }
    }
    else if (currentS < 0){
        ui->qgraph->clearGraphs();
        ui->qgraph->clearPlottables();
        for (int a = 0; a < MyClusters.size(); a++){
            if(currentX == Time){
                QVector<double> time;
                QCPGraph *currentGraph = ui->qgraph->addGraph();
                ui->qgraph->xAxis->setLabel("Time");
                ui->qgraph->xAxis->setTickLabelType(QCPAxis::ltDateTime);
                ui->qgraph->xAxis->setDateTimeFormat("yy/MM/dd");
                currentGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
                currentGraph->setPen(QPen(rcolors.at(a)));
                currentGraph->setLineStyle(QCPGraph::lsNone);
                for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                    time.append( (double) MyClusters.at(a).Events.at(i).EventTime.toTime_t() );

                if(currentY == Magnitude){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Magnitude");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).Magnitude);
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == Depth){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Depth (Km)");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).EventCord.altitude());
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == DDY){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Delta Distance (Km)");
                    var = MyClusters.at(a).DD.toVector();
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == DMY){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Delta Magnitude");
                    var = MyClusters.at(a).DM.toVector();
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == DTY){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Delta Time");
                    for(int i = 0; i < MyClusters.at(a).DT.size(); i++)
                        var.append( (double) (MyClusters.at(a).DT.at(i)*0.000016667) );
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == Lat){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Magnitude");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).EventCord.latitude());
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

            } else if(currentX == DTX){
                QVector<double> time;
                QCPGraph *currentGraph = ui->qgraph->addGraph();
                ui->qgraph->xAxis->setLabel("Delta Time (min)");
                ui->qgraph->xAxis->setTickLabelType(QCPAxis::ltNumber);
                currentGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
                currentGraph->setPen(QPen(rcolors.at(a)));
                currentGraph->setLineStyle(QCPGraph::lsNone);
                for(int i = 0; i < MyClusters.at(a).DT.size(); i++)
                    time.append( (double) (MyClusters.at(a).DT.at(i)*0.000016667) );

                if(currentY == Magnitude){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Magnitude");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).Magnitude);
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == Depth){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Depth (Km)");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).EventCord.altitude());
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == DDY){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Delta Distance (Km)");
                    var = MyClusters.at(a).DD.toVector();
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == DMY){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Delta Magnitude");
                    var = MyClusters.at(a).DM.toVector();
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == Lat){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Magnitude");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).EventCord.latitude());
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

            }else if(currentX == Lon){
                QVector<double> time;
                QCPGraph *currentGraph = ui->qgraph->addGraph();
                ui->qgraph->xAxis->setLabel("Longitude");
                ui->qgraph->xAxis->setTickLabelType(QCPAxis::ltNumber);
                currentGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
                currentGraph->setPen(QPen(rcolors.at(a)));
                currentGraph->setLineStyle(QCPGraph::lsNone);
                for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                    time.append( MyClusters.at(a).Events.at(i).EventCord.longitude() );

                if(currentY == Magnitude){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Magnitude");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).Magnitude);
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == Depth){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Depth (Km)");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).EventCord.altitude());
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == DDY){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Delta Distance (Km)");
                    var = MyClusters.at(a).DD.toVector();
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == DMY){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Delta Magnitude");
                    var = MyClusters.at(a).DM.toVector();
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }

                if(currentY == Lat){
                    QVector<double> var;
                    ui->qgraph->yAxis->setLabel("Latitude");
                    for(int i = 0; i < MyClusters.at(a).Events.size(); i++)
                        var.append(MyClusters.at(a).Events.at(i).EventCord.latitude());
                    currentGraph->addData(time,var);
                    ui->qgraph->rescaleAxes();
                    ui->qgraph->replot();
                }
            }
        }
    }
}

void Graph::on_spinBox_valueChanged(int arg1)
{
    currentS = arg1;
    PopModel();
}

void Graph::on_YCombo_currentIndexChanged(int index)
{
    currentY = index;
    PopModel();
}


void Graph::on_XCombo_currentIndexChanged(int index)
{
    currentX = index;
    PopModel();
}

void Graph::on_pushButton_clicked()
{
    if(currentS != -1){
        // Shortcut
        QCustomPlot *customplot = ui->qgraph;

        // Clear
        customplot->clearGraphs();
        customplot->clearPlottables();

        // Setup data
        QVector<double> ticks;
        QVector<double> epd;
        for (int i = 0; i < MyClusters.at(currentS).HistogramData.size(); i++){
            ticks.append(i+1);
            epd.append(MyClusters.at(currentS).HistogramData.at(i));
        }

        // Setup Bars
        QPen pen;
        pen.setWidth(1);
        QCPBars *height = new QCPBars(ui->qgraph->xAxis,ui->qgraph->yAxis);
        height->setName("Number of Events");
        height->setPen(pen);
        height->setBrush(QColor(255, 131, 0, 50));
        height->setWidth(1);
        customplot->addPlottable(height);
        customplot->xAxis->setTickVector(ticks);
        customplot->xAxis->setLabel(QString::number(timeT)+" Hour Slices");
        customplot->yAxis->setLabel("# of Events");
        customplot->xAxis->setTickLabelType(QCPAxis::ltNumber);
        height->setData(ticks,epd);




        ui->qgraph->rescaleAxes();
        ui->qgraph->replot();
    }
}

void Graph::on_pushButton_2_clicked()
{
    if(currentS != -1){
        // Shortcut
        QCustomPlot *customplot = ui->qgraph;

        // Clear
        customplot->clearGraphs();
        customplot->clearPlottables();

        // Setup data
        QVector<double> ticks;
        QVector<double> summag;
        double sum = 0;
        for (int i = 0; i < MyClusters.at(currentS).Events.size(); i++){
            ticks.append(i+1);
            sum += MyClusters.at(currentS).Events.at(i).Magnitude;
            summag.append(sum);
        }
        QCPGraph *currentGraph = customplot->addGraph();

        customplot->xAxis->setLabel("Events");
        customplot->xAxis->setTickLabelType(QCPAxis::ltNumber);
        currentGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
        currentGraph->setLineStyle(QCPGraph::lsNone);
        customplot->yAxis->setLabel("Magnitude");
        currentGraph->addData(ticks,summag);
        customplot->rescaleAxes();
        customplot->replot();
    }
}

void Graph::on_SaveJpg_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save as:",QDir::currentPath(),"*.jpg");
    if (!fileName.contains("*.jpg"))
            fileName += ".jpg";
    ui->qgraph->saveJpg(fileName);
;
}
