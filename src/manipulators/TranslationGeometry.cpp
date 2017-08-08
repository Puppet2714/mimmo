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
#include "TranslationGeometry.hpp"

namespace mimmo{


/*!
 * Default constructor of TranslationGeometry
 */
TranslationGeometry::TranslationGeometry(darray3E direction){
    m_direction = direction;
    m_alpha = 0.0;
    m_name = "mimmo.TranslationGeometry";
};

/*!
 * Custom constructor reading xml data
 * \param[in] rootXML reference to your xml tree section
 */
TranslationGeometry::TranslationGeometry(const bitpit::Config::Section & rootXML){

    m_direction.fill(0.0);
    m_alpha = 0.0;
    m_name = "mimmo.TranslationGeometry";

    std::string fallback_name = "ClassNONE";
    std::string input = rootXML.get("ClassName", fallback_name);
    input = bitpit::utils::string::trim(input);
    if(input == "mimmo.TranslationGeometry"){
        absorbSectionXML(rootXML);
    }else{
        warningXML(m_log, m_name);
    };
}

/*!Default destructor of TranslationGeometry
 */
TranslationGeometry::~TranslationGeometry(){};

/*!Copy constructor of TranslationGeometry.
 */
TranslationGeometry::TranslationGeometry(const TranslationGeometry & other):BaseManipulation(other){
    *this = other;
};

/*!Assignement operator of TranslationGeometry.
 */
TranslationGeometry & TranslationGeometry::operator=(const TranslationGeometry & other){
    *(static_cast<BaseManipulation*> (this)) = *(static_cast<const BaseManipulation*> (&other));
    m_direction = other.m_direction;
    m_alpha = other.m_alpha;
    return(*this);
};


/*! It builds the input/output ports of the object
 */
void
TranslationGeometry::buildPorts(){
    bool built = true;
    built = (built && createPortIn<darray3E, TranslationGeometry>(&m_direction, M_AXIS));
    built = (built && createPortIn<double, TranslationGeometry>(&m_alpha, M_VALUED));
    built = (built && createPortIn<dvector1D, TranslationGeometry>(this, &mimmo::TranslationGeometry::setFilter, M_FILTER));
    built = (built && createPortIn<MimmoObject*, TranslationGeometry>(&m_geometry, M_GEOM, true));
    built = (built && createPortOut<dvecarr3E, TranslationGeometry>(this, &mimmo::TranslationGeometry::getDisplacements, M_GDISPLS));
    built = (built && createPortOut<std::pair<MimmoObject*, dvecarr3E*> , TranslationGeometry>(this, &mimmo::TranslationGeometry::getDeformedField, M_PAIRVECFIELD));
    built = (built && createPortOut<MimmoObject*, TranslationGeometry>(this, &BaseManipulation::getGeometry, M_GEOM));
    m_arePortsBuilt = built;
};

/*!It sets the direction of the translation axis.
 * \param[in] direction Direction of translation axis.
 */
void
TranslationGeometry::setDirection(darray3E direction){
    m_direction = direction;
    double L = norm2(m_direction);
    for (int i=0; i<3; i++)
        m_direction[i] /= L;
}

/*!It sets the value of the translation.
 * \param[in] alpha Value of translation in length unity.
 */
void
TranslationGeometry::setTranslation(double alpha){
    m_alpha = alpha;
}

/*!It sets the filter field to modulate the displacements of the vertices
 * of the target geometry.
 * \param[in] filter Filter field defined on geometry vertices.
 */
void
TranslationGeometry::setFilter(dvector1D filter){
    m_filter = filter;
}

/*!
 * Return actual computed displacements field (if any) for the geometry linked.
 * \return  deformation field
 */
dvecarr3E
TranslationGeometry::getDisplacements(){
    return m_displ;
};

/*!
 * Return actual computed deformation field (if any) for the geometry linked.
 * If no field is actually present, return null pointers;
 * \return     std::pair of pointers linking to actual geometry pointed by the class, and the computed deformation field on its vertices
 */
std::pair<MimmoObject * , dvecarr3E * >
TranslationGeometry::getDeformedField(){

    std::pair<MimmoObject *, dvecarr3E * > pairField;
    pairField.first = getGeometry();
    pairField.second = &m_displ;
    return pairField;
};

/*!Execution command. It perform the translation by computing the displacements
 * of the points of the geometry. It applies a filter field eventually set as input.
 */
void
TranslationGeometry::execute(){

    if (getGeometry() == NULL) return;

    int nV = m_geometry->getNVertex();
    m_displ.resize(nV);
    m_filter.resize(nV, 1.0);

    long ID;
    int idx;
    liimap mapID = m_geometry->getMapDataInv();

    for (auto vertex : m_geometry->getVertices()){
        ID = vertex.getId();
        idx = mapID[ID];


        m_displ[idx] = m_alpha*m_direction*m_filter[idx];

    }
};

/*!
 * Directly apply deformation field to target geometry.
 */
void
TranslationGeometry::apply(){

    if (getGeometry() == NULL) return;
    dvecarr3E vertex = getGeometry()->getVertexCoords();
    long nv = getGeometry()->getNVertex();
    nv = long(std::min(int(nv), int(m_displ.size())));
    livector1D & idmap = getGeometry()->getMapData();
    for (long i=0; i<nv; i++){
        vertex[i] += m_displ[i];
        getGeometry()->modifyVertex(vertex[i], idmap[i]);
    }

}

/*!
 * It sets infos reading from a XML bitpit::Config::section.
 * \param[in] slotXML bitpit::Config::Section of XML file
 * \param[in] name   name associated to the slot
 */
void
TranslationGeometry::absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name){

    BITPIT_UNUSED(name);

    BaseManipulation::absorbSectionXML(slotXML, name);
    
    if(slotXML.hasOption("Direction")){
        std::string input = slotXML.get("Direction");
        input = bitpit::utils::string::trim(input);
        darray3E temp = {{0.0,0.0,0.0}};
        if(!input.empty()){
            std::stringstream ss(input);
            for(auto &val : temp) ss>>val;
        }
        setDirection(temp);
    }

    if(slotXML.hasOption("Translation")){
        std::string input = slotXML.get("Translation");
        input = bitpit::utils::string::trim(input);
        double temp = 0.0;
        if(!input.empty()){
            std::stringstream ss(input);
            ss>>temp;
        }
        setTranslation(temp);
    }

};

/*!
 * It sets infos from class members in a XML bitpit::Config::section.
 * \param[in] slotXML bitpit::Config::Section of XML file
 * \param[in] name   name associated to the slot
 */
void
TranslationGeometry::flushSectionXML(bitpit::Config::Section & slotXML, std::string name){

    BITPIT_UNUSED(name);
    BaseManipulation::flushSectionXML(slotXML, name);
    {
        std::stringstream ss;
        ss<<std::scientific<<m_direction[0]<<'\t'<<m_direction[1]<<'\t'<<m_direction[2];
        slotXML.set("Direction", ss.str());
    }

    slotXML.set("Translation", std::to_string(m_alpha));
};

}
