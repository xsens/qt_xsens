# Generated from demos.pro.

if(TARGET Qt::Quick)
    add_subdirectory(samegame)
    add_subdirectory(calqlatr)
    add_subdirectory(clocks)
    add_subdirectory(tweetsearch)
    add_subdirectory(maroon)
    add_subdirectory(photosurface)
    add_subdirectory(stocqt)
endif()
if(TARGET Qt::Quick AND TARGET Qt::QuickControls2)
    add_subdirectory(coffee)
    if(ANDROID OR IOS)
        add_subdirectory(hangman)
    endif()
endif()
if(TARGET Qt::Quick AND TARGET Qt::Network AND TARGET Qt::QmlXmlListModel)
    add_subdirectory(rssnews)
    add_subdirectory(photoviewer)
endif()

