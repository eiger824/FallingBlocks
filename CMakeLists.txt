cmake_minimum_required(VERSION 3.2)
project(FallingBlocks)
find_package(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})

# add definitions
ADD_DEFINITIONS(
  ${QT_DEFINITIONS} 
)

include_directories(
  src
  include
  images
)

set(HEADERS
  include/position.hpp
  include/matrix.hpp
  include/keyvalues.hpp
  include/passwordform.hpp
  include/gui.hpp
  include/logger.hpp
  )

set(SOURCES
  src/position.cpp
  src/matrix.cpp
  src/passwordform.cpp
  src/gui.cpp
  src/logger.cpp
  )

qt4_wrap_cpp(HEADERS_MOC ${HEADERS})

add_executable(fblocks
  src/main.cpp
  ${SOURCES}
  ${HEADERS_MOC}
  )

target_link_libraries(fblocks
  Qt4::QtGui
  Qt4::QtXml
  )

install(FILES
	images/blue.png
	images/green.png
	images/black.png
	images/pink.png
	images/orange.png
	images/red.png
	images/yellow.png
	images/white.png
	DESTINATION ${PROJECT_BINARY_DIR}/images
)
