cmake_minimum_required(VERSION 3.10)
project(libOTe_to_test)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(/usr/local/include)


link_directories(/usr/local/lib)

add_executable(test_kkrt_ot test_kkrt_ot.cpp)
# add_executable(sender sender.cpp)
# add_executable(receiver receiver.cpp)

find_package(libOTe REQUIRED)
# find_package(libOTe_Tests REQUIRED)
# find_package(cryptoTools REQUIRED)
# find_package(libOTe REQUIRED 
#     COMPONENTS
#         std_20
        
#         Debug
#         Release
#         RelWithDebInfo

#         boost
#         relic
#         sodium
#         bitpolymul
#         openssl
#         circuits

#         sse
#         avx
#         asan
#         pic
#         no_sse
#         no_avx
#         no_asan
#         no_pic

#         simplestot
#         simplestot_asm
#         mrr
#         mrr_twist
#         mr
#         mr_kyber
#         kos
#         iknp
#         silentot
#         softspoken_ot
#         delta_kos
#         silent_vole
#         oos
#         kkrt

#         foleage

#         regular_dpf
#         ternary_dpf
#         sparse_dpf
# )

target_link_libraries(test_kkrt_ot oc::libOTe oc::libOTe_Tests oc::cryptoTools pthread)
# target_link_libraries(sender oc::libOTe oc::libOTe_Tests oc::cryptoTools pthread)
# target_link_libraries(receiver oc::libOTe oc::libOTe_Tests oc::cryptoTools pthread)

# define the debug mode
# add_definitions(-DDEBUG)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/test)
add_executable(test 
    test.cpp
)
# target_link_directories(test oc::libOTe oc::libOTe_Tests oc::cryptoTools pthread)