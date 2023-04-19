add_executable(Thread_test Thread_test.cc)
target_link_libraries(Thread_test util)

add_executable(CurrentThread_test CurrentThread_test.cc)
target_link_libraries(CurrentThread_test util)

add_executable(Mutex_test Mutex_test.cc)
target_link_libraries(Mutex_test util)

add_executable(Condition_test Condition_test.cc)
target_link_libraries(Condition_test util)

add_executable(CountDownLatch_test CountDownLatch_test.cc)
target_link_libraries(CountDownLatch_test util)