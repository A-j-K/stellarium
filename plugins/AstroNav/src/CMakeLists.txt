INCLUDE_DIRECTORIES(
     .
     #gui
     ${CMAKE_BINARY_DIR}/plugins/AstroNav/src
     #${CMAKE_BINARY_DIR}/plugins/AstroNav/src/gui
)

LINK_DIRECTORIES(${BUILD_DIR}/src)

SET(AstroNav_SRCS
     AstroNav.hpp
     AstroNav.cpp
     AstroNavPlugin.hpp
     AstroNavPlugin.cpp
     Almanac/AlmanacStrategy.hpp
     Almanac/AlmanacStrategy.cpp
     Almanac/AlmanacAngloAmerican.hpp
     Almanac/AlmanacFrench.hpp
     Almanac/AlmanacRussian.hpp
     #AstroNavStar.hpp
     #AstroNavStar.cpp
)

#SET(ExoplanetsDialog_UIS
#     gui/exoplanetsDialog.ui
#)
#QT5_WRAP_UI(ExoplanetsDialog_UIS_H ${ExoplanetsDialog_UIS})

SET(AstroNav_RES ../resources/AstroNav.qrc)
QT5_ADD_RESOURCES(AstroNav_RES_CXX ${AstroNav_RES})

ADD_LIBRARY(AstroNav-static STATIC ${AstroNav_SRCS} ${AstroNav_RES_CXX})
TARGET_LINK_LIBRARIES(AstroNav-static Qt5::Core Qt5::Widgets)
SET_TARGET_PROPERTIES(AstroNav-static PROPERTIES OUTPUT_NAME "AstroNav")
SET_TARGET_PROPERTIES(AstroNav-static PROPERTIES COMPILE_FLAGS "-DQT_STATICPLUGIN")
ADD_DEPENDENCIES(AllStaticPlugins AstroNav-static)

SET_TARGET_PROPERTIES(AstroNav-static PROPERTIES FOLDER "plugins/AstroNav")

