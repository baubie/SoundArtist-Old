CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT(SoundArtist CXX)
FIND_PACKAGE(wxWidgets COMPONENTS core base adv propgrid REQUIRED)
INCLUDE( ${wxWidgets_USE_FILE} )
SET(SRCS soundartist.cpp gui.cpp)

IF (APPLE) 
    ADD_EXECUTABLE(soundartist MACOSX_BUNDLE ${SRCS} ) 
ELSE (APPLE) 
    ADD_EXECUTABLE(soundartist ${SRCS} ) 
ENDIF (APPLE) 


TARGET_LINK_LIBRARIES(soundartist ${wxWidgets_LIBRARIES})
