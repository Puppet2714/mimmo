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
 \ *---------------------------------------------------------------------------*/

#ifndef __MESHSELECTION_HPP__
#define __MESHSELECTION_HPP__

#include "BaseManipulation.hpp"
#include "MimmoObject.hpp"
#include "BasicShapes.hpp"
#include "MimmoGeometry.hpp"
#include "mimmo_common.hpp"

#include <memory>
#include <utility>


/*!
 * \ingroup Ports
 * \{
 */

//PORTS DEFINITION AS CONSTANTS
#ifndef M_GEOM
#define M_GEOM "M_GEOM" /**< Port dedicated to communication of pointers to a MimmoObject object*/
#endif

#ifndef M_VALUEB 
#define M_VALUEB "M_VALUEB" /**< Port dedicated to communication  of a conditional value [bool].*/
#endif

#ifndef M_VECTORLI
#define M_VECTORLI "M_VECTORLI" /**< Port dedicated to communication of a generic list of long integers [vector < long int > ]*/
#endif

#ifndef M_POINT 
#define M_POINT "M_POINT" /**< Port dedicated to communication of a 3D point coordinates [array < double,3 > ]*/
#endif

#ifndef M_GEOM2
#define M_GEOM2 "M_GEOM2" /**< Port dedicated to communication of pointers to a MimmoObject object*/
#endif

#ifndef M_AXES 
#define M_AXES "M_AXES" /**< Port dedicated to communication  of a reference system of axis [ array < array < double,3 >,3 > ].*/
#endif

#ifndef M_INFLIMITS
#define M_INFLIMITS "M_INFLIMITS" /**< Port dedicated to communication of inferior limits for building an elemental shape [array < double,3 > ]*/
#endif

#ifndef M_SPAN 
#define M_SPAN "M_SPAN" /**< Port dedicated to communication of dimensions of an elemental object (width, lenght etc...) [array < double,3 > ]*/
#endif

#ifndef M_VECTORSI 
#define M_VECTORSI "M_VECTORSI" /**< Port dedicated to communication  of a generic list of short integers [vector < short int > ].*/
#endif

#ifndef M_VALUESI
#define M_VALUESI "M_VALUESI" /**< Port dedicated to communication of a scalar short integer value [short int].*/
#endif

#ifndef M_SCALARFIELD 
#define M_SCALARFIELD "M_SCALARFIELD" /**< Port dedicated to communication of a generic scalar field [ vector < double > ].*/
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

#ifndef MC_ARRAY3 
#define MC_ARRAY3 "MC_ARRAY3" /**< std::array< . , 3> container identifier*/
#endif

#ifndef MC_ARR3ARR3
#define MC_ARR3ARR3 "MC_ARR3ARR3" /**< std::array< std::array< . , 3 > , 3 > container identifier*/
#endif

/*!
 * \}
 */

/*!
 * \ingroup PortData
 * \{
 */
#ifndef MD_BOOL
#define MD_BOOL "MD_BOOL" /**< boolean data identifier*/
#endif

#ifndef MD_LONG 
#define MD_LONG "MD_LONG" /**< long integer data identifier*/
#endif

#ifndef MD_MIMMO_ 
#define MD_MIMMO_ "MD_MIMMO_" /**< MimmoObject pointer data identifier*/
#endif

#ifndef MD_FLOAT
#define MD_FLOAT "MD_FLOAT" /**< float/double data identifier*/
#endif

#ifndef MD_SHORT
#define MD_SHORT "MD_SHORT" /**< short integer data identifier*/
#endif

/*!
 * \}
 */


namespace mimmo{

/*!
 * \enum SelectionType
 * \ingroup geohandlers
 * \brief Enum class for choiche of method to select sub-patch
 * of a tessellated mesh.
 */
enum class SelectionType{
    UNDEFINED    = 0,
    BOX          = 1,
    CYLINDER     = 2,
    SPHERE       = 3,
    MAPPING      = 4,
    PID          = 5,
    BOXwSCALAR   = 11
};

/*!
 * \class GenericSelection
 * \ingroup geohandlers
 * \brief Abstract Interface for selection classes
 * 
 * Class/BaseManipulation Object managing selection of sub-patches of a 3D open 
 * unstructured surface/volume mesh.
 *
 * Ports available in GenericSelection Class :
 *
 *    =========================================================
 *
     |                   Port Input       ||                               |
     |----------------|----------------------|-------------------|
     |<B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VALUEB       | setDual              | (MC_SCALAR, MD_BOOL)    |
     | M_GEOM         | setGeometry          | (MC_SCALAR, MD_MIMMO_)  |


     |             Port Output     ||                                      |
     |----------------|---------------------|--------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VECTORLI     | constrainedBoundary | (MC_VECTOR, MD_LONG)     |
     | M_GEOM         | getPatch            | (MC_SCALAR, MD_MIMMO_)   |

 *    =========================================================
 *
 */
class GenericSelection: public mimmo::BaseManipulation {

protected:

    SelectionType                   m_type;      /**< Type of enum class SelectionType for selection method */
    std::unique_ptr<MimmoObject>    m_subpatch;  /**< Pointer to result sub-patch */
    int                             m_topo;      /**< 1 = surface (default value), 2 = volume, 3 = points cloud */
    bool                            m_dual;      /**< False selects w/ current set up, true gets its "negative". False is default. */
public:

    GenericSelection();
    virtual ~GenericSelection();
    GenericSelection(const GenericSelection & other);
    GenericSelection & operator=(const GenericSelection & other);

    void    buildPorts();

    SelectionType    whichMethod();
    virtual void             setGeometry(MimmoObject *);
    void             setDual(bool flag=false);

    const MimmoObject*    getPatch()const;
    MimmoObject    *        getPatch();
    bool                isDual();

    livector1D    constrainedBoundary();

    void        execute();

    virtual void plotOptionalResults();

protected:
    /*!
     * Extract selection from target geometry
     */
    virtual livector1D extractSelection() = 0;

};


/*!
 * \class SelectionByBox
 * \ingroup geohandlers
 * \brief Selection through volume box primitive.
 * 
 * Selection Object managing selection of sub-patches of a 3D open 
 * unstructured surface mesh. Select all simplex contained in a box.
 * 
 * Ports available in SelectionByBox Class :
 *
 *    =========================================================

     |                   Port Input  ||                                       |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_POINT        | setOrigin           | (MC_ARRAY3, MD_FLOAT)       |
     | M_AXES         | setRefSystem        | (MC_ARR3ARR3, MD_FLOAT)     |
     | M_SPAN         | setSpan             | (MC_ARRAY3, MD_FLOAT)       |


     |             Port Output        ||                                      |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |


     Inherited from GenericSelection

     |                   Port Input   ||                                   |
     |----------------|----------------------|-------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VALUEB       | setDual              | (MC_SCALAR, MD_BOOL)    |
     | M_GEOM         | setGeometry          | (MC_SCALAR, MD_MIMMO_)  |


     |             Port Output          ||                                 |
     |----------------|---------------------|--------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VECTORLI     | constrainedBoundary | (MC_VECTOR, MD_LONG)     |
     | M_GEOM         | getPatch            | (MC_SCALAR, MD_MIMMO_)   |

 *    ===============================================================================
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 * - <B>ClassName</B>: name of the class as <tt>mimmo.SelectionByBox</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class;
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Proper of the class:
 
 * - <B>Dual</B>: boolean to get straight what given by selection method or its exact dual;
 * - <B>Origin</B>: array of 3 doubles identifying origin;
 * - <B>Span</B>: span of the box (width, height, depth);
 * - <B>RefSystem</B>: reference system of the box: \n
 *                <tt>\<RefSystem\> \n
 *                      \<axis0\> 1.0 0.0 0.0 \</axis0\> \n
 *                      \<axis1\> 0.0 1.0 0.0 \</axis1\> \n
 *                      \<axis2\> 0.0 0.0 1.0 \</axis2\> \n
 *                  \</RefSystem\> </tt> \n
 *
 * Geometry has to be mandatorily passed through port.
 *
 */
class SelectionByBox: public GenericSelection, public mimmo::Cube {

public:
    SelectionByBox();
    SelectionByBox(const bitpit::Config::Section & rootXML);
    SelectionByBox(darray3E origin, darray3E span, MimmoObject * target);
    virtual ~SelectionByBox();
    SelectionByBox(const SelectionByBox & other);
    SelectionByBox & operator=(const SelectionByBox & other);

    void buildPorts();

    void clear();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="" );
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="" );

protected:
    livector1D extractSelection();
};

/*!
 * \class SelectionByCylinder
 * \ingroup geohandlers
 * \brief Selection through cylinder primitive.
 * 
 * Selection Object managing selection of sub-patches of a 3D open 
 * unstructured surface mesh. Select all simplex contained in a cylinder.
 *
 * Ports available in SelectionByCylinder Class :
 *
 *    =========================================================

     |                   Port Input  ||                                       |
     |----------------|----------------------|----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_POINT        | setOrigin            | (MC_ARRAY3, MD_FLOAT)      |
     | M_AXES         | setRefSystem         | (MC_ARR3ARR3, MD_FLOAT)    |
     | M_SPAN         | setSpan              | (MC_ARRAY3, MD_FLOAT)      |
     | M_INFLIMITS    | setInfLimits         | (MC_ARRAY3, MD_FLOAT)      |

     |             Port Output        ||                                      |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |


     Inherited from GenericSelection

     |                   Port Input   ||                                   |
     |----------------|----------------------|-------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VALUEB       | setDual              | (MC_SCALAR, MD_BOOL)    |
     | M_GEOM         | setGeometry          | (MC_SCALAR, MD_MIMMO_)  |


     |             Port Output          ||                                 |
     |----------------|---------------------|--------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VECTORLI     | constrainedBoundary | (MC_VECTOR, MD_LONG)     |
     | M_GEOM         | getPatch            | (MC_SCALAR, MD_MIMMO_)   |

 *    =========================================================
 *
 * The xml available parameters, sections and subsections are the following :
 
 * Inherited from BaseManipulation:
 * - <B>ClassName</B>: name of the class as <tt>mimmo.SelectionByCylinder</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class;
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Proper of the class:
 * - <B>Dual</B>: boolean to get straight what given by selection method or its exact dual;
 * - <B>Origin</B>: array of 3 doubles identifying origin of cylinder;
 * - <B>Span</B>: span of the cylinder (base radius, angular azimuthal width, height);
 * - <B>RefSystem</B>: reference system of the cylinder(axis2 along the cylinder's height): \n
 *                <tt>\<RefSystem\> \n
 *                      \<axis0\> 1.0 0.0 0.0 \</axis0\> \n
 *                      \<axis1\> 0.0 1.0 0.0 \</axis1\> \n
 *                      \<axis2\> 0.0 0.0 1.0 \</axis2\> \n
 *                  \</RefSystem\> </tt> \n
 * - <B>InfLimits</B>: set starting point for each cylindrical coordinate. Useful to assign different starting angular coordinate to azimuthal width.
 *
 *
 * Geometry has to be mandatorily passed through port.
 *
 */
class SelectionByCylinder: public GenericSelection, public mimmo::Cylinder {

public:
    SelectionByCylinder();
    SelectionByCylinder(const bitpit::Config::Section & rootXML);
    SelectionByCylinder(darray3E origin, darray3E span, double infLimTheta, darray3E mainAxis, MimmoObject * target);
    virtual ~SelectionByCylinder();
    SelectionByCylinder(const SelectionByCylinder & other);
    SelectionByCylinder & operator=(const SelectionByCylinder & other);

    void buildPorts();

    void clear();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="" );
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="" );

protected:
    livector1D extractSelection();
};

/*!
 * \class SelectionBySphere
 * \ingroup geohandlers
 * \brief Selection through sphere primitive.
 * 
 * Selection Object managing selection of sub-patches of a 3D open 
 * unstructured surface mesh. Select all simplex contained in a sphere.
 *
 * Ports available in SelectionBySphere Class :
 *
 *    =========================================================
 *
     |                   Port Input  ||                                       |
     |----------------|----------------------|----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_POINT        | setOrigin            | (MC_ARRAY3, MD_FLOAT)      |
     | M_AXES         | setRefSystem         | (MC_ARR3ARR3, MD_FLOAT)    |
     | M_SPAN         | setSpan              | (MC_ARRAY3, MD_FLOAT)      |
     | M_INFLIMITS    | setInfLimits         | (MC_ARRAY3, MD_FLOAT)      |

     |             Port Output    ||                                          |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |


     Inherited from GenericSelection

     |                   Port Input   ||                                   |
     |----------------|----------------------|-------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VALUEB       | setDual              | (MC_SCALAR, MD_BOOL)    |
     | M_GEOM         | setGeometry          | (MC_SCALAR, MD_MIMMO_)  |


     |             Port Output          ||                                 |
     |----------------|---------------------|--------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VECTORLI     | constrainedBoundary | (MC_VECTOR, MD_LONG)     |
     | M_GEOM         | getPatch            | (MC_SCALAR, MD_MIMMO_)   |


 *    =========================================================
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 
 * - <B>ClassName</B>: name of the class as <tt>mimmo.SelectionBySphere</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class;
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Proper of the class:
 * - <B>Dual</B>: boolean to get straight what given by selection method or its exact dual;
 * - <B>Origin</B>: array of 3 doubles identifying origin of sphere;
 * - <B>Span</B>: span of the sphere (radius, angular azimuthal width, angular polar width);
 * - <B>RefSystem</B>: reference system of the sphere: \n
 *                <tt>\<RefSystem\> \n
 *                      \<axis0\> 1.0 0.0 0.0 \</axis0\> \n
 *                      \<axis1\> 0.0 1.0 0.0 \</axis1\> \n
 *                      \<axis2\> 0.0 0.0 1.0 \</axis2\> \n
 *                  \</RefSystem\> </tt> \n
 * - <B>InfLimits</B>: set starting point for each spherical coordinate. Useful to assign different starting angular coordinate to azimuthal width/ polar width.
 *
 *
 * Geometry has to be mandatorily passed through port.
 
 *
 */
class SelectionBySphere: public GenericSelection, public mimmo::Sphere {

public:
    SelectionBySphere();
    SelectionBySphere(const bitpit::Config::Section & rootXML);
    SelectionBySphere(darray3E origin, darray3E span, double infLimTheta, double infLimPhi, MimmoObject * target);
    virtual ~SelectionBySphere();
    SelectionBySphere(const SelectionBySphere & other);
    SelectionBySphere & operator=(const SelectionBySphere & other);

    void buildPorts();

    void clear();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="" );
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="" );

protected:
    livector1D extractSelection();
};

/*!
 * 
 * \class SelectionByMapping
 * \ingroup geohandlers
 * \brief Selection mapping external surfaces on the target mesh.
 * 
 * Selection Object managing selection of sub-patches of a 3D open 
 * unstructured surface mesh. Extract portion of mesh in common between 
 * a target geometry and a second one, provided externally. Extraction criterium
 * is based on euclidean nearness, within a prescribed tolerance.
 *
 * Ports available in SelectionByMapping Class :
 *
 *    =========================================================
 *
     |                   Port Input     ||                                                     |
     |----------------|--------------------------|----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_GEOM2        | addMappingGeometry       | (MC_SCALAR, MD_MIMMO_)     |

     |             Port Output       ||                                       |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |


     Inherited from GenericSelection

     |                   Port Input   ||                                   |
     |----------------|----------------------|-------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VALUEB       | setDual              | (MC_SCALAR, MD_BOOL)    |
     | M_GEOM         | setGeometry          | (MC_SCALAR, MD_MIMMO_)  |


     |             Port Output          ||                                 |
     |----------------|---------------------|--------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VECTORLI     | constrainedBoundary | (MC_VECTOR, MD_LONG)     |
     | M_GEOM         | getPatch            | (MC_SCALAR, MD_MIMMO_)   |

 *    =========================================================
 *
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 * - <B>ClassName</B>: name of the class as <tt>mimmo.SelectionByMapping</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class;
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Proper of the class:
 * - <B>Topology</B>: number indentifying topology of tesselated mesh. 1-surfaces, 2-voume. no other types are supported;
 * - <B>Dual</B>: boolean to get straight what given by selection method or its exact dual;
 * - <B>Tolerance</B>: proximity threshold to activate mapping;
 * - <B>Files</B>: list of external files to map on the target surface: \n
 *                <tt>\<Files\> \n
 *                      \<file0\> \n
 *                          \<fullpath> absolute path to your file with extension \<fullpath\> \n
 *                          \<tag\> tag extension as supported format STL,NAS,STVTU,etc...\<tag\> \n
 *                      \<file0\> \n
 *                      \<file1\> \n
 *                          \<fullpath> absolute path to your file with extension \<fullpath\> \n
 *                          \<tag\> tag extension as supported format STL,NAS,STVTU,etc...\<tag\> \n
 *                      \<file1\> \n
 *                      ... 
 *                  \</Files\> </tt> \n
 *
 * Geometry has to be mandatorily passed through port.
 */
class SelectionByMapping: public GenericSelection {

private:
    double                                  m_tolerance;     /**< tolerance for proximity detection*/
    std::unordered_map<std::string, int>    m_geolist;      /**< list of file for geometrical proximity check*/
    std::unordered_set<MimmoObject*>        m_mimmolist;    /**< list of mimmo objects for geometrical proximity check*/
    std::vector<std::set< int > >           m_allowedType;  /**< list of FileType actually allowed for the target geometry type*/
public:
    SelectionByMapping(int topo = 1);
    SelectionByMapping(const bitpit::Config::Section & rootXML);
    SelectionByMapping(std::unordered_map<std::string, int> & geolist, MimmoObject * target, double tolerance);
    virtual ~SelectionByMapping();
    SelectionByMapping(const SelectionByMapping & other);
    SelectionByMapping & operator=(const SelectionByMapping & other);

    void    buildPorts();

    double    getTolerance();
    void     setTolerance(double tol=1.e-8);

    void     setGeometry(MimmoObject * geometry);

    const std::unordered_map<std::string, int> &     getFiles() const;
    void    setFiles(std::unordered_map<std::string,int> );
    void    addFile(std::pair<std::string,int> );
    void     removeFile(std::string);
    void     removeFiles();
    void    addMappingGeometry(MimmoObject * obj);
    void    removeMappingGeometries();

    void clear();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="" );
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="" );

protected:
    livector1D extractSelection();

private:
    livector1D getProximity(std::pair<std::string, int> val);
    livector1D getProximity(MimmoObject * obj);
    svector1D extractInfo(std::string);
};

/*!
 * \class SelectionByPID
 * \ingroup geohandlers
 * \brief Selection using target mesh Part Identifiers.
 * 
 * Selection Object managing selection of sub-patches of a 3D open 
 * unstructured surface mesh. Extract portion of mesh getting its PID in 
 * target geometry.
 *
 * Ports available in SelectionByPID Class :
 *
 *    =========================================================
 *
     |                   Port Input   ||                                      |
     |----------------|----------------------|----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VECTORSI     | setPID               | (MC_VECTOR, MD_SHORT)      |
     | M_VALUESI      | setPID               | (MC_SCALAR, MD_SHORT)      |

     |             Port Output     ||                                         |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |


     Inherited from GenericSelection

     |                   Port Input   ||                                   |
     |----------------|----------------------|-------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VALUEB       | setDual              | (MC_SCALAR, MD_BOOL)    |
     | M_GEOM         | setGeometry          | (MC_SCALAR, MD_MIMMO_)  |


     |             Port Output          ||                                 |
     |----------------|---------------------|--------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VECTORLI     | constrainedBoundary | (MC_VECTOR, MD_LONG)     |
     | M_GEOM         | getPatch            | (MC_SCALAR, MD_MIMMO_)   |

 *    =========================================================
 *
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 *
 * - <B>ClassName</B>: name of the class as <tt>mimmo.SelectionByMapping</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class;
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 * Proper of the class:
 * - <B>Dual</B>: boolean to get straight what given by selection method or its exact dual;
 * - <B>nPID</B>: number of PID to be selected relative to target geometry; 
 * - <B>PID</B>: list of PID (separated by blank spaces) to be selected relative to target geometry; 
 *
 * Geometry has to be mandatorily passed through port.
 */
class SelectionByPID: public GenericSelection {

private:
    std::unordered_set<short>       m_setPID;    /**< list of pid given by the user */
    std::unordered_map<short,bool> m_activePID; /**< list of pid available in geometry, to be flagged as active or not*/

public:
    SelectionByPID();
    SelectionByPID(const bitpit::Config::Section & rootXML);
    SelectionByPID(shivector1D & pidlist, MimmoObject * target);
    virtual ~SelectionByPID();
    SelectionByPID(const SelectionByPID & other);
    SelectionByPID & operator=(const SelectionByPID & other);

    void    buildPorts();

    shivector1D getPID();
    shivector1D    getActivePID(bool active= true);

    void     setGeometry(MimmoObject * );
    void     setPID(short i = -1);
    void     setPID(shivector1D);

    void     removePID(short i = -1);
    void     removePID(shivector1D);

    void     syncPIDList();
    void     clear();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="" );
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="" );

protected:
    livector1D extractSelection();

};

/*!
 * \class SelectionByBoxWithScalar
 * \ingroup geohandlers
 * \brief Selection through volume box primitive.
 * 
 * Selection Object managing selection of sub-patches
 * of a 3D unstructured surface mesh.
 * Select all simplex contained in a box and extract
 * a scalar field if it is present.
 *
 * Ports available in SelectionByBoxWithScalar Class :
 *
 *  =========================================================
 *
     |                   Port Input      ||                                   |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_SCALARFIELD  | setField            | (MC_VECTOR, MD_FLOAT)       |


     |             Port Output    ||                                          |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_SCALARFIELD  | getField            | (MC_VECTOR, MD_FLOAT)       |

   Inherited from SelectionByBox

     |                   Port Input  ||                                       |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_POINT        | setOrigin           | (MC_ARRAY3, MD_FLOAT)       |
     | M_AXES         | setRefSystem        | (MC_ARR3ARR3, MD_FLOAT)     |
     | M_SPAN         | setSpan             | (MC_ARRAY3, MD_FLOAT)       |


     |             Port Output       ||                                       |
     |----------------|---------------------|-----------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |


     Inherited from GenericSelection

     |                   Port Input   ||                                   |
     |----------------|----------------------|-------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VALUEB       | setDual              | (MC_SCALAR, MD_BOOL)    |
     | M_GEOM         | setGeometry          | (MC_SCALAR, MD_MIMMO_)  |


     |             Port Output          ||                                 |
     |----------------|---------------------|--------------------|
     | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | M_VECTORLI     | constrainedBoundary | (MC_VECTOR, MD_LONG)     |
     | M_GEOM         | getPatch            | (MC_SCALAR, MD_MIMMO_)   |

 *  ===============================================================================
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * Inherited from BaseManipulation:
 * - <B>ClassName</B>: name of the class as <tt>mimmo.SelectionByBoxWithScalar</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>PlotInExecution</B>: boolean 0/1 print optional results of the class;
 * - <B>OutputPlot</B>: target directory for optional results writing.
 *
 *Inherited from SelectionByBox:
 
 * - <B>Dual</B>: boolean to get straight what given by selection method or its exact dual;
 * - <B>Origin</B>: array of 3 doubles identifying origin;
 * - <B>Span</B>: span of the box (width, height, depth);
 * - <B>RefSystem</B>: reference system of the box: \n
 *                <tt>\<RefSystem\> \n
 *                      \<axis0\> 1.0 0.0 0.0 \</axis0\> \n
 *                      \<axis1\> 0.0 1.0 0.0 \</axis1\> \n
 *                      \<axis2\> 0.0 0.0 1.0 \</axis2\> \n
 *                  \</RefSystem\> </tt> \n
 *
 * Geometry and fields have to be mandatorily passed/recovered through ports.
 */
class SelectionByBoxWithScalar: public SelectionByBox{
protected:
    dvector1D     m_field;          /**<Scalar field attached to the patch
                                        (related to the whole patch before execution,
                                        related to the selected patch after execution).*/

public:
    SelectionByBoxWithScalar();
    SelectionByBoxWithScalar(const bitpit::Config::Section & rootXML);
    SelectionByBoxWithScalar(darray3E origin, darray3E span, MimmoObject * target);
    virtual ~SelectionByBoxWithScalar();
    SelectionByBoxWithScalar(const SelectionByBoxWithScalar & other);
    SelectionByBoxWithScalar & operator=(const SelectionByBoxWithScalar & other);

    void buildPorts();

    void clear();

    void        setField(dvector1D);
    dvector1D   getField();

    void execute();

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="" );
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="" );

    void plotOptionalResults();

};



//Ports registration
REGISTER_PORT(M_GEOM, MC_SCALAR, MD_MIMMO_)
REGISTER_PORT(M_GEOM2, MC_SCALAR, MD_MIMMO_)
REGISTER_PORT(M_VALUEB, MC_SCALAR, MD_BOOL)
REGISTER_PORT(M_VECTORLI, MC_VECTOR, MD_LONG)
REGISTER_PORT(M_POINT, MC_ARRAY3, MD_FLOAT)
REGISTER_PORT(M_AXES, MC_ARR3ARR3, MD_FLOAT)
REGISTER_PORT(M_SPAN, MC_ARRAY3, MD_FLOAT)
REGISTER_PORT(M_INFLIMITS, MC_ARRAY3, MD_FLOAT)
REGISTER_PORT(M_VECTORSI, MC_VECTOR, MD_SHORT)
REGISTER_PORT(M_VALUESI, MC_SCALAR, MD_SHORT)
REGISTER_PORT(M_SCALARFIELD, MC_VECTOR, MD_FLOAT)

//ManipBlocks registration
REGISTER(BaseManipulation, SelectionByBox,"mimmo.SelectionByBox")
REGISTER(BaseManipulation, SelectionByCylinder, "mimmo.SelectionByCylinder")
REGISTER(BaseManipulation, SelectionBySphere,"mimmo.SelectionBySphere")
REGISTER(BaseManipulation, SelectionByMapping, "mimmo.SelectionByMapping")
REGISTER(BaseManipulation, SelectionByPID, "mimmo.SelectionByPID")
REGISTER(BaseManipulation, SelectionByBoxWithScalar, "mimmo.SelectionByBoxWithScalar")
};

/*!
 * \}
 */


#endif /* __MESHSELECTION_HPP__ */
