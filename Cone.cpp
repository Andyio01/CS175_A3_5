#include "Cone.h"
#include "Mesh.h"
#include <cmath>
#include <iostream>


static std::vector<Vertex> vertices;
static std::vector<Face>   Faces;

static float radius = 0.5f;

void Cone::drawTriangleMeshFromFaces(){

    // Get current mode
    GLint shadeModel;
    glGetIntegerv(GL_SHADE_MODEL, &shadeModel);

    // Draw side face
    glBegin(GL_TRIANGLES);

    for (Mesh* g : this->graphs){
        for (Face* face : *g->getFaceIterator()) {
                Vertex* const* verts = face->getVertices();
                for (int i = 0; i < 3; i++)
                {
                    Vertex* v = verts[i];
                    if (shadeModel == GL_SMOOTH)
                    {
                        normalizeNormal(v->getNormals());
                    }
                    else if (shadeModel == GL_FLAT)
                    {
                        normalizeNormal(face->getFaceNormal());
                    }
                    
                    glm::vec3 pos = v->getPos();
                    glVertex3f(pos.x, pos.y, pos.z);
                }
                // glNormal3f(normal.x, normal.y, normal.z);
            }
        }
    glEnd();
    
}
    

    

void Cone::drawNormalsFromFaces(){
    // glColor3f(1.0f, .0f, .0f);
    // glBegin(GL_LINES);
    // for (Face &face : Faces) {
    //     Vertex *const *verts      = face.getVertices();
    //     glm::vec3      v0Pos      = verts[0]->getPos();
    //     glm::vec3      v1Pos      = verts[1]->getPos();
    //     glm::vec3      v2Pos      = verts[2]->getPos();
    //     glm::vec3      v1v0       = glm::normalize(v1Pos - v0Pos);
    //     glm::vec3      v2v0       = glm::normalize(v2Pos - v0Pos);
    //     glm::vec3      faceNormal = glm::normalize(glm::cross(v1v0, v2v0));

    //     for (int i = 0; i < 3; ++i) {
    //         const glm::vec3 &pos = (verts[i]->getPos());
    //         glVertex3f(pos.x, pos.y, pos.z);
    //         glVertex3f(pos.x + faceNormal.x * .1f, pos.y + faceNormal.y * .1f,
    //                    pos.z + faceNormal.z * .1f);
    //     }
    // }
    // glEnd();
}


void Cone::draw(){
    drawTriangleMeshFromFaces();
}

void Cone::drawNormal() {
    // drawNormalsFromFaces();
    // std::cout << "The drawnormal function is called !" << std::endl;
    glColor3f(1.0f, .0f, .0f);

    glBegin(GL_LINES);
    for (Mesh* g : this->graphs){
        for (Vertex *v : *g->getVertexIterator()){
            const glm::vec3 &normal = v->getNormals();
            const glm::vec3 &pos = (v->getPos());
            
            glVertex3f(pos.x, pos.y, pos.z);
            glVertex3f(pos.x + normal.x * .1f, pos.y + normal.y * .1f,
                        pos.z + normal.z * .1f);
        }
    }
    glEnd();
}

void Cone::calculate() {
    int vcount = (m_segmentsX + 1) * (m_segmentsY + 1);
    int fcount = m_segmentsX  * (m_segmentsY + 1) * 2;
    // Create a new mesh to store the cone
    Mesh* side = new Mesh(vcount, fcount);
    Mesh* bottom = new Mesh(m_segmentsX + 1, m_segmentsX + 1);
    // this->clearGraphs();

    float stepAngle = 2.0f * glm::pi<float>() / m_segmentsX;  // 360 degrees divided by segmentsX
    float stepY = 1.0f / m_segmentsY;  // Height divided by segmentsY

    float height = 1.0f;  // Total height of the cone
    float radius = 0.5f;  // Base radius of the cone

    // Generate vertices for the side mesh, based on segmentY and segmentX
    for (int i = 0; i <= m_segmentsY; i++) {
        float rate = i * stepY;
        float y = -height / 2 + i * stepY * height;  // y-coordinate for each level
        float r = radius * (height / 2 - y) / height;  // Calculate radius for current height level

        for (int j = 0; j < m_segmentsX; j++) {
            float angle = j * stepAngle;
            float x = r * glm::cos(angle);  // x-coordinate
            float z = r * glm::sin(angle);  // z-coordinate

            glm::vec3 position(x, y, z);
            Vertex* v = new Vertex(position);

            // Check if the vertex is on the bottom (y == -height / 2)
            if (y == -height / 2) {
                // This vertex is on the bottom, assign the downward normal
                glm::vec3 bottomNormal(0.0f, -1.0f, 0.0f);
                Vertex* tempV = new Vertex(position);
                tempV->setNormal(bottomNormal);  // Assign the normal to the vertex
                bottom->addVertex(tempV);
            }
            if (y != height / 2){
                float n_y = float(radius / sqrt(radius*radius + height*height));
                // Calculate normal for this vertex
                float Nx = x;
                float Nz = z;
                float length = sqrt(Nx * Nx + Nz * Nz);  // Only normalize x and z
                Nx /= length;
                Nz /= length;
                glm::vec3 normal(Nx, n_y , Nz);
                // normal = glm::normalize(normal);  // Normalize the normal vector
                v->setNormal(normal);  // Store the normal directly in the vertex
            }

            
            side->addVertex(v);
        }
    }

    // Add the top vertex (apex of the cone)
    glm::vec3 topVertexPos(0.0f, height / 2, 0.0f);
    Vertex* topVertex = new Vertex(topVertexPos);
    topVertex->setNormal(glm::vec3(0.0, 1.0f, 0.0f));
    side->addVertex(topVertex);
    
    Vertex** verts = side->getVertices();
    // Generate side faces using index-based method
    for (int i = 0; i < m_segmentsY; ++i) {
        for (int j = 0; j < m_segmentsX; ++j) {
            // Calculate indices for the current and next vertices
            int index1 = i * m_segmentsX + j;
            int index2 = index1 + 1;
            if (j == m_segmentsX - 1) index2 = i * m_segmentsX;  // Wrap around to first vertex

            int index3 = (i + 1) * m_segmentsX + j;
            int index4 = index3 + 1;
            if (j == m_segmentsX - 1) index4 = (i + 1) * m_segmentsX;  // Wrap around

            // Create two faces for each segment
            Face* f1 = new Face(verts[index1], verts[index3], verts[index4]);
            Face* f2 = new Face(verts[index4], verts[index2], verts[index1]);

            side->addFace(f1);
            side->addFace(f2);
        }
    }

    // Create the top face that connects the last ring to the top vertex
    for (int j = 0; j < m_segmentsX; ++j) {
        int index1 = m_segmentsY * m_segmentsX + j;  // Last ring of vertices
        int index2 = m_segmentsY * m_segmentsX + (j + 1) % m_segmentsX;  // Wrap around

        // Create a face that connects the top vertex to the last ring
        Face* f = new Face(verts[index1], topVertex, verts[index2]);
        side->addFace(f);
    }

    // Add bottom mesh
    glm::vec3 bottomVertexPos(0.0f, -height / 2, 0.0f);
    Vertex* bottomVertex = new Vertex(bottomVertexPos);
    bottomVertex->setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
    bottom->addVertex(bottomVertex);

    // Create the bottom face by connecting each segment to the center
    for (int j = 0; j < m_segmentsX; ++j) {
        int index1 = j;
        int index2 = (j + 1) % m_segmentsX;  // Wrap around

        Face* bottomFace = new Face(bottomVertex, verts[index2], verts[index1]);
        bottom->addFace(bottomFace);
    }

    // Add bottom and side meshes to the graphs
    this->graphs.push_back(side);
    this->graphs.push_back(bottom);

    // Calculate normals for shading
    // for (Mesh* g : this->graphs) {
    //     g->calculateVertexNormal();
    // }

    // Optional: Print total number of vertices and faces
    // int verticesSize = 0, facesSize = 0;
    // for (Mesh* g : this->graphs) {
    //     verticesSize += g->getVertices().size();
    //     facesSize += g->getFaces().size();
    // }
    // std::cout << "Vertices count: " << verticesSize << std::endl;
    // std::cout << "Faces count: " << facesSize << std::endl;
}

double Cone::intersect(glm::vec3 eyePosition_world, glm::vec3 rayv_world, glm::mat4 viewMatrix_o2w) {
    // Implement intersection logic
    //viewMatrix is matrix object to world
    //float world_ray_length = glm::length(rayv_world);

    glm::mat4 ViewMatrix_w2o = glm::inverse(viewMatrix_o2w);
    glm::vec3 EyePosition_object = glm::vec3(ViewMatrix_w2o * glm::vec4(eyePosition_world, 1.0f)); //it's a point
    glm::vec3 rayv_object = glm::vec3(ViewMatrix_w2o * glm::vec4(rayv_world, 0.0f)); //it's a vector

    //float object_ray_length = glm::length(rayv_object);
    //float scale_factor = world_ray_length / object_ray_length;

    //for each pixel no matter what it needs to return the smallest t
    // cone side equation: x^2 + z^2 = （apex - y)^2 * (r^2 / height^2)
    // apex = 0.5, height = 1
    // apex - y is height of current stack to the apex 
    // Ray equation: P(t) = eye + t * rayv
    double apex = 0.5f;
    double height = 1.0f;
    double r = radius;
    //double resT;
    //cone should have two parts:
    //side part
    // This is a quadratic equation in t
    double a = rayv_object.x * rayv_object.x + rayv_object.z * rayv_object.z - 
               (r * r / (height * height)) * rayv_object.y * rayv_object.y;
    //looks good after fix - to +
    double b = 2.0f * (EyePosition_object.x * rayv_object.x + EyePosition_object.z * rayv_object.z) + 
               2.0f * (r * r / (height * height)) * (apex - EyePosition_object.y) * rayv_object.y;
    double c = EyePosition_object.x * EyePosition_object.x + EyePosition_object.z * EyePosition_object.z - 
               (r * r / (height * height)) * (apex - EyePosition_object.y) * (apex - EyePosition_object.y);

    double discriminant = b * b - 4 * a * c;
    double t_side = -1.0f;
    if (discriminant >= 0.0f) {
        double sqrt_discriminant = sqrt(discriminant);
        double t1 = (-b - sqrt_discriminant) / (2.0f * a);
        double t2 = (-b + sqrt_discriminant) / (2.0f * a);

        // pick smallest t
        if (t1 > 0.0f && t2 > 0.0f){
            t_side = std::min(t1, t2);
        }
        else if (t1 > 0.0f){
            t_side = t1;
        }
        else if (t2 > 0.0f) {
            t_side = t2;
        }

        // option: check range
        if (t_side > 0.0f) {
            double y = EyePosition_object.y + (float)t_side * rayv_object.y;
            // std::cout << "in cone EyePosition_object.y " << EyePosition_object.y << std::endl;
            // std::cout << "in cone rayv_object.y " << rayv_object.y << std::endl;
            // std::cout << "in cone T_sides: " << t_side << std::endl;
            // std::cout << "in cone inte.y " << y << std::endl;
            // std::cout << "in cone discriminant " << sqrt_discriminant << std::endl;
            // std::cout << "in cone a " << a << std::endl;
            // std::cout << "in cone b " << b << std::endl;
            // std::cout << "in cone c " << c << std::endl;
            if (y < -height / 2.0f || y > height / 2.0f){
                t_side = -1.0f; // over range no point
            }
            // } else {
            //     std::cout << "in cone T_sides: " << t_side << std::endl;
            //     std::cout << "in cone inte.y " << y << std::endl;
            // }
            
        }
    }

    //bottom part
    //bottom equpation: y = -0.5
    const double epsilon = 1e-6f;
    double t_bottom = -1.0f; //null
    if(abs(rayv_object.y) > epsilon){
        //std::cout << "rayv_object.y" << rayv_object.y << std::endl;
        t_bottom = (-height / 2.0f - EyePosition_object.y) / rayv_object.y;
        if (t_bottom > 0.0f) {
            //std::cout << "t_bottom > 0.0f" << t_bottom << std::endl;
            glm::vec3 intersectionBottom = EyePosition_object + (float) t_bottom * rayv_object;
            if (intersectionBottom.x * intersectionBottom.x + intersectionBottom.z * intersectionBottom.z > r * r){
                t_bottom = -1.0f;
            }
        } else {
            t_bottom = -1.0f;
        }
    }
    
    double ResT = -1.0f;
    if (t_side > 0.0f && t_bottom > 0.0f) {
        ResT = std::min(t_side, t_bottom);
        // std::cout << "final T_sides: " << t_side << std::endl;
        // std::cout << "final t_bottom: " << t_bottom << std::endl;
        // std::cout << "compared t returned:" << std::endl;
    } else if (t_side > 0.0f) {
        // std::cout << "final T_sides: " << t_side << std::endl;
        // std::cout << "final t_bottom: " << t_bottom << std::endl;
        // std::cout << "compared t returned:" << std::endl;
        ResT = t_side;
    } else if (t_bottom > 0.0f) {
        // std::cout << "final T_sides: " << t_side << std::endl;
        // std::cout << "final t_bottom: " << t_bottom << std::endl;
        // std::cout << "bot t returned:" << std::endl;
        ResT = t_bottom;
    }

    //return ResT * scale_factor;
    return ResT;
}

// compute the normal at the intersection point of object space!!
// TODO: NEED TO FIX
glm::vec3 Cone::computeNormal(glm::vec3 isectPoint){
    double apex = 0.5f;
    double height = 1.0f;
    double r = radius;
    double k = r / height;
    //same thing we need two parts;
    //for bottom: it should always be (0, -1, 0)
    //for side: simply get deriv of equation
    // x^2 + z^2 = （apex - y)^2 * (r^2 / height^2)
    // dF/dx = 2x
    // dF/dy = -2k^2*(a-y)
    // dF/dz = 2z
    const double epsilon = 1e-4f; // for comparation

    if (abs(isectPoint.y + 0.5f) < epsilon) { // cuz double, isectPoint.y should be around -0.5
        //std::cout << "cone bot: " << isectPoint.x << ", " << isectPoint.y << ", " << isectPoint.z << std::endl;
        return glm::vec3(0.0f, -1.0f, 0.0f);
    } else {
        double x = isectPoint.x;
        double y = isectPoint.y;
        double z = isectPoint.z;
        
        //(2x, -2k^2*(a-y), 2z)
        double nx = 2.0f * x;
        double ny = 2.0f * k * k * (apex - y);
        double nz = 2.0f * z;

        // glm::vec3 normal1;
        // normal1.x = isectPoint.x;
        // normal1.z = isectPoint.z;
        // normal1.y = radius / height; 

    // return glm::normalize(normal); 
        
        //glm::vec3 normal(nx, ny, nz);
        glm::vec3 normal = glm::normalize(glm::vec3(nx, ny, nz));
        //std::cout << "cone side: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
        return normal;
    }

    // float apexY = 0.5f;
    // float bottomY = -0.5f; 
    // float height = apexY - bottomY;
    // float radius = 0.5f; 


    // if (isectPoint.y <= bottomY + 1e-6) {
    //     return glm::vec3(0.0f, -1.0f, 0.0f); 
    // }  else {


    // glm::vec3 normal;
    // normal.x = isectPoint.x;
    // normal.z = isectPoint.z;
    // normal.y = radius / height; 

    // return glm::normalize(normal); 
    // }
    //return glm::normalize(isectPoint);
}