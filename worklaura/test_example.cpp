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

#include "mimmo_iogeneric.hpp"
using namespace std;
using namespace bitpit;
using namespace mimmo;



// =================================================================================== //
/*!
 * Reading & managing a quad planar mesh as a degenerate volume mesh.
 */
int test4() {
	
//     MimmoObject * geo = new MimmoObject(2,2);
//     
//     dvecarr3E vertices(9, {{0,0,0}});
//     livector2D conns(4, livector1D(4,0));
//     
//     vertices[0][0] = -0.5; vertices[0][1] = -0.5;
//     vertices[1][1] = -0.5;
//     vertices[2][0] = -0.5;
//     vertices[3][0] =  0.5; vertices[3][1] =  -0.5;
//     vertices[5][0] =  -0.5; vertices[5][1] = 0.5;
//     vertices[6][0] =  0.5; vertices[6][1] = 0.5;
//     vertices[7][1] =  0.5;
//     vertices[8][0] = 0.5;
//     
//     conns[0] = {{0,1,4,2}};
//     conns[1] = {{1,3,8,4}};
//     conns[2] = {{4,8,6,7}};
//     conns[3] = {{2,4,7,5}};
//     
//     for(auto & vv : vertices){
//         geo->addVertex(vv);
//     }
//     short pid=0;
//     for(auto & cc: conns){
//         geo->addConnectedCell(cc,bitpit::ElementInfo::Type::QUAD, pid);
//         ++pid;
//     }
//     
//     geo->cleanGeometry();
//     
//    MimmoGeometry * writer = new MimmoGeometry();
//     writer->setIOMode(1);
//     writer->setDir(".");
//     writer->setFilename("planarquads_mimmo");
//     writer->setFileType(FileType::SQVTUPLANAR);
//     writer->setGeometry(geo);
//     writer->setCodex(false);
//     writer->execute();
//     
//     delete geo;
//     delete writer;
    
    
    MimmoGeometry * reader = new MimmoGeometry();
    reader->setIOMode(2);
    reader->setReadDir("geodata");
    reader->setReadFilename("planarquads_mimmo");
    reader->setReadFileType(FileType::SQVTUPLANAR);
    reader->setWriteDir(".");
    reader->setWriteFilename("out-example");
    reader->setWriteFileType(FileType::SQVTUPLANAR);
    reader->setCodex(false);
    reader->execute();
    
    auto obj = reader->getGeometry();
    
    obj->getPatch()->buildAdjacencies();
    obj->getPatch()->buildInterfaces();
    obj->getPatch()->update();
    
    std::cout<<obj->getNCells()<<std::endl;
    
    bitpit::VolUnstructured * vol= static_cast<bitpit::VolUnstructured * >(obj->getPatch());
    
    for(auto &interf : vol->getInterfaces()){
        auto normal = vol->evalInterfaceNormal(interf.getId());
        std::cout<<interf.getId()<<'\t'<<"normal: "<<normal<<std::endl;
    }
    
    for(auto &cell : vol->getCells()){
        auto volume = vol->evalCellVolume(cell.getId());
        std::cout<<cell.getId()<<'\t'<<"volume: "<<volume<<std::endl;
    }
    
    delete reader;
    obj = NULL;
    vol = NULL;
    bool check = true;
    return int(!check);
}

// =================================================================================== //

int main( int argc, char *argv[] ) {

	BITPIT_UNUSED(argc);
	BITPIT_UNUSED(argv);
	
#if ENABLE_MPI==1
	MPI::Init(argc, argv);

	{
#endif
		/**<Calling mimmo Test routines*/

        int val = test4() ;

#if ENABLE_MPI==1
	}

	MPI::Finalize();
#endif
	
	return val;
}
