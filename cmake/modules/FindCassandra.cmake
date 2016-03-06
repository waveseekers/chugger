#
# Finds Cassandra library.
#
# System must have set CASSANDRA_ROOT
# environmental variable set
# (on linux we also try to guess if library is
# available).
#
# OUTPUT:
# 	Cassandra_FOUND - is set to YES if library was found
#	Cassandra_INCLUDE_DIR - include dir to Cassandra library
#	Cassandra_LIBRARIES - list of Cassandra libraries
#

include(FindPackageHandleStandardArgs)

find_path(Cassandra_INCLUDE_DIR
        NAMES cassandra.h
        PATHS /usr/include/cassandra ${PROJECT_SOURCE_DIR}/src/vendor/cassandra ENV CASSANDRA_ROOT
        PATH_SUFFIXES include
        DOC "Cassandra include directory")

find_library(Cassandra_LIBRARIES
        NAMES libcassandra.dylib
        PATHS /usr/lib /opt/cassandra ENV CASSANDRA_ROOT
        PATH_SUFFIXES lib
        DOC "Cassandra library directory")

find_package_handle_standard_args(Cassandra DEFAULT_MSG Cassandra_LIBRARIES Cassandra_INCLUDE_DIR)
mark_as_advanced(Cassandra_INCLUDE_DIR Cassandra_LIBRARIES)
