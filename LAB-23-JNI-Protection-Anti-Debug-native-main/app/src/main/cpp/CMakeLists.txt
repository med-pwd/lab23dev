cmake_minimum_required(VERSION 3.22.1)

project("jni_antidebug")

# Add the native library
add_library(
        native-lib
        SHARED
        native-lib.cpp
)

# Find the Android log library
find_library(
        log-lib
        log
)

# Link the libraries
target_link_libraries(
        native-lib
        ${log-lib}
)
