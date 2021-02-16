# CMakeLists for json.h

# to use this in your library
# assuming you have the lib installed in "lib/json.h"
#
# add_subdirectory(lib/json.h)
# target_link_libraries(your-application-name PUBLIC json)

add_library(json INTERFACE)

target_include_directories(json INTERFACE include/)
