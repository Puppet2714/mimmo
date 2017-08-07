/*---------------------------------------------------------------------------*\
 *
 *  mimmo
 *
 *  Copyright (C) 2015-2017 OPTIMAD engineering Srl
 *
 *  -------------------------------------------------------------------------
 *  License
 *  This file is part of mimmo.
 *
 *  mimmo is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License v3 (LGPL)
 *  as published by the Free Software Foundation.
 *
 *  mimmo is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mimmo. If not, see <http://www.gnu.org/licenses/>.
 *
\*---------------------------------------------------------------------------*/

#include "GenericInput.hpp"
#include "Operators.hpp"
#include <fstream>

using namespace std;
namespace mimmo {

/*!
 * Default constructor of GenericInput.
 * \param[in] readFromFile True if the object reads the values from file (default value false).
 * \param[in] csv True if the input file is a csv format file (default value false).
 */
GenericInput::GenericInput(bool readFromFile, bool csv){
    m_readFromFile  = readFromFile;
    m_csv           = csv;
    m_portsType     = BaseManipulation::ConnectionType::FORWARD;
    m_name          = "mimmo.GenericInput";
    m_dir           = "./";
};

/*!
 * Custom constructor reading xml data
 * \param[in] rootXML reference to your xml tree section
 */
GenericInput::GenericInput(const bitpit::Config::Section & rootXML){

    m_readFromFile  = false;
    m_csv           = false;
    m_portsType     = BaseManipulation::ConnectionType::FORWARD;
    m_name             = "mimmo.GenericInput";
    m_dir       = "./";
    m_filename  = "input.txt";

    std::string fallback_name = "ClassNONE";
    std::string input = rootXML.get("ClassName", fallback_name);
    input = bitpit::utils::string::trim(input);
    if(input == "mimmo.GenericInput"){
        absorbSectionXML(rootXML);
    }else{
        warningXML(m_log, m_name);
    };
}

/*!
 * Custom constructor of GenericInput.
 * \param[in] dir Directory of the input file.
 * \param[in] filename Name of the input file.
 * \param[in] csv True if the input file is a csv format file (default value false).
 * The m_readFromFile flag is set to true.
 */
GenericInput::GenericInput(std::string dir, std::string filename, bool csv){
    m_readFromFile  = true;
    m_csv           = csv;
    m_dir           = dir;
    m_filename      = filename;
    m_portsType     = BaseManipulation::ConnectionType::FORWARD;
};

GenericInput::~GenericInput(){};

/*!
 * Copy constructor of GenericInput.
 */
GenericInput::GenericInput(const GenericInput & other):BaseManipulation(other){
    m_readFromFile  = other.m_readFromFile;
    m_csv           = other.m_csv;
    m_dir           = other.m_dir;
    m_filename      = other.m_filename;
};

/*!
 * Assignement operator of GenericInput.
 */
GenericInput & GenericInput::operator=(const GenericInput & other){
    *(static_cast<BaseManipulation*> (this)) = *(static_cast<const BaseManipulation*> (&other));
    m_readFromFile     = other.m_readFromFile;
    m_csv           = other.m_csv;
    m_dir           = other.m_dir;
    m_filename         = other.m_filename;
    return *this;
};

/*!
 * It sets if the object imports the displacements from an input file.
 * \param[in] readFromFile True if the object reads the values from file.
 */
void
GenericInput::setReadFromFile(bool readFromFile){
    m_readFromFile = readFromFile;
};

/*!It sets if the input file is in csv format.
 * \param[in] csv Is the input file write in comma separated value format?
 */
void
GenericInput::setCSV(bool csv){
    m_csv = csv;
};

/*!It sets the name of the input file.
 * \param[in] filename Name of the input file.
 */
void
GenericInput::setFilename(std::string filename){
    m_filename = filename;
};

/*!It sets the name of the directory of input file.
 * \param[in] dir Name of the directory of input file.
 */
void
GenericInput::setReadDir(std::string dir){
    m_dir = dir;
};

/*! It builds the input/output ports of the object
 */
void
GenericInput::buildPorts(){

    PortManager::instance().addPort(M_COORDS, MC_VECARR3, MD_FLOAT);
    PortManager::instance().addPort(M_DISPLS, MC_VECARR3, MD_FLOAT);
    PortManager::instance().addPort(M_FILTER, MC_VECTOR, MD_FLOAT);
    PortManager::instance().addPort(M_SCALARFIELD, MC_VECTOR, MD_FLOAT);
    PortManager::instance().addPort(M_POINT, MC_ARRAY3, MD_FLOAT);
    PortManager::instance().addPort(M_SPAN, MC_ARRAY3, MD_FLOAT);
    PortManager::instance().addPort(M_DIMENSION, MC_ARRAY3, MD_INT);
    PortManager::instance().addPort(M_VALUED, MC_SCALAR, MD_FLOAT);
    PortManager::instance().addPort(M_VALUEI, MC_SCALAR, MD_INT);
    PortManager::instance().addPort(M_VALUEB, MC_SCALAR, MD_BOOL);
    PortManager::instance().addPort(M_DEG, MC_ARRAY3, MD_INT);
    PortManager::instance().addPort(M_FILENAME, MC_SCALAR, MD_STRING);
    PortManager::instance().addPort(M_DIR, MC_SCALAR, MD_STRING);
    
    bool built = true;
    built = (built && createPortOut<dvecarr3E, GenericInput>(this, &mimmo::GenericInput::getResult<dvecarr3E>, M_COORDS));
    built = (built && createPortOut<dvecarr3E, GenericInput>(this, &mimmo::GenericInput::getResult<dvecarr3E>, M_DISPLS));
    built = (built && createPortOut<dvector1D, GenericInput>(this, &mimmo::GenericInput::getResult<dvector1D>, M_FILTER));
    built = (built && createPortOut<dvector1D, GenericInput>(this, &mimmo::GenericInput::getResult<dvector1D>, M_SCALARFIELD));
    built = (built && createPortOut<darray3E, GenericInput>(this, &mimmo::GenericInput::getResult<darray3E>, M_POINT));
    built = (built && createPortOut<darray3E, GenericInput>(this, &mimmo::GenericInput::getResult<darray3E>, M_SPAN));
    built = (built && createPortOut<iarray3E, GenericInput>(this, &mimmo::GenericInput::getResult<iarray3E>, M_DIMENSION));
    built = (built && createPortOut<double, GenericInput>(this, &mimmo::GenericInput::getResult<double>, M_VALUED));
    built = (built && createPortOut<int, GenericInput>(this, &mimmo::GenericInput::getResult<int>, M_VALUEI));
    built = (built && createPortOut<bool, GenericInput>(this, &mimmo::GenericInput::getResult<bool>, M_VALUEB));
    built = (built && createPortOut<iarray3E, GenericInput>(this, &mimmo::GenericInput::getResult<iarray3E>, M_DEG));
    built = (built && createPortOut<string, GenericInput>(this, &mimmo::GenericInput::getResult<string>, M_FILENAME));
    built = (built && createPortOut<string, GenericInput>(this, &mimmo::GenericInput::getResult<string>, M_DIR));
    //     built = (built && createPortOut<string*, GenericInput>(this, &mimmo::GenericInput::getResult<string*>, M_FILENAMEPTR));
    //     built = (built && createPortOut<string*, GenericInput>(this, &mimmo::GenericInput::getResult<string*>, M_DIRPTR));

    m_arePortsBuilt = built;
}

/*!It clear the input member of the object
 */
void
GenericInput::clearInput(){
    m_input.reset(nullptr);
}

/*!It clear the result member of the object
 */
void
GenericInput::clearResult(){
    m_result.reset(nullptr);
}

/*!Execution command.
 * It does nothing, the real execution of the object
 * happens in setInput/getResult.
 */
void
GenericInput::execute(){};

/*!
 * It sets infos reading from a XML bitpit::Config::section.
 * \param[in] slotXML bitpit::Config::Section of XML file
 * \param[in] name   name associated to the slot
 */
void
GenericInput::absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name){

    BITPIT_UNUSED(name);

    std::string input;

    BaseManipulation::absorbSectionXML(slotXML, name);

    if(slotXML.hasOption("ReadFromFile")){
        std::string input = slotXML.get("ReadFromFile");
        input = bitpit::utils::string::trim(input);
        bool temp = false;
        if(!input.empty()){
            std::stringstream ss(input);
            ss>>temp;
        }
        setReadFromFile(temp);
    };

    if(slotXML.hasOption("CSV")){
        std::string input = slotXML.get("CSV");
        input = bitpit::utils::string::trim(input);
        bool temp = false;
        if(!input.empty()){
            std::stringstream ss(input);
            ss>>temp;
        }
        setCSV(temp);
    };

    if(slotXML.hasOption("Filename") && m_readFromFile){
        std::string input = slotXML.get("Filename");
        input = bitpit::utils::string::trim(input);
        setFilename(input);
    };

    if(slotXML.hasOption("ReadDir") && m_readFromFile){
        std::string input = slotXML.get("ReadDir");
        input = bitpit::utils::string::trim(input);
        setReadDir(input);
    };

}

/*!
 * It sets infos from class members in a XML bitpit::Config::section.
 * \param[in] slotXML bitpit::Config::Section of XML file
 * \param[in] name   name associated to the slot
 */
void
GenericInput::flushSectionXML(bitpit::Config::Section & slotXML, std::string name){

    BITPIT_UNUSED(name);

    BaseManipulation::flushSectionXML(slotXML, name);
    
    slotXML.set("ReadFromFile", std::to_string((int)m_readFromFile));
    slotXML.set("CSV", std::to_string((int)m_csv));
    slotXML.set("ReadDir", m_dir);
    slotXML.set("Filename", m_filename);
};

}

