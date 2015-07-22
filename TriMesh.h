#ifndef TRIMESH_H
#define TRIMESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
//#include <deque>

struct TriMeshTraits : public OpenMesh::DefaultTraits
{
    typedef OpenMesh::Vec3d Point;
    typedef OpenMesh::Vec3d Normal;

    // Add normal property to vertices and faces, and no other for now
    // Later on, you might want to add color properties for visulization
    VertexAttributes  ( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status );
    FaceAttributes    ( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status );
    EdgeAttributes    ( OpenMesh::Attributes::Status );
};


typedef OpenMesh::TriMesh_ArrayKernelT<TriMeshTraits> TriMesh;

//////////////////////////////////////////////////////////////
// Implementations
//////////////////////////////////////////////////////////////

TriMesh verts_to_trimesh(const GLfloat verts_arr[], int asize) {
    std::vector<int> verts(verts_arr, verts_arr + asize);

    TriMesh mesh;
    std::vector<TriMesh::VertexHandle> vhandles(asize);

    for (int i = 0; i < asize; i += 3) { 
        TriMesh::Point pt = TriMesh::Point(verts[i + 0], verts[i + 1], verts[i + 2]);
        TriMesh::VertexHandle vhandle = mesh.add_vertex(pt);
        vhandles.push_back(vhandle);
    }
    
    std::vector<TriMesh::VertexHandle> face_vhandles(3);
    for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ) {
        //std::cout << "face: " << v_it << "/" << vhandles.size() << std::endl;
        //face_vhandles[2] = *v_it++;
        //face_vhandles[1] = *v_it++;
        //face_vhandles[0] = *v_it++;
        face_vhandles[0] = *v_it++;
        face_vhandles[1] = *v_it++;
        face_vhandles[2] = *v_it++;
        mesh.add_face(face_vhandles);
    }

    return mesh;
}


void print_normals(std::vector<double> normals) {
  // print the normals
  for (size_t i = 0; i < normals.size(); i += 3) {

    std::cout << "Normal #" << i / 3 << ": " << normals[i]
                                     << ", " << normals[i+1]
                                     << ", " << normals[i+2] << std::endl;
  }
}

std::vector<double> mesh_normals(TriMesh mesh) {
  mesh.request_vertex_normals();
  mesh.request_face_normals();
  mesh.update_normals();

  std::vector<double> normals(mesh.n_vertices());

  for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++) {
    TriMesh::Normal normal = mesh.normal(*v_it);
    normals.push_back(normal[0]);
    normals.push_back(normal[1]);
    normals.push_back(normal[2]);
  }

  // for (TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
  //   TriMesh::Normal normal = mesh.normal(*f_it);
  //   std::cout << "Face #" << *f_it << ": (" /*<< mesh.face(*f_it)*/ << ") " << normal << std::endl;
  // }

  return normals;
}

#endif // TRIMESH_H

