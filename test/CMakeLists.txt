################################
## Project
################################
project(odnfc_test)

################################
## Dependencies
################################
include(../cmake/Findgoogletest.cmake)

add_subdirectory(cpabe)
add_subdirectory(lsss)
add_subdirectory(relic_integration)
add_subdirectory(serialization)
