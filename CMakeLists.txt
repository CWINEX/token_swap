cmake_minimum_required(VERSION 3.5)
project(ico VERSION 1.0.0)

find_package(eosio.cdt)

### Generate the wasm and abi
add_contract( ico ico ico.cpp )

target_include_directories(${CMAKE_PROJECT_NAME}.wasm PUBLIC 
                           ${CMAKE_CURRENT_SOURCE_DIR}/lib/eosio.token/include/)

### add the path to where the ricardian contracts/clauses are found
target_compile_options( ico.wasm PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR} )
