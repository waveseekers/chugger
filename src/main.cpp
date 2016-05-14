#include <fstream>
#include <iomanip>
#include <iostream>
#include "vendor/cassandra/cassandra.h"
#include "csv.hpp"
#include "shared/time.hpp"

static const int BATCH_COUNT = 100;

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

    // Handle connection error
    if (cass_future_error_code(connect_future) != CASS_OK) {
        const char* message;
        size_t message_length;
        cass_future_error_message(connect_future, &message, &message_length);
        fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message_length, message);
        exit(EXIT_FAILURE);
    }

    // Prepare for batch inserts
    CassUuidGen* uuid_gen = cass_uuid_gen_new(); // UUID generator created once per app
    CassBatch* batch = cass_batch_new(CASS_BATCH_TYPE_LOGGED);

    // open CSV file
    std::ifstream in("../data.csv");
    if (!in.is_open())
    {
        std::cout << "File not found\n";
        exit(EXIT_FAILURE);
    }

    // start timer
    double duration;
    std::clock_t start;
    start = clock();

    // parse rows
    csv::CSVRow row;
    int count = 0;
    int dt_dd = 0, dt_mo = 0, dt_yyyy = 0;
    int dt_HH = 0, dt_MM = 0, dt_SS = 0, dt_FFF = 0;

    while (in >> row)
    {
        count++;
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

        // Instantiate new time object
//        ws::Time dt(ts);
//        unsigned long int ts_ms = dt.GetTimestampMilliseconds();
//        unsigned long int si_ms = dt.GetSessionIntervalMilliseconds();

        // Instantiate new tick object
        CassUuid uuid;
        std::string symbol = "6E";
//        cass_uuid_gen_from_time(uuid_gen, ts_ms, &uuid);

        if (count % BATCH_COUNT == 0)
        {
            printf("row %u: %s, %s, %s\n", count, row[0].c_str(), row[1].c_str(), ts.c_str());
//            std::cout << row[0] << "," << row[1] << "," << ts << "\n";
        }
    }

    // end timer
    duration = (clock() - start) / (double) CLOCKS_PER_SEC;

    std::cout << "\n";
    std::cout << "Operation took: " << duration << "s\n";

    // clean up
    cass_future_free(connect_future);
    cass_cluster_free(cluster);
    cass_session_free(session);
    cass_uuid_gen_free(uuid_gen);

    return 0;
}