cmake_minimum_required(VERSION 3.0.0)
project(aesd-assignments)
# A list of all automated test source files
# At minimum it should include the files in the test/assignmentX directory
# Students can also add paths to their own test files using a ../ prefix to
# reference this working directory

set(CMAKE_C_FLAGS "-pthread")

set(AUTOTEST_SOURCES
    test/assignment1/Test_hello.c
    test/assignment1/Test_assignment_validate.c
#<<<<<<< HEAD
#<<<<<<< HEAD
#<<<<<<< HEAD
    ../student-test/assignment1/Test_validate_username.c
#=======
    test/assignment3/Test_systemcalls.c
#>>>>>>> assignments-base/assignment3-part-1
#=======
    test/assignment4/Test_threading.c
#>>>>>>> assignments-base/assignment4
#=======
#>>>>>>> assignments-base/assignment5
)
# A list of all files containing test code that is used for assignment validation
set(TESTED_SOURCE
    ../examples/autotest-validate/autotest-validate.c
#<<<<<<< HEAD
    ../examples/systemcalls/systemcalls.c
    ../examples/threading/threading.c
#=======
#>>>>>>> assignments-base/assignment5
)
add_subdirectory(assignment-autotest)
