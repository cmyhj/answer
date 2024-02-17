cmake_minimum_required(VERSION 3.8)
project(answer)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

ament_auto_add_executable(answer_node src/answer_node.cpp include/answer/answer.cpp
        include/answer/image_processing.cpp)
target_compile_features(answer_node PUBLIC c_std_99 cxx_std_17)
include_directories(include)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS ${PROJECT_NAME}_node
        DESTINATION lib/${PROJECT_NAME})
ament_package()
# Install launch files.
install(DIRECTORY
        launch
        DESTINATION share/${PROJECT_NAME}/
)
install(
        PROGRAMS
        launch/answer.py
        DESTINATION lib/${PROJECT_NAME}
)