#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "vendor/csv/csv.hpp"
#include "vendor/cassandra/cassandra.h"
#include "shared/datalib.hpp"
#include "shared/time.hpp"
#include "shared/tick.hpp"

static const int BATCH_SIZE = 100;
static const bool CSV_SKIP_FIRST_LINE = true;

void
CassPrintError(CassFuture* future) {
    const char* message;
    size_t message_length;
    cass_future_error_message(future, &message, &message_length);
    fprintf(stderr, "Error: %.*s\n", (int)message_length, message);
}

CassError
PrepareTickInsert(CassSession* session,
                  const CassPrepared** prepared)
{
    CassError rc = CASS_OK;
    CassStatement* statement = NULL;
    CassFuture* future = NULL;
    const char* query;
    query = "INSERT INTO waveseeker.ticks (symbol, interval_ms, timestamp_ms, price, volume) VALUES (?, ?, ?, ?, ?);";

    future = cass_session_prepare(session, query);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        CassPrintError(future);
    } else {
        *prepared = cass_future_get_prepared(future);
    }
    cass_future_free(future);

    return rc;
}

CassError
BatchInsertWithPrepared(CassSession* session,
                        const CassPrepared* prepared,
                        std::array <ws::Tick, BATCH_SIZE>* ticks)
{
    CassError rc = CASS_OK;
    CassFuture* future = NULL;
    CassBatch* batch = cass_batch_new(CASS_BATCH_TYPE_LOGGED);
    const ws::Tick* t;

    for(auto& tick : *ticks)
    {
        CassStatement* statement = cass_prepared_bind(prepared);
        cass_statement_bind_string(statement, 0, tick.getSymbolAsChar());
        cass_statement_bind_int64(statement, 1, tick.getSessionInterval());
        cass_statement_bind_int64(statement, 2, tick.getTimestamp());
        cass_statement_bind_float(statement, 3, tick.getPrice());
        cass_statement_bind_float(statement, 4, tick.getVolume());

        cass_batch_add_statement(batch, statement);
        cass_statement_free(statement);
    }

    future = cass_session_execute_batch(session, batch);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        CassPrintError(future);
    }

    cass_future_free(future);
    cass_batch_free(batch);

    return rc;
}

CassError
InsertWithPrepared(CassSession* session,
                   const CassPrepared* prepared,
                   ws::Tick* tick)
{
    CassError rc = CASS_OK;
    CassStatement* statement = NULL;
    CassFuture* future = NULL;

    statement = cass_prepared_bind(prepared);
    cass_statement_bind_string(statement, 0, tick->getSymbolAsChar());
    cass_statement_bind_int64(statement, 1, tick->getSessionInterval());
    cass_statement_bind_int64(statement, 2, tick->getTimestamp());
    cass_statement_bind_float(statement, 3, tick->getPrice());
    cass_statement_bind_float(statement, 4, tick->getVolume());

    future = cass_session_execute(session, statement);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        CassPrintError(future);
    }

    cass_future_free(future);
    cass_statement_free(statement);

    return rc;
}

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
    const CassPrepared* prepared = NULL;

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
    // format: date, time, price, volume
    std::array <ws::Tick, BATCH_SIZE> ticks;
    csv::CSVRow row;
    int count = 0;
    int tickCount = 0;
    int dt_dd = 0, dt_mo = 0, dt_yyyy = 0;
    int dt_HH = 0, dt_MM = 0, dt_SS = 0, dt_FFF = 0;

    if (CSV_SKIP_FIRST_LINE)
    {
        in >> row;
    }

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
        ws::Time dt(ts);
        unsigned long int ts_ms = dt.GetTimestampMilliseconds();
        unsigned long int si_ms = dt.GetSessionIntervalMilliseconds();

        // Instantiate new tick object
        CassUuid uuid;
        std::string symbol = "6E";
        cass_uuid_gen_from_time(uuid_gen, ts_ms, &uuid);
        ws::Tick tick(symbol, ts_ms, std::stof(row[2]), std::stof(row[3]));

        // Push tick to ticks array for batch insert
        if (tickCount > BATCH_SIZE - 1)
        {
            tickCount = 0;
            if (PrepareTickInsert(session, &prepared) == CASS_OK) {
                BatchInsertWithPrepared(session, prepared, &ticks);
                cass_prepared_free(prepared);
                printf("row %u: %s, %s, %s\n", count, row[0].c_str(), row[1].c_str(), ts.c_str());
            }
        }

        // Check valid range for ticks array
        if (tickCount >=0 && tickCount < BATCH_SIZE)
        {
            ticks[tickCount] = tick;
            tickCount++;
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