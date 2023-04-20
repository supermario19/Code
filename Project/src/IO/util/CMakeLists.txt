set(util_src
   Thread.cc
   CurrentThread.cc
   Mutex.cc
   Condition.cc
   CountDownLatch.cc
   Timestamp.cc
  )

add_library(util ${util_src})
target_link_libraries(util pthread rt)

install(TARGETS util DESTINATION lib/SCU/IO/util)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/SCU/IO/util)

if(SCU_BUILD_EXAMPLES)
    add_subdirectory(tests)
endif()
