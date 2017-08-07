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

#ifndef PORT_MANAGER_HPP
#define PORT_MANAGER_HPP

#include "portDefinitions.hpp"
#include "bitpit_common.hpp"
#include <unordered_map>
#include <vector>
#include <cassert>

namespace mimmo{

/*!
 *   \ingroup common
 *   \{
 */    
    
/*!
 * \struct InfoPort
 * \brief  collection of data functional to a port registration.
 * 
 * Contains a unique id identifying the port and two strings indentifying the container and the type 
 * of data associated to the port.
 */
struct InfoPort{
    
    long id;                 /**< integer label identifying the port*/
    std::string container;   /**< string label identifying the container associated to the port*/
    std::string datatype;    /**< string label identifying the data type associated to the port*/
    
    /*! Base constructor */
    InfoPort(){
        id=0; 
        container=""; 
        datatype="";
    };
    /*! Base Destructor */
    virtual ~InfoPort(){};
    /*! Copy constructor */
    InfoPort(const InfoPort & other){
        id = other.id;
        container = other.container;
        datatype = other.datatype;
    };
    /*! Copy operator */
    InfoPort & operator=(const InfoPort & other){
        id = other.id;
        container = other.container;
        datatype = other.datatype;
        return *this;
    };
};

/*!
 * \class PortManager
 * \brief Basic singleton for managing Ports declaration in mimmo. 
 *
 * PortManager is the reference singleton to declare and store ports of mimmo, together with their
 * containers and data types. PortManager can be "extended", that is more ports, containers and data type 
 * can be added, once mimmo is already built.
 * PortManager contains utilities to manage the ports addition.
 */
class PortManager {

private:
    /*!Private constructor of the singleton*/
    PortManager(){};
    /*!Prevent copy-construction for singleton*/
    PortManager(const PortManager&);
    /*!Prevent assignment for singleton*/
    PortManager& operator=(const PortManager&);
    /*! Destructor */
    ~PortManager() { 
    }; 

public:
    /*! Instance the singleton */
    static PortManager& instance(){
        static PortManager manager;
        return manager;
    }
    
    /*! Add a port to registered list. Beware: eventual compatibility between mimmo ports 
     * is verified assuring that they are sharing the same type of container and data. So be careful 
     * in the choice of names for container and datatype of a port. 
     * \param[in] name name of the port
     * \param[in] container_name name of the container associated to the port
     * \param[in] datatype_name name of the datatype associated to the port
     * \return current size of ports registered in the list
     */ 
    int addPort(const std::string name, const std::string container_name, const std::string datatype_name){
        
        if (containsPort(name) ){
            return (int) ports.size(); 
        }
        InfoPort temp;
        temp.container = container_name;
        temp.datatype = datatype_name;
        long idC=long(containers.size()), idD=long(datatypes.size());
        
        
        temp.id = long(ports.size());
        ports[name]= temp;
        
        if(!containsContainer(container_name)){
            containers[container_name]= idC;
        }
        
        if(!containsDatatype(datatype_name)){
            datatypes[datatype_name]= idD;
        }
        
        return (int) ports.size();
    }
    
    /*! Check if a port name is already registered 
     * \param[in] name name of the port
     * \return true if the port already exists
     */
    bool containsPort(const std::string name){
        return ports.count(name) > 0;
    }

    /*! Check if a container name is already registered 
     * \param[in] name name of the container
     * \return true if the container already exists
     */
    bool containsContainer(const std::string name){
        return containers.count(name) > 0;
    }
    
    /*! Check if a data type name is already registered 
     * \param[in] name name of the datatype
     * \return true if the datatype already exists
     */
    bool containsDatatype(const std::string name){
        return datatypes.count(name) > 0;
    }
    
    /*!\return the whole list of registered ports
     */
    std::unordered_map<std::string, InfoPort> mapRegisteredPorts(){
        return ports;
    }

    /*!\return the whole list of registered containers for ports
     */
    std::unordered_map<std::string, long> mapRegisteredContainers(){
        return containers;
    }
    
    /*!\return the whole list of registered datatypes for ports
     */
    std::unordered_map<std::string, long> mapRegisteredDataTypes(){
        return datatypes;
    }
    
    /*!
     * Return data attached to a Port in a InfoData struct
     * \param[in] name of the Port 
     * \return copy of InfoData struct associated to the Port
     */
     InfoPort getPortData(const std::string name){
        assert(containsPort(name));
        return ports[name];
    } 
    
    
private:
    std::unordered_map<std::string, InfoPort> ports;  /**< list of registered ports */
    std::unordered_map<std::string, long> containers; /**< list of registered port containers */
    std::unordered_map<std::string, long> datatypes;  /**< list of registered port data types */
    
};

};

/*!
 * \}
 */

#endif

