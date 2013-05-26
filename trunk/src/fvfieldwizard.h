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

#include <sstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <dolfin/function/FunctionSpace.h>
#include <configreader.h>

/////////// FVFieldWizard class ////////////////////////
class FVFunctionSpaceWizard : public QWizard
{
    Q_OBJECT
public:
    FVFunctionSpaceWizard(ConfigReader* cr, QWidget *parent = 0);
//    FVFunctionSpaceWizard(std::vector<std::string> elems , std::vector<std::string> approx, QWidget *parent = 0);

    void accept();
    void reject()
    {
        isFinished = false;
        QDialog::reject();
    }

//    std::vector<std::string> elems;
//    std::vector<std::string> approx;

    ConfigReader* cr;
    int fieldType;
    int FEType;
    int approxDeg;
    bool isFinished;
    bool optionExists;

    dolfin::FunctionSpace* getFunctionSpace( dolfin::Mesh* mesh);
    ufc::finite_element* getElementByName(string name);
    ufc::dofmap* getDofByName(string name);

    bool isVector();
};

/////////// IntroPage class ////////////////////////
class IntroPage : public QWizardPage
 {
     Q_OBJECT

 public:
     IntroPage(QWidget *parent = 0);

 private:
     QLabel *label;
 };

/////////// FieldTypePage class ////////////////////////
class FieldTypePage : public QWizardPage
 {
     Q_OBJECT

 public:
     FieldTypePage(std::vector<std::string> types, QWidget *parent = 0);

 private:
     QLabel *classNameLabel;
     QComboBox *fieldTypeCombo;
     QLineEdit *classNameLineEdit;
 };

/////////// FETypePage class ////////////////////////
class FETypePage : public QWizardPage
 {
     Q_OBJECT

 public:
     FETypePage(std::vector<std::string> elems, QWidget *parent = 0);

 private:
     QComboBox *FETypeCombo;
 };

/////////// ApproxDegPage class ////////////////////////
class ApproxDegPage : public QWizardPage
 {
     Q_OBJECT

 public:
     ApproxDegPage(std::vector<std::string> approx, QWidget *parent = 0);

 private:
     QComboBox *approxCombo;
 };


#endif // FVFIELDWIZARD_H
