#include <fstream>
#include <iomanip>
#include <iostream>
#include "csv.hpp"
#include "vendor/cassandra.h"

int main() {
    std::cout << "Chugging..." << std::endl;

    // Setup and connect to cluster
    CassFuture* connect_future = NULL;
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();

    // Add contact points
    cass_cluster_set_contact_points(cluster, "127.0.0.1");

    // Provide the cluster object as configuration to connect the session
    connect_future = cass_session_connect(session, cluster);

    // open CSV file
    std::ifstream in("../data_huge.csv");
    if (!in.is_open())
    {
        std::cout << "File not found\n";
        return 1;
    }

    // start timer
    double duration;
    std::clock_t start;
    start = clock();

    // parse rows
    csv::CSVRow row;
    int dt_dd=0, dt_mo=0, dt_yyyy=0;
    int dt_HH=0, dt_MM=0, dt_SS=0, dt_FFF=0;

    while (in >> row)
    {
        sscanf(row[0].c_str(), "%d/%d/%d", &dt_mo, &dt_dd, &dt_yyyy);
        sscanf(row[1].c_str(), "%d:%d:%d.%03d", &dt_HH, &dt_MM, &dt_SS, &dt_FFF);

        std::stringstream ss_dd, ss_mm, ss_HH, ss_MM, ss_SS, ss_FFF;
        ss_dd  << std::setw(2) << std::setfill('0') << dt_dd;
        ss_mm  << std::setw(2) << std::setfill('0') << dt_mo;
        ss_HH  << std::setw(2) << std::setfill('0') << dt_HH;
        ss_MM  << std::setw(2) << std::setfill('0') << dt_MM;
        ss_SS  << std::setw(2) << std::setfill('0') << dt_SS;
        ss_FFF << std::setw(3) << std::setfill('0') << dt_FFF;

        // Posix time format "YYYY-MM-DD hh:mm:ss.fff"
        std::string ts = std::to_string(dt_yyyy) + '-' + ss_mm.str() + '-' + ss_dd.str() + ' ' +
                ss_HH.str() + ':' + ss_MM.str() + ':' + ss_SS.str() + '.' + ss_FFF.str();

//        std::cout << row[0] << "," << row[1] << "," << ts << "\n";
    }

    // end timer
    duration = (clock() - start) / (double) CLOCKS_PER_SEC;

    std::cout << "\n";
    std::cout << "Operation took: " << duration << "s\n";

    return 0;
}