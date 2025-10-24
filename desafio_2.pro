TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cancion.cpp \
        lista_favoritos.cpp \
        main.cpp \
        reproductor.cpp \
        usuarios.cpp

HEADERS += \
    canciones.h \
    lista_favoritos.h \
    main.h \
    reproductor.h \
    usuarios.h
