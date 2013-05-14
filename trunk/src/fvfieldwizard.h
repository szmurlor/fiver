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

/////////// FVFieldWizard class ////////////////////////
class FVFieldWizard : public QWizard
{
    Q_OBJECT
public:
    FVFieldWizard(std::vector<std::string> elems , std::vector<std::string> approx, QWidget *parent = 0);

    void accept();

    std::vector<std::string> elems;
    std::vector<std::string> approx;

    int fieldType;
    int FEType;
    int approxDeg;

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
     FieldTypePage(QWidget *parent = 0);

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
