The goal of the project is to develop an easy to install and use, and powerful application for scientific visualisation of simulation results obtained from FEM/BEM methods.

The project is internally developed by the Division of Theory of Electrical Engineering and Information Systems at the Warsaw University of Technology, Poland.


## Short build instructions ##

1. Requirements
  * Qt 4.x
  * g++
  * Mesa or other OpenGL library
  * libQGLViewer
2. To build the project simply run following commands:
```
svn checkout https://fiver.googlecode.com/svn/trunk/ fiver
cd fiver
qmake
make
sudo make install
```
Files: README, INSTALL contain detailed instructions.

3. Specific instructions for Linux distibutions:
  * for Fedora 16 instead of qmake use commands:
    * `sudo yum install LIBQGLViewer-devel`
    * `QGLVIEWER_LIB=QGLViewer qmake`