#include "Cylinder.h"
#include "Mesh.h"
#include <iostream>


float radius = 0.5f;

void Cylinder::drawTriangleMeshFromFaces() {
    // Get current mode
    // GLint shadeModel;
    // glGetIntegerv(GL_SHADE_MODEL, &shadeModel);

    // // Draw side face
    // glBegin(GL_TRIANGLES);

    // for (Mesh* g : this->graphs) {
    //     for (Face* face : *g->getFaceIterator()) {
    //         Vertex* const* verts = face->getVertices();
    //         for (int i = 0; i < 3; i++)
    //         {
    //             Vertex* v = verts[i];
    //             if (shadeModel == GL_SMOOTH)
    //             {
    //                 normalizeNormal(v->getNormals());
    //             }
    //             else if (shadeModel == GL_FLAT)
    //             {
    //                 normalizeNormal(face->getFaceNormal());
    //             }

    //             glm::vec3 pos = v->getPos();
    //             glVertex3f(pos.x, pos.y, pos.z);
    //         }
    //     }
    // }
    // glEnd();

}

void Cylinder::draw() {
    // drawTriangleMeshFromFaces();
}

void Cylinder::drawNormal() {
    // glColor3f(1.0f, .0f, .0f);

    // glBegin(GL_LINES);
    // for (Mesh* g : this->graphs) {
    //     for (Vertex* v : *g->getVertexIterator()) {
    //         const glm::vec3& normal = v->getNormals();
    //         const glm::vec3& pos = (v->getPos());

    //         glVertex3f(pos.x, pos.y, pos.z);
    //         glVertex3f(pos.x + normal.x * .1f, pos.y + normal.y * .1f,
    //             pos.z + normal.z * .1f);
    //     }
    // }
    // glEnd();
}

void Cylinder::calculate() {
    // int vcount = (m_segmentsY + 1) * m_segmentsX;
    // int fcount = m_segmentsX * m_segmentsY * 2;
    // //  Create a new graph to store the side shape
    // Mesh* side = new Mesh(vcount, fcount);
    // Mesh* topFace = new Mesh(m_segmentsX + 1, m_segmentsX + 1);
    // Mesh* bottomFace = new Mesh(m_segmentsX + 1, m_segmentsX + 1);

    // // this->clearGraphs();

    // float stepAngle = 360.0f / m_segmentsX;
    // float stepY = 1.0f / m_segmentsY;

    // this->vertices.clear();
    // this->Faces.clear();


    // for (int i = 0; i <= m_segmentsY; i++) {
    //     float y = i * stepY - radius;

    //     for (int j = 0; j < m_segmentsX; j++) {
    //         float angle = glm::radians(j * stepAngle);
    //         float x = radius * glm::cos(angle);
    //         float z = radius * glm::sin(angle);

    //         glm::vec3 position(x, y, z);
    //         // Calculate normal
    //         glm::vec3 normal = glm::vec3(x, 0.0f, z);

    //         // Build vertex with position and normal
    //         Vertex* v = new Vertex(position);

    //         v->setNormal(glm::vec3(x, 0.0f, z));
    //         side->addVertex(v);
    //         /* Add vertice on top and bottom to their graph */

    //         if (i == 0) // This point to the bottom
    //         {
    //             Vertex* vbottom = new Vertex(position);
    //             glm::vec3 bottomNormal = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
    //             vbottom->setNormal(bottomNormal);
    //             bottomFace->addVertex(vbottom);
    //         }

    //         if (i == m_segmentsY) // This point to the top
    //         {
    //             Vertex* vtop = new Vertex(position);
    //             glm::vec3 topNormal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
    //             vtop->setNormal(topNormal);
    //             topFace->addVertex(vtop);
    //         }
    //     }
    // }

    // // Get the vertices list for calculating the face 
    // Vertex** verts = side->getVertices();
    // // Add all side face to side shape
    // for (int i = 0; i < m_segmentsY; ++i) {
    //     for (int j = 0; j < m_segmentsX; ++j) {
    //         int index1 = i * m_segmentsX + j;
    //         int index2 = (j + 1) % m_segmentsX + i * m_segmentsX;
    //         int index3 = (i + 1) * m_segmentsX + j;
    //         int index4 = (i + 1) * m_segmentsX + ((j + 1) % m_segmentsX);

    //         Face* f1 = new Face(verts[index1], verts[index3], verts[index4]);

    //         // verts[index1]->addFace(f1);
    //         // verts[index3]->addFace(f1);
    //         // verts[index4]->addFace(f1);

    //         Face* f2 = new Face(verts[index4], verts[index2], verts[index1]);

    //         // verts[index4]->addFace(f2);
    //         // verts[index2]->addFace(f2);
    //         // verts[index1]->addFace(f2);

    //         side->addFace(f1);
    //         side->addFace(f2);
    //     }
    // }

    // /* --------------------- Add the top and bottom center points to the graph --------------------*/
    // glm::vec3 topCenter(0, 1.0f - radius, 0);
    // glm::vec3 bottomCenter(0, -radius, 0);

    // // int topCenterIndex = this->vertices.size();
    // glm::vec3 topNormal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
    // Vertex* top = new Vertex(topCenter);
    // top->setNormal(topNormal);
    // topFace->addVertex(top);

    // // int bottomCenterIndex = this->vertices.size();
    // glm::vec3 bottomNormal = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
    // Vertex* bottom = new Vertex(bottomCenter);
    // bottom->setNormal(bottomNormal);
    // bottomFace->addVertex(bottom);



    // // Calculate and add the top and bottom face

    // Vertex** topVertices = topFace->getVertices();
    // Vertex** bottomVertices = bottomFace->getVertices();

    // for (int i = 0; i < m_segmentsX; i++)
    // {
    //     int index1 = i;
    //     int index2 = (i + 1) % (m_segmentsX);
    //     Face* f1 = new Face(topVertices[m_segmentsX], topVertices[index2], topVertices[index1]);
    //     topFace->addFace(f1);
    // }

    // for (int j = 0; j < m_segmentsX; j++)
    // {
    //     int index1 = j;
    //     int index2 = (j + 1) % (m_segmentsX);
    //     Face* f2 = new Face(bottomVertices[m_segmentsX], bottomVertices[index1], bottomVertices[index2]);
    //     bottomFace->addFace(f2);
    // }

    // // Push the graph to STL for managing
    // this->graphs.push_back(side);
    // this->graphs.push_back(topFace);
    // this->graphs.push_back(bottomFace);

    // Print total size
    // int verticesSize = 0, facesSize = 0;
    // for (Mesh* g : this->graphs)
    // {
    //     verticesSize += g->getVertices().size();
    //     facesSize += g->getFaces().size();
    // }



    // std::cout << "Vertices count: " << verticesSize - 2 * m_segmentsX << std::endl;
    // std::cout << "Faces count: " << facesSize << std::endl;
}

double Cylinder::intersect(glm::vec3 eyePosition_world, glm::vec3 rayv_world, glm::mat4 viewMatrix_o2w) {
    //viewMatrix is matrix object to world
    glm::mat4 ViewMatrix_w2o = glm::inverse(viewMatrix_o2w);
    glm::vec3 EyePosition_object = glm::vec3(ViewMatrix_w2o * glm::vec4(eyePosition_world, 1.0f)); //point
    glm::vec3 rayv_object = glm::vec3(ViewMatrix_w2o * glm::vec4(rayv_world, 0.0f)); // vector

    float radius = 0.5f;  // r
    float height = 1.0f;  // height


    //side equation: x^2 + z^2 = r^2
    //Ray equation: P(t) = eye + t * rayv
    //top: y = h/2
    //bot: y = -h/2
    //for top and bot we need to check if in valid circle: x^2 + z^2 <= r^2
    //A: 
    //B:
    //C:
    float a = rayv_object.x * rayv_object.x + rayv_object.z * rayv_object.z;
    float b = 2.0f * (EyePosition_object.x * rayv_object.x + EyePosition_object.z * rayv_object.z);
    float c = EyePosition_object.x * EyePosition_object.x + EyePosition_object.z * EyePosition_object.z - radius * radius;

    float discriminant = b * b - 4 * a * c;
    float t_side = -1.0f;

    // side
    if (discriminant >= 0.0f) { //two point
        float sqrt_discriminant = sqrt(discriminant);
        float t1 = (-b - sqrt_discriminant) / (2.0f * a);
        float t2 = (-b + sqrt_discriminant) / (2.0f * a);

        // choose positive and smaller
        if (t1 > 0.0f && t2 > 0.0f)
            t_side = std::min(t1, t2);
        else if (t1 > 0.0f)
            t_side = t1;
        else if (t2 > 0.0f)
            t_side = t2;

        // check if side point in the height range
        if (t_side > 0.0f) {
            float y = EyePosition_object.y + t_side * rayv_object.y;
            if (y < -height / 2.0f || y > height / 2.0f)
                t_side = -1.0f; // over range no point
        }
    }

    // top
    float t_top = -1.0f;
    if (rayv_object.y != 0.0f) {
        t_top = (height / 2.0f - EyePosition_object.y) / rayv_object.y;
        if (t_top > 0.0f) {
            float x = EyePosition_object.x + t_top * rayv_object.x;
            float z = EyePosition_object.z + t_top * rayv_object.z;
            if (x * x + z * z > radius * radius)
                t_top = -1.0f; // over circle
        } else {
            t_top = -1.0f;
        }
    }

    // bot
    float t_bottom = -1.0f;
    if (rayv_object.y != 0.0f) {
        t_bottom = (-height / 2.0f - EyePosition_object.y) / rayv_object.y;
        if (t_bottom > 0.0f) {
            float x = EyePosition_object.x + t_bottom * rayv_object.x;
            float z = EyePosition_object.z + t_bottom * rayv_object.z;
            if (x * x + z * z > radius * radius)
                t_bottom = -1.0f; // over circle
        } else {
            t_bottom = -1.0f;
        }
    }

    // choose closest
    float t = -1.0f;
    if (t_side > 0.0f)
        t = t_side;
    if (t_top > 0.0f && (t < 0.0f || t_top < t))
        t = t_top;
    if (t_bottom > 0.0f && (t < 0.0f || t_bottom < t))
        t = t_bottom;

    // only positive means having result
    if (t > 0.0f)
        return (float) t;
    else
        return -1.0f;
}

// compute the normal at the intersection point of object space!!
glm::vec3 Cylinder::computeNormal(glm::vec3 isectPoint){
    //top vector should always be (0, 1, 0)
    //bot vector should always be (0, -1, 0)
    //side vector should be magnitude of x and z
    //(x, 0, z)
    // std::cout << "isectPoint: " << isectPoint.x << ", " << isectPoint.y << ", " << isectPoint.z << std::endl;
    float radius = 0.5f;
    float height = 1.0f;
    const float epsilon = 1e-3f;

    //check if intersec is in top
    if(abs(isectPoint.y - height/2.0f) < epsilon){
        return glm::vec3(0.0f, 1.0f, 0.0f);
    } 
    //check if intersec is in bot
    else if (abs(isectPoint.y + height/2.0f) < epsilon){
        return glm::vec3(0.0f, -1.0f, 0.0f );
    } else { //side
        glm::vec3 normal = glm::vec3(float(isectPoint.x), 0.0f, float(isectPoint.z));
        normal = glm::normalize(normal);
        // std::cout << "normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl; 
        return normal;
    }
    // return glm::vec3(0, 0, 0);
}