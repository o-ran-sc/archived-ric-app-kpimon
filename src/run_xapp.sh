#! /bin/bash
THREADS=1
gNodeB=NYC123,ABC345,CDR331
#echo "Running ./kpi-xapp -g $gNodeB -t $THREADS";
./kpi-xapp -g $gNodeB -t $THREADS
