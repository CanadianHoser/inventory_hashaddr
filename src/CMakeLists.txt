# https://r4nd0m6uy.ch/cmake-and-cpputest.html

# (1) Build a library with my application sources
set(CMAKE_C_STANDARD 11)

set(APP_LIB_SOURCE
    asset_tracker.c
)

find_package(OpenSSL REQUIRED)
add_library(${APP_LIB_NAME} ${APP_LIB_SOURCE})

# (2) Add main(..) to the application library to have something we can run
add_executable(${APP_NAME} main.c)

target_link_libraries(${APP_NAME} ${APP_LIB_NAME} OpenSSL::SSL)
