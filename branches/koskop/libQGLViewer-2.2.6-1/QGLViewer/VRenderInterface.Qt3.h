/****************************************************************************
** Form interface generated from reading ui file 'VRenderInterface.Qt3.ui'
**
** Created: pon wrz 15 19:17:23 2008
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef VRENDERINTERFACE_H
#define VRENDERINTERFACE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QCheckBox;
class QLabel;
class QComboBox;
class QPushButton;

class VRenderInterface : public QDialog
{
    Q_OBJECT

public:
    VRenderInterface( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~VRenderInterface();

    QCheckBox* includeHidden;
    QCheckBox* cullBackFaces;
    QCheckBox* blackAndWhite;
    QCheckBox* colorBackground;
    QCheckBox* tightenBBox;
    QLabel* sortLabel;
    QComboBox* sortMethod;
    QPushButton* SaveButton;
    QPushButton* CancelButton;

protected:
    QVBoxLayout* VRenderInterfaceLayout;
    QSpacerItem* spacer1;
    QHBoxLayout* layout3;
    QHBoxLayout* Layout4;

protected slots:
    virtual void languageChange();

};

#endif // VRENDERINTERFACE_H
