TARGET = QtPositioning
QT = core-private
CONFIG += simd optimize_full

#INCLUDEPATH += ../3rdparty/poly2tri
INCLUDEPATH += ../3rdparty/clipper
INCLUDEPATH += ../3rdparty/clip2tri

QMAKE_DOCS = $$PWD/doc/qtpositioning.qdocconf
OTHER_FILES += configure.json doc/src/*.qdoc doc/src/plugins/*.qdoc   # show .qdoc files in Qt Creator

ANDROID_BUNDLED_JAR_DEPENDENCIES = \
    jar/Qt$${QT_MAJOR_VERSION}AndroidPositioning.jar:org.qtproject.qt.android.positioning.QtPositioning
ANDROID_PERMISSIONS += \
    android.permission.ACCESS_FINE_LOCATION
ANDROID_FEATURES += \
    android.hardware.location.gps
ANDROID_LIB_DEPENDENCIES = \
    plugins/position/libplugins_position_qtposition_android.so
MODULE_WINRT_CAPABILITIES_DEVICE += \
    location
MODULE_PLUGIN_TYPES = \
    position

PUBLIC_HEADERS += \
                    qgeoaddress.h \
                    qgeoareamonitorinfo.h \
                    qgeoareamonitorsource.h \
                    qgeoshape.h \
                    qgeorectangle.h \
                    qgeocircle.h \
                    qgeocoordinate.h \
                    qgeolocation.h \
                    qgeopositioninfo.h \
                    qgeopositioninfosource.h \
                    qgeosatelliteinfo.h \
                    qgeosatelliteinfosource.h \
                    qnmeapositioninfosource.h \
                    qgeopositioninfosourcefactory.h \
                    qpositioningglobal.h \
                    qgeopolygon.h \
                    qgeopath.h \

PRIVATE_HEADERS += \
                    qgeoaddress_p.h \
                    qgeoshape_p.h \
                    qgeorectangle_p.h \
                    qgeocircle_p.h \
                    qgeolocation_p.h \
                    qlocationutils_p.h \
                    qnmeapositioninfosource_p.h \
                    qgeocoordinate_p.h \
                    qgeopositioninfosource_p.h \
                    qdoublevector2d_p.h \
                    qdoublevector3d_p.h \
                    qwebmercator_p.h \
                    qpositioningglobal_p.h \
                    qdoublematrix4x4_p.h \
                    qgeopath_p.h \
                    qgeopolygon_p.h \
                    qgeocoordinateobject_p.h \
                    qgeopositioninfo_p.h \
                    qgeosatelliteinfo_p.h \
                    qgeosatelliteinfosource_p.h \
                    qclipperutils_p.h

SOURCES += \
            qgeoaddress.cpp \
            qgeoareamonitorsource.cpp \
            qgeoareamonitorinfo.cpp \
            qgeoshape.cpp \
            qgeorectangle.cpp \
            qgeocircle.cpp \
            qgeocoordinate.cpp \
            qgeolocation.cpp \
            qgeopositioninfo.cpp \
            qgeopositioninfosource.cpp \
            qgeosatelliteinfo.cpp \
            qgeosatelliteinfosource.cpp \
            qlocationutils.cpp \
            qnmeapositioninfosource.cpp \
            qgeopositioninfosourcefactory.cpp \
            qdoublevector2d.cpp \
            qdoublevector3d.cpp \
            qgeopath.cpp \
            qgeopolygon.cpp \
            qwebmercator.cpp \
            qdoublematrix4x4.cpp \
            qclipperutils.cpp \
            qgeocoordinateobject.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)

LIBS_PRIVATE += -L$$MODULE_BASE_OUTDIR/lib -lqt_clip2tri$$qtPlatformTargetSuffix()
