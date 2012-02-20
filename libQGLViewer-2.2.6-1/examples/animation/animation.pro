# The <code>animate()</code> function illustrated by a water particle simulation.

# When animation is activated (the Return key toggles animation), the <code>animate()</code> and
# then the <code>draw()</code> functions are called in an infinite loop.

# You can tune the frequency of your animation (default is 25Hz) using
# <code>setAnimationInterval()</code>. The frame rate will then be fixed, provided that your
# animation loop function is fast enough.

TEMPLATE = app
TARGET   = animation
CONFIG  += qt opengl warn_on release thread

HEADERS  = animation.h
SOURCES  = animation.cpp main.cpp

# The rest of this configuration file is pretty complex since it tries to automatically
# detect system paths and configuration. In your applications, you can probably simply use:
#unix:LIBS *= -lQGLViewer
#win32:LIBS *= QGLViewer226.lib (with Visual 6, use QGLViewer2.lib or QGLViewer.lib instead)

#Windows Qt 2.3 users should uncomment the next 2 lines and remove all the remaining lines:
#DEFINES *= QT_DLL QT_THREAD_SUPPORT
#LIBS *= QGLViewer.Qt2.3.lib

QT_VERSION=$$[QT_VERSION]

contains( QT_VERSION, "^4.*" ) {
  QT *= xml opengl
} else {
  CONFIG *= thread
}

#                       Unix configuration
# See doc/installUnix.html and doc/examples/index.html for details.
# Same INCLUDE_DIR and LIB_DIR parameters than for the make install.
unix {
  isEmpty( PREFIX ) {
    PREFIX=/usr
  }

  # INCLUDE_DIR
  isEmpty( INCLUDE_DIR ) {
    INCLUDE_DIR = $${PREFIX}/include

    !exists( $${INCLUDE_DIR}/QGLViewer/qglviewer.h ) {
      exists( ../../QGLViewer/qglviewer.h ) {
        message( Using ../.. as INCLUDE_DIR )
        INCLUDE_DIR = ../..
      }
    }
  }

  !exists( $${INCLUDE_DIR}/QGLViewer/qglviewer.h ) {
    message( Unable to find QGLViewer/qglviewer.h in $${INCLUDE_DIR} )
    error( Use qmake INCLUDE_DIR=Path/To/QGLViewerHeaderFiles )
  }

  # LIB_NAME
  LIB_NAME = libQGLViewer*.so*
  macx|darwin-g++ {
    LIB_NAME = libQGLViewer*.$${QMAKE_EXTENSION_SHLIB}
  }
  hpux {
    LIB_NAME = libQGLViewer*.sl*
  }

  !isEmpty( QGLVIEWER_STATIC ) {
    LIB_NAME = libQGLViewer*.a
  }

  # LIB_DIR
  isEmpty( LIB_DIR ) {
    LIB_DIR = $${PREFIX}/lib

    !exists( $${LIB_DIR}/$${LIB_NAME} ) {
      exists( ../../QGLViewer/$${LIB_NAME} ) {
        message( Using ../../QGLViewer as LIB_DIR )
        macx|darwin-g++ {
          message( You should add the path to "../../QGLViewer" to your DYLD_LIBRARY_PATH variable )
        } else {
          message( You should add the path to "../../QGLViewer" to your LD_LIBRARY_PATH variable )
        }
        message( See the "Compilation" section in doc/examples/index.html for details )
        LIB_DIR = ../../QGLViewer
      }
    }
  }

  !exists( $${LIB_DIR}/$${LIB_NAME} ) {
    message( Unable to find $${LIB_NAME} in $${LIB_DIR} )
    error( You should run qmake LIB_DIR=Path/To/$${LIB_NAME} )
  }

  # Paths were correctly detected
  INCLUDEPATH *= $${INCLUDE_DIR}
  DEPENDPATH  *= $${INCLUDE_DIR}
  isEmpty( QGLVIEWER_STATIC ) {
    LIBS *= -L$${LIB_DIR} -lQGLViewer
  } else {
    LIBS *= $${LIB_DIR}/$${LIB_NAME}
  }

  macx {
    LIBS *= -lobjc
    CONFIG -= thread
  }

  # Remove debugging options
  release:QMAKE_CFLAGS_RELEASE -= -g
  release:QMAKE_CXXFLAGS_RELEASE -= -g

  # Intermediate files are created in an hidden folder
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


#                    Windows configuration.
win32 {
  # Various compilation flags
  !win32-g++ {
    QMAKE_CXXFLAGS = -TP -G6 -GR -Zi
    win32-msvc {
      QMAKE_CXXFLAGS *= -GX
    } else {
      QMAKE_CXXFLAGS *= -EHs
    }
  }

  # Use the Qt DLL version
  DEFINES *= QT_DLL QT_THREAD_SUPPORT

  !isEmpty( QGLVIEWER_STATIC ) {
    DEFINES *= QGLVIEWER_STATIC
  }

  win32-g++ {
    LIB_FILE = libQGLViewer*2.a
  } else {
    LIB_FILE = QGLViewer*.lib
  }

  # Compilation from zip file : libQGLViewer is in ../..
  exists( ../../QGLViewer ) {
    exists( ../../QGLViewer/qglviewer.h ) {
      INCLUDEPATH *= ../..
    }

    exists( ../../QGLViewer/Debug ) {
      exists( ../../QGLViewer/Debug/$${LIB_FILE} ) {
        LIB_PATH = ../../QGLViewer/Debug
      }
    }

    exists( ../../QGLViewer/Release ) {
      exists( ../../QGLViewer/Release/$${LIB_FILE} ) {
        LIB_PATH = ../../QGLViewer/Release
      }
    }

    exists( ../../QGLViewer/$${LIB_FILE} ) {
      LIB_PATH = ../../QGLViewer
    }
  }

  exists( $${LIB_PATH}/QGLViewer226.lib ) {
    LIBS *= $${LIB_PATH}/QGLViewer226.lib
  } else {
    exists( $${LIB_PATH}/QGLViewer2.lib ) {
      LIBS *= $${LIB_PATH}/QGLViewer2.lib
    } else {
      exists( $${LIB_PATH}/QGLViewer.lib ) {
        LIBS *= $${LIB_PATH}/QGLViewer.lib
      } else {
        exists( $${LIB_PATH}/libQGLViewer2.a ) {
          LIBS *= -L$${LIB_PATH} -lQGLViewer2
        } else {
          exists( $${LIB_PATH}/libQGLViewerd2.a ) {
            LIBS *= -L$${LIB_PATH} -lQGLViewerd2
          } else {
            error( Unable to find $${LIB_FILE}. )
	  }
	}
      }
    }
  }
}
