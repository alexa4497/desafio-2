TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cancion.cpp \
        lista_favoritos.cpp \
        main.cpp \
        metricas.cpp \
        publicidad.cpp \
        reproductor.cpp \
        usuarios.cpp

HEADERS += \
    canciones.h \
    creditos.h \
    lista_favoritos.h \
    main.h \
    metricas.h \
    publicidad.h \
    reproductor.h \
    usuarios.h
