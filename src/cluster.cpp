#include "cluster.h"

void ClusterF::identifyCluster(QList<Event> Events){

    if(!twoPass) {
        // Identify Method
        emit dbstat("Using method 1 Pass");
        // Clear Clusters
        Clusters.clear();

        // Initialize some variables
        Event           tempEvent;
        Cluster           tempCluster;
        static Cluster    Empty;

        // Take the last Event to intialize the first Cluster
        tempEvent = Events.takeLast();

        // Initialize the First Cluster
        int cluster = 0;
        tempCluster.ID=cluster;
        tempCluster.LatestTime = tempCluster.FirstTime = tempEvent.EventTime;
        tempCluster.FirstCoord = tempEvent.EventCord;
        tempCluster.numberofEvents = 1;
        tempCluster.Events.append(tempEvent);
        tempCluster.DD.append(0);
        tempCluster.DM.append(0);
        tempCluster.DT.append(0);
        tempCluster.HistogramData.clear();

        // Ad the first cluster to the list
        Clusters.append(tempCluster);

        // Initialize some more variables
        double Distance = 0, Magnitude = 0;
        qint64 Time = 0;
        bool found;

        // New and improved search function
        while(!Events.empty()){
            found = false;
            tempEvent = Events.takeLast();

            for(int i=0; i < Clusters.size(); i++){
                if (!found){
                    Time =  Clusters[i].LatestTime.toMSecsSinceEpoch() - tempEvent.EventTime.toMSecsSinceEpoch(); //Time diff in Msec
                    Distance =  Clusters[i].FirstCoord.distanceTo(tempEvent.EventCord)/1000; //Distance in Km
                    Magnitude =  fabs(Clusters[i].LatestMag - tempEvent.Magnitude);

                    //emit dbstat("Comparing to Cluster: "+QString::number(i));
                    //emit dbstat("Distance: "+QString::number(Distance));
                    //emit dbstat("TimeDiff: "+QString::number(Time));

                    if ( Time > 0 && Time <= TimeT && Distance <= DistT ){
                        Clusters[i].Events.append(tempEvent);
                        Clusters[i].LatestTime = tempEvent.EventTime;
                        Clusters[i].LatestMag = tempEvent.Magnitude;
                        Clusters[i].DD.append(Distance);
                        Clusters[i].DM.append(Magnitude);
                        Clusters[i].DT.append(Time);
                        Clusters[i].numberofEvents = Clusters[i].numberofEvents + 1 ;
                        found = true;
                        continue;
                    }
                }
            }
            if(!found) {
                // Empty The current Cluster
                tempCluster = Empty;
                // Initialize the Another Cluster
                cluster++;
                tempCluster.ID=cluster;
                tempCluster.LatestTime = tempCluster.FirstTime = tempEvent.EventTime;
                tempCluster.FirstCoord = tempEvent.EventCord;
                tempCluster.numberofEvents = 1;
                tempCluster.Events.append(tempEvent);
                tempCluster.DD.append(0);
                tempCluster.DM.append(0);
                tempCluster.DT.append(0);
                Clusters.append(tempCluster);
                found = true;
                continue;
            }
        }
    } else {
        // Identify Method
        emit dbstat("Using method 2 passes");
        // Clear Clusters
        Clusters.clear();

        // Initialize some variables
        Event           tempEvent;
        Cluster           tempCluster;
        static Cluster    Empty;

        // Let's try a new stratagy:
        // This will first look for possible events then deduce their center
        // Then the clusters will be search from the center of the cluster.
        QList<Event> PreList(Events);
        QList<Cluster> PreSList;
        QList<QGeoCoordinate> PreLoc;
        QList<QDateTime> PreTim;

        // Take the last Event to intialize the first cluster
        tempEvent = PreList.takeLast();

        // Initialize the First cluster
        int cluster = 0;
        tempCluster.ID=cluster;
        PreTim.append(tempEvent.EventTime);
        tempCluster.LatestTime = tempEvent.EventTime;
        tempCluster.FirstCoord = tempEvent.EventCord;
        tempCluster.numberofEvents = 1;
        tempCluster.Events.append(tempEvent);
        tempCluster.DD.append(0);
        tempCluster.DM.append(0);
        tempCluster.DT.append(0);

        // Addthe first cluster to the list
        PreSList.append(tempCluster);

        // Initialize some more variables
        double Distance = 0, Magnitude = 0;
        qint64 Time = 0;
        bool found;

        // This loop will find Preliminary locations for a Cluster
        while(!PreList.empty()){
            found = false;
            tempEvent = PreList.takeLast();

            for(int i=0; i < PreSList.size(); i++){
                if (!found){
                    Time =  PreSList[i].LatestTime.toMSecsSinceEpoch() - tempEvent.EventTime.toMSecsSinceEpoch(); //Time diff in Msec
                    Distance =  PreSList[i].FirstCoord.distanceTo(tempEvent.EventCord)/1000; //Distance in Km
                    Magnitude =  PreSList[i].LatestMag - tempEvent.Magnitude;

                    if ( Time > 0 && Time <= TimeT && Distance <= DistT ){
                        PreSList[i].Events.append(tempEvent);
                        PreSList[i].LatestTime = tempEvent.EventTime;
                        PreSList[i].LatestMag = (PreSList.at(i).LatestMag+tempEvent.Magnitude)/2;
                        PreSList[i].DD.append(Distance);
                        PreSList[i].DM.append(Magnitude);
                        PreSList[i].DT.append(Time);
                        PreSList[i].numberofEvents = PreSList[i].numberofEvents + 1 ;
                        found = true;
                        continue;
                    }
                }
            }
            if(!found) {
                // Empty The current Cluster
                tempCluster = Empty;
                // Initialize the Another Cluster
                cluster++;
                tempCluster.ID=cluster;
                PreTim.append(tempEvent.EventTime);
                tempCluster.LatestTime = tempEvent.EventTime;
                tempCluster.FirstCoord = tempEvent.EventCord;
                tempCluster.numberofEvents = 1;
                tempCluster.Events.append(tempEvent);
                tempCluster.DD.append(0);
                tempCluster.DM.append(0);
                tempCluster.DT.append(0);
                PreSList.append(tempCluster);
                found = true;
                continue;
            }
        }

        // At this point we have possible cluters... let's look at their center:
        for (int i = 0; i < PreSList.size(); i++){
            QGeoCoordinate Average;
            double LatSum, LonSum, AltSum;
            double numEvents = PreSList.at(i).Events.size();
            LatSum = LonSum = AltSum = 0;
            for(int j = 0; j < numEvents; j++){
                double tLat, tLon, tAlt;
                tLat = PreSList.at(i).Events.at(j).EventCord.latitude();
                tLon = PreSList.at(i).Events.at(j).EventCord.longitude();
                tAlt = PreSList.at(i).Events.at(j).EventCord.altitude();
                LatSum += tLat;
                LonSum += tLon;
                AltSum += tAlt;
            }
            Average.setLatitude(LatSum/numEvents);
            Average.setLongitude(LonSum/numEvents);
            Average.setAltitude(AltSum/numEvents);
            PreLoc.append(Average);
        }

        // Lets set up all the clusters eventtime set to first event in the PreClusters
        cluster = 0;
        for (int i = 0; i < PreLoc.size(); i++){
            tempCluster = Empty;
            tempCluster.FirstCoord = PreLoc.at(i);
            tempCluster.ID = cluster;
            tempCluster.LatestTime = tempCluster.FirstTime = PreTim.at(i);
            Clusters.append(tempCluster);
            cluster++;
        }

        // Initialize some variables again
        Distance = 0;
        Magnitude = 0;
        Time = 0;
        found = false;

        // New and improved search function
        while(!Events.empty()){
            found = false;
            tempEvent = Events.takeLast();
            for(int i=0; i < Clusters.size(); i++){
                if (!found){
                    Time =  Clusters[i].LatestTime.toMSecsSinceEpoch() - tempEvent.EventTime.toMSecsSinceEpoch(); //Time diff in Msec
                    Distance =  Clusters[i].FirstCoord.distanceTo(tempEvent.EventCord)/1000; //Distance in Km
                    Magnitude =  fabs(Clusters[i].LatestMag - tempEvent.Magnitude);

                    if ( Time >= 0 && Time <= TimeT && Distance <= DistT ){
                        Clusters[i].Events.append(tempEvent);
                        Clusters[i].LatestTime = tempEvent.EventTime;
                        Clusters[i].LatestMag = (Clusters.at(i).LatestMag+tempEvent.Magnitude)/2;
                        Clusters[i].DD.append(Distance);
                        Clusters[i].DM.append(Magnitude);
                        Clusters[i].DT.append(Time);
                        Clusters[i].numberofEvents = Clusters[i].numberofEvents + 1 ;
                        found = true;
                        continue;
                    }
                }
            }
            if(!found) {
                // Empty The current Cluster
                tempCluster = Empty;
                // Initialize the Another Cluster
                cluster++;
                tempCluster.ID=cluster;
                tempCluster.LatestTime = tempCluster.FirstTime = tempEvent.EventTime;
                tempCluster.FirstCoord = tempEvent.EventCord;
                tempCluster.numberofEvents = 1;
                tempCluster.Events.append(tempEvent);
                tempCluster.DD.append(0);
                tempCluster.DM.append(0);
                tempCluster.DT.append(0);
                Clusters.append(tempCluster);
                found = true;
                continue;
            }
        }
    }
    createHist();
    removeEmpty();
}

void ClusterF::setMethod(bool Pass){
    twoPass = Pass;
}

void ClusterF::setDistT(double Dist){
    DistT = Dist;
}

void ClusterF::setMagT(double Mag){
    MagT = Mag;
}

void ClusterF::setTimeT(qint64 Time){
    TimeT = Time;
}

void ClusterF::setMinEV(int Min){
    MinEv = Min;
}

int ClusterF::getMinEV(){
    return MinEv;
}

void ClusterF::removeEmpty(){
    switch(MinEv){
    case 5:
        Filtered = QtConcurrent::filtered(Clusters,ClusterF::emptyfilter5);
        break;
    case 10:
        Filtered = QtConcurrent::filtered(Clusters,ClusterF::emptyfilter10);
        break;
    case 15:
        Filtered = QtConcurrent::filtered(Clusters,ClusterF::emptyfilter15);
        break;
    case 20:
        Filtered = QtConcurrent::filtered(Clusters,ClusterF::emptyfilter20);
        break;
    case 25:
        Filtered = QtConcurrent::filtered(Clusters,ClusterF::emptyfilter25);
        break;
    default:
        Filtered = QtConcurrent::filtered(Clusters,ClusterF::emptyfilter10);
        break;
    }
}

int ClusterF::getNumCluster(){
    return Clusters.length();
}

int ClusterF::getNumFCluster(){
    return Filtered.results().length();
}

QList<Cluster> ClusterF::getClusters(){
    return Filtered.results();
    //return Clusters;
}

bool ClusterF::emptyfilter5(const Cluster &t){
    if(t.numberofEvents >= 5)
        return true;
    else
        return false;
    return false;
}
bool ClusterF::emptyfilter10(const Cluster &t){
    if(t.numberofEvents >= 10)
        return true;
    else
        return false;
    return false;
}
bool ClusterF::emptyfilter15(const Cluster &t){
    if(t.numberofEvents >= 15)
        return true;
    else
        return false;
    return false;
}
bool ClusterF::emptyfilter20(const Cluster &t){
    if(t.numberofEvents >= 20)
        return true;
    else
        return false;
    return false;
}
bool ClusterF::emptyfilter25(const Cluster &t){
    if(t.numberofEvents >= 25)
        return true;
    else
        return false;
    return false;
}

void ClusterF::createHist(){
    for(int i = 0; i < Clusters.size(); i++){
        QDateTime F, L, C, CP;
        F = Clusters.at(i).LatestTime;
        L = Clusters.at(i).FirstTime;
        C = F;
        int sec = MagT*3600;
        CP = F.addSecs(sec);
        while(CP <= L){
            int count = 0;
            for (int j = 0; j < Clusters.at(i).Events.size(); j++){
                if(Clusters.at(i).Events.at(j).EventTime >= C && Clusters.at(i).Events.at(j).EventTime < CP){
                    count++;
                }
            }
            Clusters[i].HistogramData.append(count);
            C = CP;
            CP = CP.addSecs(sec);
        }
    }
}

double ClusterF::getMagT(){
    return MagT;
}
