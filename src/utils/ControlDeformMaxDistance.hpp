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
#ifndef __CONTROLDEFORMMAXDISTANCE_HPP__
#define __CONTROLDEFORMMAXDISTANCE_HPP__

#include "BaseManipulation.hpp"
#include "MimmoObject.hpp"

namespace mimmo{

/*!
 * \class ControlDeformMaxDistance
 * \ingroup utils
 * \brief ControlDeformMaxDistance is a class that check a deformation field associated to a MimmoObject geometry,
 *  once a maximum limit distance of deformation is fixed, w.r.t. the undeformed state.
 *
 * ControlDeformMaxDistance is derived from BaseManipulation class.
 * It needs a maximum, isotropic limit distance d w.r.t. geometry undeformed state, which is used to evaluate the isolevel d
 * of the target geometry. 
 * Returns a double value V, namely the maximum signed distance from constraint iso-level amongst all field points, 
 * reporting how much the current deformation field violate the constraint itself.
 * if V >0 a violation occurs. if V=0, a contact occurs, otherwise if V<0 no violation occurs. 
 * No optional result are plot. 
 * Class absorbs/flushes its parameters from/to xml dictionaries
 *
 * \n
 * Ports available in ControlDeformMaxDistance Class :
 *
 *    =========================================================

     |Port Input  | | |
     |-|-|-|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_GDISPLS| setDefField       | (MC_VECARR3, MD_FLOAT)       |
     | M_VALUED | setLimitDistance  | (MC_SCALAR, MD_FLOAT)         |
     | M_GEOM   | setGeometry       | (MC_SCALAR, MD_MIMMO_)        |

     |Port Output | | |
     |-|-|-|
     | <B>PortType</B> | <B>variable/function</B> |<B>DataType</B> |
     | M_SCALARFIELD | getViolationField | (MC_VECTOR, MD_FLOAT)             |
     | M_VALUED      | getViolation      | (MC_SCALAR, MD_FLOAT)             |
     | M_VIOLATION   | getViolationPair  | (MC_PAIR,MD_PAIRMIMMO_OBJFLOAT_)  |

 *    =========================================================
 * \n
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 * - <B>ClassName</B>: name of the class as <tt>mimmo.ControlDeformMaxDistance</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class;
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Proper of the class:
 * - <B>LimitDistance</B>: constraint surface distance from target geometry;
 *
 * Geometry and deformation field have to be mandatorily passed through port.
 *
 */
class ControlDeformMaxDistance: public BaseManipulation{
private:
    double                        m_maxDist;        /**<Limit Distance*/
    dvector1D                    m_violationField;    /**<Violation Distance Field */
    dvecarr3E                    m_defField;     /**<Deformation field*/

public:
    ControlDeformMaxDistance();
    ControlDeformMaxDistance(const bitpit::Config::Section & rootXML);
    ~ControlDeformMaxDistance();

    ControlDeformMaxDistance(const ControlDeformMaxDistance & other);
    ControlDeformMaxDistance & operator=(const ControlDeformMaxDistance & other);

    void    buildPorts();

    double                                     getViolation();
    dvector1D                                getViolationField();
    std::pair<BaseManipulation*, double>    getViolationPair();

    void    setDefField(dvecarr3E field);
    void    setLimitDistance(double dist);

    void     execute();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name = "");
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name= "");

protected:
    void plotOptionalResults();


};

REGISTER_PORT(M_GDISPLS, MC_VECARR3, MD_FLOAT,__CONTROLDEFORMMAXDISTANCE_HPP__)
REGISTER_PORT(M_VALUED, MC_SCALAR, MD_FLOAT,__CONTROLDEFORMMAXDISTANCE_HPP__)
REGISTER_PORT(M_GEOM, MC_SCALAR, MD_MIMMO_,__CONTROLDEFORMMAXDISTANCE_HPP__)
REGISTER_PORT(M_SCALARFIELD, MC_VECTOR, MD_FLOAT,__CONTROLDEFORMMAXDISTANCE_HPP__)
REGISTER_PORT(M_VIOLATION, MC_PAIR, MD_PAIRMIMMO_OBJFLOAT_,__CONTROLDEFORMMAXDISTANCE_HPP__)


REGISTER(BaseManipulation, ControlDeformMaxDistance, "mimmo.ControlDeformMaxDistance")
}

#endif /* __CONTROLDEFORMMAXDISTANCE_HPP__ */
