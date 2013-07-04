#ifndef FVFIELDWIZARD_H
#define FVFIELDWIZARD_H

#include <QWizard>
#include <QVariant>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

#include <sstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <dolfin/function/FunctionSpace.h>
#include <configreader.h>
#include <fvopenerxmlfield.h>
#include <fvhelpers.h>

/////////// FVFieldWizard class ////////////////////////
class FVFunctionSpaceWizard : public QWizard
{
    Q_OBJECT
public:
    FVFunctionSpaceWizard(ConfigReader* cr, QWidget *parent = 0);

    void accept();
    void reject()
    {
        isFinished = false;
        QDialog::reject();
    }

    ConfigReader* cr;
    int fieldType;
    int FEType;
    int approxDeg;
    QString fname;
    bool isFinished;
    bool optionExists;

    dolfin::FunctionSpace* getFunctionSpace( dolfin::Mesh* mesh);
    ufc::finite_element* getElementByName(string name);
    ufc::dofmap* getDofByName(string name);

    bool isVector();
};


/////////// WizardPage class ////////////////////////
class WizardPage : public QWizardPage
 {
     Q_OBJECT

 public:
     WizardPage(std::vector<std::string> types, std::vector<std::string> elems,
                std::vector<std::string> approx, QWidget *parent = 0);

public slots:
     void on_fileButton_clicked();
private:
     QComboBox *fieldTypeCombo;
     QComboBox *FETypeCombo;
     QComboBox *approxCombo;

     QLineEdit *fileLineEdit;
     QPushButton *fileButton;

     QString selectedFilter;
     QMap< QString, FVOpener* > filters;

 };



#endif // FVFIELDWIZARD_H
