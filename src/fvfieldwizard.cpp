#include "fvfieldwizard.h"
#include <QFormLayout>
#include <iostream>

#include <finiteelementsclasses.h>
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
    this->cr = cr;

    addPage(new WizardPage(cr->types,cr->elems,cr->approx));

    setWindowTitle(tr("Fiver Field Wizard"));
 }



void FVFunctionSpaceWizard::accept()
 {
     fieldType = field("fieldType").toInt();
     FEType = field("FEType").toInt();
     approxDeg = field("approxDeg").toInt();
     fname = field("fileName").toString();
     isFinished = true;


     QDialog::accept();
 }


/////////// WizardPage class impl ////////////////////////
WizardPage::WizardPage(std::vector<std::string> types,std::vector<std::string> elems,
                       std::vector<std::string> approx, QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(tr("Field Definition Wizard"));
     setSubTitle(tr("Specify the "
                    "type of the field you try to load and the "
                    "type of finite element which is in use."));

     //Field type
     fieldTypeCombo = new QComboBox;
     fieldTypeCombo->addItem("select");
     for (std::vector<std::string>::iterator it = types.begin() ; it != types.end(); it++)
         fieldTypeCombo->addItem((*it).c_str());

     registerField("fieldType*", fieldTypeCombo);

     //FEType
     FETypeCombo = new QComboBox;
     FETypeCombo->addItem("select");
     for (std::vector<std::string>::iterator it = elems.begin() ; it != elems.end(); it++)
         FETypeCombo->addItem((*it).c_str());

     registerField("FEType*", FETypeCombo);

     //Degree of approx
     approxCombo = new QComboBox;
     approxCombo->addItem("select");
     for (std::vector<std::string>::iterator it = approx.begin() ; it != approx.end(); it++)
         approxCombo->addItem((*it).c_str());

     registerField("approxDeg*", approxCombo);

     fileLineEdit = new QLineEdit();
     fileLineEdit->setEnabled(false);
     fileButton = new QPushButton("&Choose file", this);
     connect(fileButton, SIGNAL(clicked()), this, SLOT(on_fileButton_clicked()));
     registerField("fileName*", fileLineEdit);

     QFormLayout *layout = new QFormLayout;
     layout->addRow("Field type: ", fieldTypeCombo);
     layout->addRow("Finite Element type: ", FETypeCombo);
     layout->addRow("Degree of approximation: ", approxCombo);
     layout->addRow(fileLineEdit, fileButton);
     setLayout(layout);

 }

void WizardPage::on_fileButton_clicked(){
//    fileLineEdit->setEnabled(true);
    filters[tr("1 - Xml field file (*.xml*)")] =  new FVOpenerXmlField();

    //    //////////////////////////////////////////////////////
    //         Show Dialog to Open File
    QStringList files;
    files = FVHelpers::openFiles(filters, selectedFilter);
    //    ///////////////////////////////////////////////////////

    fileLineEdit->insert(files.at(0));
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
