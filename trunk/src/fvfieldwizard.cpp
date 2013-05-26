#include "fvfieldwizard.h"
#include <QFormLayout>
#include <iostream>

//#include <TetraScalar.h>
#include <Tetra.h>
#include <dolfin/mesh/Mesh.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/fem/DofMap.h>
#include <dolfin/la/GenericVector.h>
#include <dolfin/common/Array.h>

/////////// FVFieldWizard class impl ////////////////////////
//FVFunctionSpaceWizard::FVFunctionSpaceWizard(std::vector<std::string> elems, std::vector<std::string> approx, QWidget *parent)
FVFunctionSpaceWizard::FVFunctionSpaceWizard(ConfigReader* cr, QWidget *parent)
    : QWizard(parent)
 {
//    this->elems = elems;
//    this->approx = approx;


    this->cr = cr;

    addPage(new IntroPage);
    addPage(new FieldTypePage(cr->types));
    addPage(new FETypePage(cr->elems));
    addPage(new ApproxDegPage(cr->approx));

    setWindowTitle(tr("Fiver Field Wizard"));
 }



void FVFunctionSpaceWizard::accept()
 {
     fieldType = field("fieldType").toInt();
     FEType = field("FEType").toInt();
     approxDeg = field("approxDeg").toInt();

     isFinished = true;
     QDialog::accept();
 }


/////////// IntroPage class impl ////////////////////////
IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Field Definition Wizard"));

    label = new QLabel(tr("This wizard will help to determine "
                          "type of the field you try to load and the "
                          "type of finite element which is in use."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}



/////////// FieldTypePage class impl ////////////////////////
FieldTypePage::FieldTypePage(std::vector<std::string> types, QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(tr("Field Type Definition"));
     setSubTitle(tr("Specify the type of field which you  "
                    "would like to load."));

     fieldTypeCombo = new QComboBox;
     fieldTypeCombo->addItem("select");
     for (std::vector<std::string>::iterator it = types.begin() ; it != types.end(); it++)
         fieldTypeCombo->addItem((*it).c_str());

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

     FETypeCombo = new QComboBox;
     FETypeCombo->addItem("select");
     for (std::vector<std::string>::iterator it = elems.begin() ; it != elems.end(); it++)
         FETypeCombo->addItem((*it).c_str());

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

     approxCombo = new QComboBox;
     approxCombo->addItem("select");
     for (std::vector<std::string>::iterator it = approx.begin() ; it != approx.end(); it++)
         approxCombo->addItem((*it).c_str());

     registerField("approxDeg*", approxCombo);

     QFormLayout *layout = new QFormLayout;
     layout->addRow("Degree of approximation: ", approxCombo);
     setLayout(layout);

}

dolfin::FunctionSpace * FVFunctionSpaceWizard::getFunctionSpace( dolfin::Mesh* mesh)
{
    ufc::finite_element* el;
    ufc::dofmap* dofmap;
    optionExists=false;

//    std::cout << "Vals in wizard: \"" <<fieldType << "\" \"" << FEType<< "\" \"" << approxDeg << "\"" << std::endl;

    int FT = fieldType, FET=FEType, AD = approxDeg;

    for (int i=0; i < cr->map.size(); i++ ){
        vector<string> m = cr->map[i];
        if (cr->types[FT-1] == m[0] && cr->elems[FET-1] == m[1] && cr->approx[AD-1] == m[2] ){
            el =getElementByName(m[3]);
            dofmap = getDofByName(m[3]);
            optionExists=true;
            break;
        }
    }

    if (optionExists){
        dolfin::FiniteElement* elem = new dolfin::FiniteElement(boost::shared_ptr<ufc::finite_element>(el) );
        dolfin::DofMap* dof = new dolfin::DofMap( boost::shared_ptr<ufc::dofmap>(dofmap) , *mesh );
        dolfin::FunctionSpace* V = new dolfin::FunctionSpace(
                    boost::shared_ptr<dolfin::Mesh>(mesh),
                    boost::shared_ptr<dolfin::FiniteElement>(elem),
                    boost::shared_ptr<dolfin::DofMap>(dof)
                    );
        return V;
    } else {
        std::cout << "There is no configuration for options: \""<< cr->types[FT-1] << "\" \""<<
                     cr->elems[FET-1]  << "\" \"" << cr->approx[AD-1] <<"\""<< std::endl;
    }
    return NULL;
}

ufc::finite_element* FVFunctionSpaceWizard::getElementByName(std::string name)
{
    if (name == "tetrascalar0"){
        return new tetrascalar0();
    } else if ( name == "tetrascalar1") {
        return new tetrascalar1();
    } else if ( name == "tetravector0") {
        return new tetravector0();
    } else if ( name == "tetravector1") {
        return new tetravector1();
    } else {
        std::cerr << "There is no class named: " << name << std::endl;
        return NULL;
    }
}

ufc::dofmap* FVFunctionSpaceWizard::getDofByName(std::string name)
{
    if (name == "tetrascalar0") {
        return new tetrascalar_dof0();
    } else if (name ==  "tetrascalar1" ) {
        return new tetrascalar_dof1();
    } else if (name ==  "tetravector0" ) {
        return new tetravector_dof0();
    } else if (name ==  "tetravector1" ) {
        return new tetravector_dof1();
    } else {
        std::cerr << "There is no class named: " << name << std::endl;
        return NULL;
    }
}

bool FVFunctionSpaceWizard::isVector()
{
    return cr->types[fieldType-1] == "vector";
}
