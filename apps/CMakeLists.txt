
add_executable(yall_app1 yall_app1.cpp)
target_link_libraries(yall_app1 PRIVATE yall)

add_executable(yall_app2 yall_app2.cpp)
target_link_libraries(yall_app2 PRIVATE yall)

if(${SANITIZE_YALL_APPS})
  target_compile_options(yall_app1 PRIVATE -fsanitize=address)
  target_link_options(yall_app1 PRIVATE -fsanitize=address)

  target_compile_options(yall_app2 PRIVATE -fsanitize=address)
  target_link_options(yall_app2 PRIVATE -fsanitize=address)
endif()
