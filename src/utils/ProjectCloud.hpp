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
#ifndef __PROJECTCLOUD_HPP__
#define __PROJECTCLOUD_HPP__

#include "BaseManipulation.hpp"


/*!
 * \ingroup Ports
 * \{
 */

//PORTS DEFINITION AS CONSTANTS
#ifndef M_GEOM
#define M_GEOM "M_GEOM" /**< Port dedicated to communication of pointers to a MimmoObject object*/
#endif

#ifndef M_COORDS 
#define M_COORDS "M_COORDS" /**< Port dedicated to communication  of a list of point 3D coordinates.*/
#endif

/*!
 * \}
 */

/*!
 * \ingroup PortContainers
 * \{
 */

#ifndef MC_SCALAR
#define MC_SCALAR "MC_SCALAR" /**< Single value container identifier*/
#endif

#ifndef MC_VECARR3 
#define MC_VECARR3 "MC_VECARR3" /**< std::vector< std::array< ., 3> > container identifier*/
#endif

/*!
 * \}
 */

/*!
 * \ingroup PortData
 * \{
 */

#ifndef MD_MIMMO_ 
#define MD_MIMMO_ "MD_MIMMO_" /**< MimmoObject pointer data identifier*/
#endif

#ifndef MD_FLOAT
#define MD_FLOAT "MD_FLOAT" /**< float/double data identifier*/
#endif

/*!
 * \}
 */



namespace mimmo{

/*!
 *  \class ProjectCloud
 *  \ingroup utils
 *  \brief ProjectCloud is the class that project a 3D cloud of points on a target geometry
 *
 * \n
 * Ports available in ProjectCloud Class :
 *
 *  =========================================================

   |Port Input | | |
   |-|-|-|
   | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> | 
   | M_COORDS | setCoords         | (MC_VECARR3, MD_FLOAT)    |
   | M_GEOM   | setGeometry       | (MC_SCALAR, MD_MIMMO_)    |

   |Port Output  | | |
   |-|-|-|
   | <B>PortType</B> | <B>variable/function</B> |<B>DataType</B>   |
   | M_COORDS | getCloudResult    | (MC_VECARR3, MD_FLOAT)    |

 *    =========================================================
 * \n
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 * - <B>ClassName</B>: name of the class as <tt>mimmo.ProjectCloud</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class;
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Geometry and points have to be mandatorily passed through port.
 *
 */

class ProjectCloud: public BaseManipulation{
protected:
    dvecarr3E            m_points;    /**<Coordinates of 3D points in the cloud.*/
    dvecarr3E            m_proj;     /**<Projected points coordinates.*/

public:
    ProjectCloud();
    ProjectCloud(const bitpit::Config::Section & rootXML);
    ~ProjectCloud();

    ProjectCloud(const ProjectCloud & other);
    ProjectCloud & operator=(const ProjectCloud & other);

    void    buildPorts();

    dvecarr3E    getCoords();
    dvecarr3E    getCloudResult();

    void    setCoords(dvecarr3E coords);
    void     execute();

    void clear();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name= "");
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name= "");

protected:
    virtual void plotOptionalResults();

};

//Ports registration
REGISTER_PORT(M_GEOM, MC_SCALAR, MD_MIMMO_)
REGISTER_PORT(M_COORDS, MC_VECARR3, MD_FLOAT)

//ManipBlock registration
REGISTER(BaseManipulation, ProjectCloud,"mimmo.ProjectCloud")

};

#endif /* __PROJECTCLOUD_HPP__ */
