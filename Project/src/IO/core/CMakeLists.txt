set(core_src
    Channel.cc
    EventLoop.cc
    Poller.cc
    EpollPoller.cc
    DefaultPoller.cc
  )

add_library(core ${core_src})
target_link_libraries(core pthread rt util)

install(TARGETS core DESTINATION lib/SCU/IO/core)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/SCU/IO/core)

if(SCU_BUILD_EXAMPLES)
    add_subdirectory(tests)
endif()
