
include_directories( ${Boost_INCLUDE_DIRS} )

add_executable( playgroundpp main.cpp )

target_link_libraries(playgroundpp 
    PRIVATE ${Boost_LIBRARIES}
    PRIVATE pthread
    PRIVATE CLI11::CLI11
)
