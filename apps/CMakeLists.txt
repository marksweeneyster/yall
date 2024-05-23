
add_executable(yall_app1 yall_app1.cpp)
add_executable(yall_app2 yall_app2.cpp)
add_executable(yall_app3 yall_app3.cpp)

set(YALL_APPS_TARGETS yall_app1 yall_app2 yall_app3)

foreach (yall_app IN LISTS YALL_APPS_TARGETS)
  target_link_libraries(${yall_app} PRIVATE yall)
  if (${SANITIZE_YALL_APPS})
    target_compile_options(${yall_app} PRIVATE -fsanitize=address)
    target_link_options(${yall_app} PRIVATE -fsanitize=address)
  endif ()
endforeach ()