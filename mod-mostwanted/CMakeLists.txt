# Define the module name
set(MOD_MOSTWANTED_NAME "mod-mostwanted")

# Log that the module is being loaded
message("  -> Module: ${MOD_MOSTWANTED_NAME} loaded")

# Add the source directory to the include path
target_include_directories(modules PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add the source files to the build
target_sources(modules PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_mostwanted.cpp
)

# Handle the configuration file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/configs/modules/${MOD_MOSTWANTED_NAME}.conf")
    message("  -> Copying ${MOD_MOSTWANTED_NAME}.conf.dist to ${CMAKE_BINARY_DIR}/configs/modules/")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/conf/mod_mostwanted.conf.dist" DESTINATION "${CMAKE_BINARY_DIR}/configs/modules/")
endif()