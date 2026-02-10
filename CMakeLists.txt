# Setup project
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(BoneOscopyProject)

#---------------------------------------------------------------------------

# C++ flags

## Minimun C++ standard to compile
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Deactivate specific extensions
set(CMAKE_CXX_EXTENSIONS OFF)

## 03 Optimization level 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

#---------------------------------------------------------------------------

# G4 flags

add_definitions(-DG4MULTITHREADED)

#---------------------------------------------------------------------------

# Find G4 packages, activating all available UI and Vis drivers by default
# Set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#---------------------------------------------------------------------------
# Find ROOT packages

find_package(ROOT REQUIRED COMPONENTS Core)

if(ROOT_FOUND)
    include(${ROOT_USE_FILE})
else()
    message(FATAL_ERROR "Geant4 not found!")
endif() 

#---------------------------------------------------------------------------

# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#---------------------------------------------------------------------------

# Locate sources and headers for this project

file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


# Add the executable, and link it to the Geant4 libraries
#
add_executable(simulation main.cc ${sources} ${headers})
target_link_libraries(simulation ${Geant4_LIBRARIES})

#---------------------------------------------------------------------------
# Copy all scripts to the build directory. Make possible run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB1_SCRIPTS
  )

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()



#----------------------------------------------------------------------------
# Create costum target for build dependency

add_custom_target(BoneOscopyProject DEPENDS simulation)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

install(TARGETS simulation 
        DESTINATION ${PROJECT_SOURCE_DIR}/bin 
        CONFIGURATIONS Release Debug
)

install(FILES ${simulation_SCRIPTS} DESTINATION ${PROJECT_SOURCE_DIR}/macros)
