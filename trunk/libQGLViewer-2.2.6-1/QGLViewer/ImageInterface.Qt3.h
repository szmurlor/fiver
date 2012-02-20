/****************************************************************************
** Form interface generated from reading ui file 'ImageInterface.Qt3.ui'
**
** Created: pon wrz 15 19:17:23 2008
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IMAGEINTERFACE_H
#define IMAGEINTERFACE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QSpinBox;
class QCheckBox;
class QPushButton;

class ImageInterface : public QDialog
{
    Q_OBJECT

public:
    ImageInterface( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ImageInterface();

    QLabel* textLabel1;
    QSpinBox* imgWidth;
    QLabel* textLabel2;
    QSpinBox* imgHeight;
    QLabel* textLabel3;
    QSpinBox* imgQuality;
    QLabel* textLabel3_2;
    QSpinBox* oversampling;
    QCheckBox* whiteBackground;
    QCheckBox* expandFrustum;
    QPushButton* pushButton1;
    QPushButton* pushButton2;

protected:
    QVBoxLayout* ImageInterfaceLayout;
    QHBoxLayout* layout5;
    QHBoxLayout* layout2;
    QSpacerItem* spacer1;
    QHBoxLayout* layout2_2;
    QSpacerItem* spacer1_2;
    QHBoxLayout* layout4;
    QSpacerItem* spacer3;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // IMAGEINTERFACE_H
