#include "fvfieldwizard.h"
#include <QFormLayout>
#include <iostream>

/////////// FVFieldWizard class impl ////////////////////////
FVFieldWizard::FVFieldWizard(std::vector<std::string> elems, std::vector<std::string> approx, QWidget *parent)
    : QWizard(parent)
 {
    this->elems = elems;
    this->approx = approx;

    addPage(new IntroPage);
    addPage(new FieldTypePage);
    addPage(new FETypePage(elems));
    addPage(new ApproxDegPage(approx));
//     addPage(new OutputFilesPage);
//     addPage(new ConclusionPage);

//     setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
//     setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("Fiver Field Wizard"));
 }



void FVFieldWizard::accept()
 {
//     QByteArray className = field("className").toByteArray();
//     QByteArray baseClass = field("baseClass").toByteArray();
//     QByteArray macroName = field("macroName").toByteArray();
//     QByteArray baseInclude = field("baseInclude").toByteArray();

     fieldType = field("fieldType").toInt();
     FEType = field("FEType").toInt();
     approxDeg = field("approxDeg").toInt();
     std::cout << "wybrano: \n" << fieldType << " | "  << FEType << " | " << approxDeg << std::endl;
//     QString implementation = field("implementation").toString();
     QDialog::accept();
 }


/////////// IntroPage class impl ////////////////////////
IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Field Definition Wizard"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel(tr("This wizard will help to determine "
                          "type of the field you try to load and the "
                          "type of finite element which is in use."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}



/////////// FieldTypePage class impl ////////////////////////
FieldTypePage::FieldTypePage(QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(tr("Field Type Definition"));
     setSubTitle(tr("Specify the type of field which you  "
                    "would like to load."));
//     setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

//     classNameLabel = new QLabel(tr("&Class name:"));
//     classNameLineEdit = new QLineEdit;
//     classNameLabel->setBuddy(classNameLineEdit);

     fieldTypeCombo = new QComboBox;
     fieldTypeCombo->addItem("select");
     fieldTypeCombo->addItem("scalar field");
     fieldTypeCombo->addItem("vector field");

//     baseClassLabel = new QLabel(tr("B&ase class:"));
//     baseClassLineEdit = new QLineEdit;
//     baseClassLabel->setBuddy(baseClassLineEdit);

//     qobjectMacroCheckBox = new QCheckBox(tr("Generate Q_OBJECT &macro"));

//     groupBox = new QGroupBox(tr("C&onstructor"));

//     registerField("className", classNameLineEdit);
     registerField("fieldType*", fieldTypeCombo);

     QFormLayout *layout = new QFormLayout;
     layout->addRow("field type: ", fieldTypeCombo);
     setLayout(layout);

 }


/////////// FETypePage class impl ////////////////////////
FETypePage::FETypePage(std::vector<std::string> elems, QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(tr("Finite Element Type Definition"));
     setSubTitle(tr("Specify the type of finite element which is  "
                    "in use."));
//     setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

     FETypeCombo = new QComboBox;
     FETypeCombo->addItem("select");
     for (std::vector<std::string>::iterator it = elems.begin() ; it != elems.end(); it++)
         FETypeCombo->addItem((*it).substr(0,(*it).size()-1).c_str());

     registerField("FEType*", FETypeCombo);

     QFormLayout *layout = new QFormLayout;
     layout->addRow("Finite Element type: ", FETypeCombo);
     setLayout(layout);

 }

/////////// ApproxDegPage class impl ////////////////////////
ApproxDegPage::ApproxDegPage(std::vector<std::string> approx,QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(tr("Degree of approximation definition"));
     setSubTitle(tr("Specify the degree of approximation." ));
//     setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

     approxCombo = new QComboBox;
     approxCombo->addItem("select");
     for (std::vector<std::string>::iterator it = approx.begin() ; it != approx.end(); it++)
         approxCombo->addItem((*it).substr(0,(*it).size()-1).c_str());

     registerField("approxDeg*", approxCombo);

     QFormLayout *layout = new QFormLayout;
     layout->addRow("Degree of approximation: ", approxCombo);
     setLayout(layout);

 }
