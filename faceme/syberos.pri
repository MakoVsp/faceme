QML_IMPORT_PATH = $$[QT_INSTALL_QML]

APP_DIR = /data/apps
INSTALL_DIR = $$APP_DIR/com.syberos.faceme
LIB_DIR = /data/app-libs/com.syberos.faceme

INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../syberos_application

# release or debug
#QMAKE_CFLAGS += -g
#DEFINES += __DEBUG__
