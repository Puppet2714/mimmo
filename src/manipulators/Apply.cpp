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
#include "Apply.hpp"


namespace mimmo{

/*!
 * Default constructor of Apply
 */
Apply::Apply():BaseManipulation(){
    m_name = "mimmo.Apply";
};

/*!
 * Custom constructor reading xml data
 * \param[in] rootXML reference to your xml tree section
 */
Apply::Apply(const bitpit::Config::Section & rootXML){

    m_name = "mimmo.Apply";

    std::string fallback_name = "ClassNONE";
    std::string input = rootXML.get("ClassName", fallback_name);
    input = bitpit::utils::string::trim(input);
    if(input == "mimmo.Apply"){
        absorbSectionXML(rootXML);
    }else{
        warningXML(m_log, m_name);
    };
}

/*!Default destructor of Apply
 */
Apply::~Apply(){};

/*!Copy constructor of Apply.
 */
Apply::Apply(const Apply & other):BaseManipulation(){
    *this = other;
};

/*!Assignement operator of Apply.
 */
Apply & Apply::operator=(const Apply & other){
    *(static_cast<BaseManipulation*> (this)) = *(static_cast<const BaseManipulation*> (&other));
    m_input = other.m_input;
    return(*this);
};


/*! It builds the input/output ports of the object
 */
void
Apply::buildPorts(){
    
    PortManager::instance().addPort(M_GDISPLS, MC_VECARR3, MD_FLOAT);
    PortManager::instance().addPort(M_GEOM, MC_SCALAR, MD_MIMMO_);
    
    bool built = true;
    built = (built && createPortIn<dvecarr3E, Apply>(this, &Apply::setInput, M_GDISPLS, true));
    built = (built && createPortIn<MimmoObject*, Apply>(this, &BaseManipulation::setGeometry, M_GEOM, true));
    built = (built && createPortOut<MimmoObject*, Apply>(this, &BaseManipulation::getGeometry, M_GEOM));
    m_arePortsBuilt = built;
};

/*!It sets the displacements input.
 * \param[in] input Input displacements of the geometry vertices.
 */
void
Apply::setInput(dvecarr3E input){
    m_input = input;
}

/*!Execution command.
 * It applies the deformation stored in the input of base class (casting the input
 * for apply object to dvecarr3E) to the linked geometry.
 * After exec() the original geometry will be permanently modified.
 */
void
Apply::execute(){
    if (getGeometry() == NULL) return;

    dvecarr3E vertex = getGeometry()->getVertexCoords();
    long nv = getGeometry()->getNVertex();
    nv = long(std::min(int(nv), int(m_input.size())));
    livector1D & idmap = getGeometry()->getMapData();
    for (long i=0; i<nv; i++){
        vertex[i] += m_input[i];
        getGeometry()->modifyVertex(vertex[i], idmap[i]);
    }

};


/*!
 * It sets infos reading from a XML bitpit::Config::section.
 * \param[in] slotXML bitpit::Config::Section of XML file
 * \param[in] name   name associated to the slot
 */
void
Apply::absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name){

    BITPIT_UNUSED(name);
    BaseManipulation::absorbSectionXML(slotXML, name);

};

/*!
 * It sets infos from class members in a XML bitpit::Config::section.
 * \param[in] slotXML bitpit::Config::Section of XML file
 * \param[in] name   name associated to the slot
 */
void
Apply::flushSectionXML(bitpit::Config::Section & slotXML, std::string name){

    BITPIT_UNUSED(name);

    BaseManipulation::flushSectionXML(slotXML, name);
    
};


}


