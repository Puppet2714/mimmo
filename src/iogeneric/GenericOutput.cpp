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
#include "GenericOutput.hpp"

using namespace std;
namespace mimmo{

/*!
 * Default constructor of GenericOutput
 * \param[in] dir Directory of the output file (default value = "./").
 * \param[in] filename Name of the output file (default value = "output.txt").
 * \param[in] csv True if the output file is a csv format file (default value false).
 */
GenericOutput::GenericOutput(std::string dir, std::string filename, bool csv){
    m_dir       = dir;
    m_filename  = filename;
    m_csv       = csv;
    m_portsType    = ConnectionType::BACKWARD;
    m_name         = "mimmo.GenericOutput";
};

/*!
 * Custom constructor reading xml data
 * \param[in] rootXML reference to your xml tree section
 */
GenericOutput::GenericOutput(const bitpit::Config::Section & rootXML){

    m_dir       = "./";
    m_filename  = "output.txt";
    m_portsType    = ConnectionType::BACKWARD;
    m_name         = "mimmo.GenericOutput";
    m_csv       = false;

    std::string fallback_name = "ClassNONE";
    std::string input = rootXML.get("ClassName", fallback_name);
    input = bitpit::utils::string::trim(input);
    if(input == "mimmo.GenericOutput"){
        absorbSectionXML(rootXML);
    }else{
        warningXML(m_log, m_name);
    };
}

/*!
 * Default destructor of GenericOutput.
 */
GenericOutput::~GenericOutput(){
    m_portsType = ConnectionType::BACKWARD;
};

/*!
 * Copy constructor of GenericOutput.
 */
GenericOutput::GenericOutput(const GenericOutput & other):BaseManipulation(other){
    m_dir           = other.m_dir;
    m_filename      = other.m_filename;
    m_csv           = other.m_csv;
};

/*!
 * Assignement operator of GenericOutput.
 */
GenericOutput & GenericOutput::operator=(const GenericOutput & other){
    *(static_cast<BaseManipulation*> (this)) = *(static_cast<const BaseManipulation*> (&other));
    m_dir           = other.m_dir;
    m_filename         = other.m_filename;
    m_csv           = other.m_csv;
    return *this;
};

/*!
 * It builds the input/output ports of the object
 */
void
GenericOutput::buildPorts(){

    bool built = true;
    built = (built && createPortIn<dvecarr3E, GenericOutput>(this, &mimmo::GenericOutput::setInput<dvecarr3E>, M_COORDS));
    built = (built && createPortIn<dvecarr3E, GenericOutput>(this, &mimmo::GenericOutput::setInput<dvecarr3E>, M_DISPLS));
    built = (built && createPortIn<dvector1D, GenericOutput>(this, &mimmo::GenericOutput::setInput<dvector1D>, M_FILTER));
    built = (built && createPortIn<darray3E, GenericOutput>(this, &mimmo::GenericOutput::setInput<darray3E>, M_POINT));
    built = (built && createPortIn<iarray3E, GenericOutput>(this, &mimmo::GenericOutput::setInput<iarray3E>, M_DIMENSION));
    built = (built && createPortIn<double, GenericOutput>(this, &mimmo::GenericOutput::setInput<double>, M_VALUED));
    built = (built && createPortIn<int, GenericOutput>(this, &mimmo::GenericOutput::setInput<int>, M_VALUEI));
    built = (built && createPortIn<bool, GenericOutput>(this, &mimmo::GenericOutput::setInput<bool>, M_VALUEB));
    built = (built && createPortIn<iarray3E, GenericOutput>(this, &mimmo::GenericOutput::setInput<iarray3E>, M_DEG));
    built = (built && createPortIn<string, GenericOutput>(this, &mimmo::GenericOutput::setInput<string>, M_FILENAME));
    built = (built && createPortIn<string, GenericOutput>(this, &mimmo::GenericOutput::setInput<string>, M_DIR));
    //     built = (built && createPortIn<string*, GenericOutput>(this, &mimmo::GenericOutput::setInput<string*>, M_FILENAMEPTR));
    //     built = (built && createPortIn<string*, GenericOutput>(this, &mimmo::GenericOutput::setInput<string*>, M_DIRPTR));

    m_arePortsBuilt = built;
}

/*!
 * It sets the name of the directory to write the output file.
 * \param[in] dir Name of the directory of the output file.
 */
void
GenericOutput::setWriteDir(std::string dir){
    m_dir = dir;
};

/*!
 * It sets the name of the output file.
 * \param[in] filename Name of the output file.
 */
void
GenericOutput::setFilename(std::string filename){
    m_filename = filename;
};

/*!
 * It sets if the output file has to be written in csv format.
 * \param[in] csv Write the output file in csv format.
 */
void
GenericOutput::setCSV(bool csv){
    m_csv = csv;
};

/*!
 * It clear the input member of the object
 */
void
GenericOutput::clearInput(){
    m_input.reset(nullptr);
}

/*!
 * It clear the result member of the object
 */
void
GenericOutput::clearResult(){
    m_result.reset(nullptr);
}

/*!Execution command.
 * It does nothing, the real execution of the object
 * happens in setInput.
 */
void
GenericOutput::execute(){

    bool ok = rename("output.txt", m_filename.c_str());
    if (!ok)
        (*m_log)<<"Generic Output execution failed renaming..."<<std::endl;
};

/*!
 * It sets infos reading from a XML bitpit::Config::section.
 * \param[in] slotXML bitpit::Config::Section of XML file
 * \param[in] name   name associated to the slot
 */
void
GenericOutput::absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name){

    BITPIT_UNUSED(name);
    std::string input;

    BaseManipulation::absorbSectionXML(slotXML, name);

    if(slotXML.hasOption("Filename")){
        std::string input = slotXML.get("Filename");
        input = bitpit::utils::string::trim(input);
        setFilename(input);
    };

    if(slotXML.hasOption("WriteDir")){
        std::string input = slotXML.get("WriteDir");
        input = bitpit::utils::string::trim(input);
        setWriteDir(input);
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

}

/*!
 * It sets infos from class members in a XML bitpit::Config::section.
 * \param[in] slotXML bitpit::Config::Section of XML file
 * \param[in] name   name associated to the slot
 */
void
GenericOutput::flushSectionXML(bitpit::Config::Section & slotXML, std::string name){

    BITPIT_UNUSED(name);

    BaseManipulation::flushSectionXML(slotXML, name);
    
    slotXML.set("Filename", m_filename);
    slotXML.set("WriteDir", m_dir);
    slotXML.set("CSV", std::to_string((int)m_csv));
};

}
