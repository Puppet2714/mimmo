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
#ifndef __STITCHGEOMETRY_HPP__
#define __STITCHGEOMETRY_HPP__

#include "MimmoObject.hpp"
#include "BaseManipulation.hpp"

namespace mimmo{

/*!
 *    \class StitchGeometry
 * \ingroup geohandlers
 *    \brief StitchGeometry is an executable block class capable of
 *         stitch multiple MimmoObject geometries of the same topology 
 *
 *    StitchGeometry is the object to append two or multiple MimmoObject of the same topology
 *  in a unique MimmoObject container. 
 * 
 * Ports available in StitchGeometry Class :
 *
 *    =========================================================

     |Port Input | | |
     |-|-|-|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_GEOM   | setAddGeometry                     | (MC_SCALAR, MD_MIMMO_)      |
     | M_VECGEOM| setGeometry                        | (MC_VECTOR, MD_MIMMO_)      |


     |Port Output | | |
     |-|-|-|
     | <B>PortType</B> | <B>variable/function</B> |<B>DataType</B>              |
     | M_GEOM    | getGeometry                        | (MC_SCALAR, MD_MIMMO_)         |
     | M_VECGEOM | getOriginalGeometries              | (MC_VECTOR, MD_MIMMO_)         |
     | M_MAPDCELL| getCellDivisionMap                 | (MC_UN_MAP, MD_LONGPAIRINTLONG)|
     | M_MAPDVERT| getVertDivisionMap                 | (MC_UN_MAP, MD_LONGPAIRINTLONG)|

 *    =========================================================
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 * - <B>ClassName</B>: name of the class as "mimmo.StitchGeometry"
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class.
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Proper of the class:
 * - <B>Topology</B>: info on admissible topology format 1-surface, 2-volume, 3-pointcloud
 * - <B>BvTree</B>: evaluate bvTree true 1/false 0
 * - <B>KdTree</B>: evaluate kdTree true 1/false 0
 
 * Geometries have to be mandatorily passed through port.
 *
 */
class StitchGeometry: public BaseManipulation{

private:
    int                                     m_topo;        /**<Mark topology of your stitcher 1-surface, 2-volume, 3-pointcloud*/
    std::unordered_map<MimmoObject*,int>    m_extgeo;    /**< pointers to external geometries*/

    std::unique_ptr<MimmoObject> m_patch;    /**< resulting patch geometry */

    std::unordered_map<long, std::pair<int, long> > m_mapCellDivision; /**< division map of actual ID-cell, part Id, original ID-cell*/
    std::unordered_map<long, std::pair<int, long> > m_mapVertDivision; /**< division map of actual ID-vertex, part Id, original ID-vertex*/

    bool        m_buildBvTree;                /**<If true build BvTree of stitched geometry. */
    bool        m_buildKdTree;                /**<If true build KdTree of stitched geometry. */

    int m_geocount;                            /**<Internal geometry counter */

public:
    StitchGeometry(int topo);
    StitchGeometry(const bitpit::Config::Section & rootXML);
    virtual ~StitchGeometry();

    StitchGeometry(const StitchGeometry & other);
    StitchGeometry & operator=(const StitchGeometry & other);

    void buildPorts();

    int                             getTopology();
    std::vector< MimmoObject *>     getOriginalGeometries();
    MimmoObject *                     getGeometry();

    std::unordered_map<long, std::pair<int, long> >    getCellDivisionMap();
    std::unordered_map<long, std::pair<int, long> >    getVertDivisionMap();

    void        setAddGeometry(MimmoObject * geo);
    void        setGeometry( std::vector<MimmoObject *> list);

    void        setBuildBvTree(bool build);
    void        setBuildKdTree(bool build);

    bool         isEmpty();

    void         clear();
    void         execute();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="");
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="");

    void     plotOptionalResults();
};

REGISTER_PORT(M_GEOM, MC_SCALAR, MD_MIMMO_, __STITCHGEOMETRY_HPP__)
REGISTER_PORT(M_VECGEOM, MC_VECTOR, MD_MIMMO_,__STITCHGEOMETRY_HPP__)
REGISTER_PORT(M_MAPDCELL, MC_UN_MAP, MD_LONGPAIRINTLONG,__STITCHGEOMETRY_HPP__)
REGISTER_PORT(M_MAPDVERT, MC_UN_MAP, MD_LONGPAIRINTLONG,__STITCHGEOMETRY_HPP__)


REGISTER(BaseManipulation, StitchGeometry, "mimmo.StitchGeometry")

};

#endif /* __STITCHGEOMETRY_HPP__ */
