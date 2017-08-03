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
#ifndef __SPLITFIELDS_HPP__
#define __SPLITFIELDS_HPP__

#include "MimmoObject.hpp"
#include "BaseManipulation.hpp"

/*!
 * \ingroup Ports
 * \{
 */

//PORTS DEFINITION AS CONSTANTS
#ifndef M_GEOM
#define M_GEOM "M_GEOM" /**< Port dedicated to communication of pointers to a MimmoObject object*/
#endif

#ifndef M_VECGEOM 
#define M_VECGEOM "M_VECGEOM" /**< Port dedicated to communication  of std::vector of MimmoObject pointers.*/
#endif

#ifndef M_MAPDCELL
#define M_MAPDCELL "M_MAPDCELL" /**< Port dedicated to communication of a unordered map of cell-ids of type < long, std::pair < int, long > >*/
#endif

#ifndef M_MAPDVERT
#define M_MAPDVERT "M_MAPDVERT" /**< Port dedicated to communication of a unordered map of vertex-ids of type < long, std::pair < int, long > >*/
#endif

#ifndef M_SCALARFIELD 
#define M_SCALARFIELD "M_SCALARFIELD" /**< Port dedicated to communication of a generic scalar field [ vector < double > ].*/
#endif

#ifndef M_GDISPLS
#define M_GDISPLS "M_GDISPLS" /**< Port dedicated to communication of displacements of geometry vertex [vector< array < double,3 > >].*/
#endif

#ifndef M_UMGEOSFD 
#define M_UMGEOSFD "M_UMGEOSFD" /**< Port dedicated to communication of an unorderd map of MimmoObject pointers  and std::vector< double > pointers.*/
#endif

#ifndef M_UMGEOVFD 
#define M_UMGEOVFD "M_UMGEOVFD" /**< Port dedicated to communication of an unorderd map of MimmoObject pointers  and std::vector< std::array < double,3> > pointers.*/
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

#ifndef MC_VECTOR 
#define MC_VECTOR "MC_VECTOR" /**< std::vector< . > container identifier*/
#endif

#ifndef MC_UN_MAP
#define MC_UN_MAP "MC__UN_MAP" /**< std::unordered_map< . , .> container identifier*/
#endif

#ifndef MC_VECARR3
#define MC_VECARR3 "MC_VECARR3" /**< std::vector< std::array< . , 3 > > container identifier*/
#endif

/*!
 * \}
 */

/*!
 * \ingroup PortData
 * \{
 */
#ifndef MD_LONGPAIRINTLONG
#define MD_LONGPAIRINTLONG "MD_LONGPAIRINTLONG" /**< tuple (long, std::pair< int, long>) data identifier*/
#endif

#ifndef MD_FLOAT 
#define MD_FLOAT "MD_FLOAT" /**< float/double data identifier*/
#endif

#ifndef MD_MIMMO_ 
#define MD_MIMMO_ "MD_MIMMO_" /**< MimmoObject pointer data identifier*/
#endif

#ifndef MD_MIMMO_VECFLOAT_
#define MD_MIMMO_VECFLOAT_ "MD_MIMMO_VECFLOAT_" /**< tuple (MimmoObject *, std::vector< double > *) data identifier*/
#endif

#ifndef MD_MIMMO_VECARR3FLOAT_
#define MD__MIMMO_VECARR3FLOAT_ "MD__MIMMO_VECARR3FLOAT_" /**< tuple (MimmoObject *, std::vector< std::array< double, 3> > *) data identifier*/
#endif

/*!
 * \}
 */



namespace mimmo{

/*!
 * \class SplitField
 * \ingroup geohandlers
 * \brief SplitField is an abstract executable block class capable of
 *         splitting fields associated to a stitched MimmoObject, once the
 *           original geometries of the stitched object and its division maps are set
 *
 *    SplitField is an abstract class. To use its features take a look to its specializations,
 *  here presented as derived class, SplitScalarField and SplitVectorField.
 * 
 * Ports available in SplitField Class :
 *
 *    =========================================================
 *
     |                 Port Input     ||                                                     |
     |------------|------------------------------------|-----------------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_GEOM     | setGeometry                        | (MC_SCALAR, MD_MIMMO_)            |
     | M_VECGEOM  | setSplittedGeometries              | (MC_VECTOR, MD_MIMMO_)            |
     | M_MAPDCELL | setCellDivisionMap                 | (MC_UN_MAP, MD_LONGPAIRINTLONG)   |
     | M_MAPDVERT | setVertDivisionMap                 | (MC_UN_MAP, MD_LONGPAIRINTLONG)   |


    |            Port Output         ||             |
    |------------|------------------------------------|-----------------------------|
    | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |

 *    =========================================================
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 * - <B>ClassName</B>: name of the class as "mimmo.Split<Scalar/Vector>Fields"
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class.
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Proper of the class:
 * - <B>Topology</B>: info on admissible topology format 1-surface, 2-volume, 3-pointcloud
 *
 * Geometries and fields have to be mandatorily passed through port.
 *
 */
class SplitField: public BaseManipulation{

protected:
    int m_topo;                                 /**< flag for geometrical topology control*/
    std::vector<MimmoObject*>    m_originals;    /**< pointers to external geometries*/

    std::unordered_map<long, std::pair<int, long> > m_mapCellDivision; /**< division map of actual ID-cell, part Id, original ID-cell*/
    std::unordered_map<long, std::pair<int, long> > m_mapVertDivision; /**< division map of actual ID-vertex, part Id, original ID-vertex*/

public:
    SplitField(int topo = 1);
    virtual ~SplitField();

    SplitField(const SplitField & other);
    SplitField & operator=(const SplitField & other);

    void buildPorts();

    void        setGeometry(MimmoObject * geo);
    void        setSplittedGeometries( std::vector<MimmoObject *> list);
    void        setCellDivisionMap(std::unordered_map<long, std::pair<int, long> > map);
    void        setVertDivisionMap(std::unordered_map<long, std::pair<int, long> > map);

    bool         isEmpty();
    void         clear();
    int         getTopo();
    void         execute();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="");
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="");

private:
    /*!
     * Split method virtual. See derived classes SplitScalarField and SplitVectorField.
     */
    virtual bool split()=0;
};

/*!
 * \class SplitScalarField
 * \ingroup geohandlers
 * \brief SplitScalarField is specialized derived class of SplitField to split a
 *         scalar field of doubles.
 * 
 * Ports available in SplitScalarField Class :
 *
 *    =========================================================
 *
     |                 Port Input   ||                              |
     |--------------|--------------------|----------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_SCALARFIELD| setField           | (MC_VECTOR, MD_FLOAT)|


     |            Port Output   ||                                        |
     |-----------|-------------------|--------------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_UMGEOSFD| getSplittedData   | (MC_UN_MAP, MD_MIMMO_VECFLOAT_)|


  Inherited from SplitField

     |                 Port Input   ||                                                         |
     |------------|------------------------------------|-----------------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_GEOM     | setGeometry                        | (MC_SCALAR, MD_MIMMO_)            |
     | M_VECGEOM  | setSplittedGeometries              | (MC_VECTOR, MD_MIMMO_)            |
     | M_MAPDCELL | setCellDivisionMap                 | (MC_UN_MAP, MD_LONGPAIRINTLONG)   |
     | M_MAPDVERT | setVertDivisionMap                 | (MC_UN_MAP, MD_LONGPAIRINTLONG)   |


     |            Port Output  ||                               |
     |-----------|------------------------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |

 *    =========================================================
 *
 */

class SplitScalarField: public SplitField{
private:
    dvector1D m_field;                  /**<Input field to be split. */
    std::vector< dvector1D > m_result;  /**<Result split fields. */

public:
    SplitScalarField(int topo =1);
    SplitScalarField(const bitpit::Config::Section & rootXMl);
    virtual ~SplitScalarField();

    void buildPorts();
    std::unordered_map<MimmoObject*,dvector1D* >     getSplittedData();
    void     setField(dvector1D field);

    void clear();

    void     plotOptionalResults();
private:
    bool split();
};

/*!
 *  \class SplitVectorField
 *    \brief SplitVectorField is specialized derived class of SplitField to split a
 *         scalar field of array<double,3>.
 * 
 * Ports available in SplitVectorField Class :
 *
 *    =========================================================
 * ~~~
 *   |                 Port Input   ||                              |
 *
 *   |--------------|--------------------|----------------|
 *   | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
 *   | M_GDISPLS| setField           | (MC_VECARR3E, MD_FLOAT)|
 *
 *   |            Port Output   ||                                        |
 *   |-----------|-------------------|--------------------------|
 *   | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
 *   | M_UMGEOVSFD| getSplittedData   | (MC_UN_MAP, MD_MIMMO_VECARR3FLOAT_)|
 *
 * Inherited from SplitField
 *
 *   |                 Port Input   ||                                                         |
 *   |------------|------------------------------------|-----------------------------|
 *   | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
 *   | M_GEOM     | setGeometry                        | (MC_SCALAR, MD_MIMMO_)            |
 *   | M_VECGEOM  | setSplittedGeometries              | (MC_VECTOR, MD_MIMMO_)            |
 *   | M_MAPDCELL | setCellDivisionMap                 | (MC_UN_MAP, MD_LONGPAIRINTLONG)   |
 *   | M_MAPDVERT | setVertDivisionMap                 | (MC_UN_MAP, MD_LONGPAIRINTLONG)   |
 *
 *
 *   |            Port Output  ||                               |
 *   |-----------|------------------------------------|-----------------------|
 *   | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
 * ~~~
 *    =========================================================
 *
 */
class SplitVectorField: public SplitField{
private:
    dvecarr3E m_field;                  /**<Input field to be split. */
    std::vector< dvecarr3E > m_result;  /**<Result split fields. */

public:

    SplitVectorField(int topo =1);
    SplitVectorField(const bitpit::Config::Section & rootXMl);
    virtual ~SplitVectorField();

    void buildPorts();
    std::unordered_map< MimmoObject*,dvecarr3E* >     getSplittedData();
    void     setField(dvecarr3E field);

    void clear();

    void     plotOptionalResults();
private:
    bool split();
};

//Ports registration
REGISTER_PORT(M_GEOM, MC_SCALAR, MD_MIMMO_)
REGISTER_PORT(M_VECGEOM, MC_VECTOR, MD_MIMMO_)
REGISTER_PORT(M_MAPDCELL, MC_UN_MAP, MD_LONGPAIRINTLONG)
REGISTER_PORT(M_MAPDVERT, MC_UN_MAP, MD_LONGPAIRINTLONG)
REGISTER_PORT(M_SCALARFIELD, MC_VECTOR, MD_FLOAT)
REGISTER_PORT(M_GDISPLS, MC_VECARR3, MD_FLOAT)
REGISTER_PORT(M_UMGEOSFD, MC_UN_MAP, MD_MIMMO_VECFLOAT_)
REGISTER_PORT(M_UMGEOVFD, MC_UN_MAP, MD_MIMMO_VECARR3FLOAT_)


//ManipBlocks registration
REGISTER(BaseManipulation, SplitScalarField, "mimmo.SplitScalarField")
REGISTER(BaseManipulation, SplitVectorField, "mimmo.SplitVectorField")
};

#endif /* __SPLITFIELDS_HPP__ */
