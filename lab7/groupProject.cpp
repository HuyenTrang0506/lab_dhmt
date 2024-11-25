#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>

#define M_PI 3.14159265358979323846

float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationY1 = 0.0f;
float rotationY2 = 0.0f;
float rotationY3 = 0.0f;
float scale = 1.0f; 

bool wireframeMode = true; 
bool orthographicMode = false; 

struct Vector3D {
    float x, y, z;
    Vector3D(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

    void normalize() {
        float length = sqrt(x * x + y * y + z * z);
        if (length > 0) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    Vector3D operator+(const Vector3D& v) const {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    Vector3D operator-(const Vector3D& v) const {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    Vector3D operator*(float f) const {
        return Vector3D(x * f, y * f, z * f);
    }

    float dot(const Vector3D& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3D cross(const Vector3D& v) const {
        return Vector3D(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    Vector3D& operator+=(const Vector3D& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
};

// Structure to store vertex information
struct Vertex {
    Vector3D position;
    Vector3D normal;
    Vector3D color;
    Vertex(const Vector3D& pos = Vector3D(),
        const Vector3D& norm = Vector3D(),
        const Vector3D& col = Vector3D(1, 1, 1))
        : position(pos), normal(norm), color(col) {}
};

struct Material {
    Vector3D ambient;
    Vector3D diffuse;
    Vector3D specular;
    float shininess;

    Material(Vector3D amb = Vector3D(0.2f, 0.2f, 0.2f),
        Vector3D diff = Vector3D(0.8f, 0.8f, 0.8f),
        Vector3D spec = Vector3D(1.0f, 1.0f, 1.0f),
        float shin = 32.0f)
        : ambient(amb), diffuse(diff), specular(spec), shininess(shin) {}
};
void multiplyMatrices(const float mat1[16], const float mat2[16], float result[16]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i * 4 + j] += mat1[i * 4 + k] * mat2[k * 4 + j];
            }
        }
    }
}
void setPerspective(GLfloat fovY, GLfloat aspect, GLfloat nearZ, GLfloat farZ) {
    GLfloat f = 1.0f / tan(fovY * M_PI / 360.0f);
    GLfloat matrix[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (farZ + nearZ) / (nearZ - farZ), -1,
        0, 0, (2 * farZ * nearZ) / (nearZ - farZ), 0
    };
    glMultMatrixf(matrix);
}


void setOrtho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearZ, GLfloat farZ) {
    GLfloat matrix[16] = {
        2 / (right - left), 0, 0, -(right + left) / (right - left),
        0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
        0, 0, -2 / (farZ - nearZ), -(farZ + nearZ) / (farZ - nearZ),
        0, 0, 0, 1
    };
    glMultMatrixf(matrix); 
}
void scaleMatrix(float scaleX, float scaleY, float scaleZ) {
     GLfloat matrix[16] = {
        scaleX, 0, 0, 0,
	    0, scaleY, 0, 0,
	    0, 0, scaleZ, 0,
	    0, 0, 0, 1
    };
     glMultMatrixf(matrix); 
}

Vector3D lightPos(5.0f, 5.0f, 5.0f);
struct Shape {
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    Material material;


    void initializePolyhedron(float radius) {
        vertices.clear();
        indices.clear();

        std::vector<Vector3D> positions = {
             Vector3D(0, 0, radius),
             Vector3D(0, 0, -radius),
             Vector3D(-radius, 0, 0),
             Vector3D(radius, 0, 0),
             Vector3D(0, -radius, 0),
             Vector3D(0, radius, 0)
        };


        for (const auto& pos : positions) {
            Vector3D normal = pos;
            normal.normalize();
            vertices.push_back(Vertex(pos, normal));
        }

        std::vector<std::vector<int>> faces = {
            {0, 3, 5}, {0, 5, 2}, {0, 2, 4}, {0, 4, 3},
            {1, 5, 3}, {1, 2, 5}, {1, 4, 2}, {1, 3, 4}
        };

      
        for (const auto& face : faces) {
            for (int idx : face) {
                indices.push_back(idx);
            }
        }
    }

    // Initialize Sphere
    void initializeSphere(float radius, int stacks, int slices) {
        vertices.clear();
        indices.clear();

        for (int i = 0; i <= stacks; ++i) {
            float phi = M_PI * float(i) / float(stacks);//vi do
            for (int j = 0; j <= slices; ++j) {
                float theta = 2.0f * M_PI * float(j) / float(slices);//kinh do

                float x = radius * sin(phi) * cos(theta);
                float y = radius * cos(phi);
                float z = radius * sin(phi) * sin(theta);

                Vector3D position(x, y, z);
                Vector3D normal = position;
                normal.normalize();

                vertices.push_back(Vertex(position, normal));
            }
        }

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                int first = i * (slices + 1) + j;
                int second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(first + 1);
                indices.push_back(second);

                // Changed winding order for the second triangle
                indices.push_back(second);
                indices.push_back(first + 1);
                indices.push_back(second + 1);
            }
        }
    }

    void initializeCylinder(float radius, float y_begin, float y_end) {
        vertices.clear();
        indices.clear();

        float height = y_end - y_begin;
        int segments = static_cast<int>(20 * sqrt(radius));
        
        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * M_PI * float(i) / float(segments);
            float x = radius * cos(theta);
            float z = radius * sin(theta);

            Vector3D position(x, y_begin, z);
            Vector3D normal(x, 0.0f, z);
            normal.normalize();

            vertices.push_back(Vertex(position, normal));

            position.y = y_end;
            vertices.push_back(Vertex(position, normal));
        }

        // Generate indices for the side triangles
        for (int i = 0; i < segments; ++i) {
            int base = i * 2;
            indices.push_back(base);
            indices.push_back(base + 1);
            indices.push_back(base + 2);

            indices.push_back(base + 1);
            indices.push_back(base + 3);
            indices.push_back(base + 2);
        }

        // Add center vertices for top and bottom caps
        int centerBottomIndex = vertices.size();
        vertices.push_back(Vertex(Vector3D(0.0f, y_begin, 0.0f), Vector3D(0.0f, -1.0f, 0.0f)));

        int centerTopIndex = vertices.size();
        vertices.push_back(Vertex(Vector3D(0.0f, y_end, 0.0f), Vector3D(0.0f, 1.0f, 0.0f)));

        // Add indices for bottom cap
        for (int i = 0; i < segments; ++i) {
            indices.push_back(centerBottomIndex);
            indices.push_back(i * 2);
            indices.push_back((i * 2 + 2) % (segments * 2));
        }

        // Add indices for top cap
        for (int i = 0; i < segments; ++i) {
            indices.push_back(centerTopIndex);
            indices.push_back(i * 2 + 1);
            indices.push_back((i * 2 + 3) % (segments * 2));
        }
    }

    void applyCombinedTransformation(float tx, float ty, float tz, float rotationAngleX, float rotationAngleY) {

        float translationMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            tx, ty, tz, 1.0f
        };

        float rotationMatrixX[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, static_cast<float>(cos(rotationAngleX * M_PI / 180.0)), -static_cast<float>(sin(rotationAngleX * M_PI / 180.0)), 0.0f,
            0.0f, static_cast<float>(sin(rotationAngleX * M_PI / 180.0)), static_cast<float>(cos(rotationAngleX * M_PI / 180.0)), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float rotationMatrixY[16] = {
            static_cast<float>(cos(rotationAngleY * M_PI / 180.0)), 0.0f, static_cast<float>(sin(rotationAngleY * M_PI / 180.0)), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -static_cast<float>(sin(rotationAngleY * M_PI / 180.0)), 0.0f, static_cast<float>(cos(rotationAngleY * M_PI / 180.0)), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };


        float tempMatrix[16];
        float combinedMatrix[16];
        multiplyMatrices(rotationMatrixY, rotationMatrixX, tempMatrix);
        multiplyMatrices(translationMatrix, tempMatrix, combinedMatrix);

        // Apply the combined transformation
        for (auto& vertex : vertices) {
            Vector3D& pos = vertex.position;
            float newX = pos.x * combinedMatrix[0] + pos.y * combinedMatrix[4] + pos.z * combinedMatrix[8] + combinedMatrix[12];
            float newY = pos.x * combinedMatrix[1] + pos.y * combinedMatrix[5] + pos.z * combinedMatrix[9] + combinedMatrix[13];
            float newZ = pos.x * combinedMatrix[2] + pos.y * combinedMatrix[6] + pos.z * combinedMatrix[10] + combinedMatrix[14];
            pos.x = newX;
            pos.y = newY;
            pos.z = newZ;
        }
    }

    Vector3D calculateLambert(const Vector3D& position, const Vector3D& normal) {
        Vector3D lightDir = lightPos - position;
        lightDir.normalize();

        float diff = std::max(normal.dot(lightDir), 0.0f);

        Vector3D ambient = material.ambient;
        Vector3D diffuse = material.diffuse * diff;

        return ambient + diffuse;
    }

    // Render the shape using Lambert shading
    void renderLambert() {
        glShadeModel(GL_FLAT);
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < indices.size(); i += 3) {
            Vertex& v0 = vertices[indices[i]];
            Vertex& v1 = vertices[indices[i + 1]];
            Vertex& v2 = vertices[indices[i + 2]];

            Vector3D edge1 = v1.position - v0.position;
            Vector3D edge2 = v2.position - v0.position;
            Vector3D normal = edge1.cross(edge2);
            normal.normalize();

            Vector3D color = calculateLambert(v0.position, normal);

            glColor3f(color.x, color.y, color.z);
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(v0.position.x, v0.position.y, v0.position.z);
            glVertex3f(v1.position.x, v1.position.y, v1.position.z);
            glVertex3f(v2.position.x, v2.position.y, v2.position.z);
        }
        glEnd();
    }

  
};

void renderScene() {

    Shape polyhedron1, polyhedron2, polyhedron3, polyhedron4;
    polyhedron1.initializePolyhedron(1.25f);
    polyhedron1.material = Material(
        Vector3D(0xFE / 255.0f, 0xDC / 255.0f, 0x7B / 255.0f),  // Honeysuckle 
        Vector3D(0xFF / 255.0f, 0xFB / 255.0f, 0xF0 / 255.0f) // Sweet Daisy

    );
    polyhedron2.initializePolyhedron(0.35f);
    polyhedron2.material = Material(
        Vector3D(0xd2 / 255.0f, 0xb3 / 255.0f, 0xdb / 255.0f), // Lavender (ambient)
        Vector3D(0xe9 / 255.0f, 0xf3 / 255.0f, 0xff / 255.0f)  // Bridal Blue (diffuse)

    );
    polyhedron3.initializePolyhedron(0.35f);
    polyhedron3.material = Material(

        Vector3D(0x42 / 255.0f, 0x6b / 255.0f, 0xc2 / 255.0f),  // Sapphire blue 
        Vector3D(0xd2 / 255.0f, 0xb3 / 255.0f, 0xdb / 255.0f) // Lavender

    );
    polyhedron4.initializePolyhedron(0.35f);
    polyhedron4.material = Material(

        Vector3D(0xDF / 255.0f, 0x61 / 255.0f, 0x49 / 255.0f),  // Sapphire blue 
        Vector3D(0xd2 / 255.0f, 0xb3 / 255.0f, 0xdb / 255.0f) // Lavender

    );
    Shape sphere1, sphere2, sphere3;
    sphere1.initializeSphere(0.4f, 20, 15);
    sphere1.material = Material(
        Vector3D(0xDF / 255.0f, 0x61 / 255.0f, 0x49 / 255.0f), // Coral Blush 
        Vector3D(0xF4 / 255.0f, 0x90 / 255.0f, 0x69 / 255.0f)  // Fuzzy Peach 
    );

    sphere2.initializeSphere(0.6f, 15, 20);
    sphere2.material = Material(
        Vector3D(0x70 / 255.0f, 0x82 / 255.0f, 0x40 / 255.0f), // Moss Green 
        Vector3D(0xAB / 255.0f, 0xBA / 255.0f, 0x72 / 255.0f)  // Sage 
    );

    sphere3.initializeSphere(0.5f, 30, 20);
    sphere3.material = Material(
        Vector3D(0xAB / 255.0f, 0xBA / 255.0f, 0x72 / 255.0f), // Sage 
        Vector3D(0xDF / 255.0f, 0x61 / 255.0f, 0x49 / 255.0f)  // Coral Blush 
    );

    Shape cylinder1, cylinder2, cylinder3, cylinder4, cylinder5;
    cylinder1.initializeCylinder(4.5f, 3.75f, 4.0f);
    cylinder1.material = Material(
        Vector3D(0x29 / 255.0f, 0x24 / 255.0f, 0x21 / 255.0f), // Black 
        Vector3D(0xA7 / 255.0f, 0x5F / 255.0f, 0x37 / 255.0f)  // Copper 
    );

    cylinder2.initializeCylinder(4.5f, -4.0f, -3.75f);
    cylinder2.material = Material(
        Vector3D(0x29 / 255.0f, 0x24 / 255.0f, 0x21 / 255.0f), // Black 
        Vector3D(0xA7 / 255.0f, 0x5F / 255.0f, 0x37 / 255.0f)  // Copper 
    );

    cylinder3.initializeCylinder(4.2f, 3.5f, 3.75f);
    cylinder3.material = Material(
        Vector3D(0xA7 / 255.0f, 0x5F / 255.0f, 0x37 / 255.0f),  // Copper 
        Vector3D(0x29 / 255.0f, 0x24 / 255.0f, 0x21 / 255.0f) // Black 
    );

    cylinder4.initializeCylinder(4.2f, -3.75f, -3.5f);
    cylinder4.material = Material(
        Vector3D(0xA7 / 255.0f, 0x5F / 255.0f, 0x37 / 255.0f),  // Copper 
        Vector3D(0x29 / 255.0f, 0x24 / 255.0f, 0x21 / 255.0f) // Black 
    );

    cylinder5.initializeCylinder(0.15f, -3.5f, 3.5f);
    cylinder5.material = Material(
        Vector3D(0x29 / 255.0f, 0x24 / 255.0f, 0x21 / 255.0f), // Black 
        Vector3D(0xA7 / 255.0f, 0x5F / 255.0f, 0x37 / 255.0f)  // Copper 
    );

    polyhedron1.applyCombinedTransformation(0.0f, 0.0f, 0.0f, rotationX, rotationY);
    polyhedron2.applyCombinedTransformation(4.0f, 0.0f, -0.25f, rotationX, rotationY2);
    polyhedron3.applyCombinedTransformation(-4.0f, -2.25f, 0.0f, rotationX, rotationY1);
    polyhedron4.applyCombinedTransformation(0.0f, 2.0f, 4.0f, rotationX, rotationY);
    sphere1.applyCombinedTransformation(2.0f, 0.0f, -1.5f, rotationX, rotationY1);
    sphere2.applyCombinedTransformation(-1.5f, 1.5f, -3.0f, rotationX, rotationY2);
    sphere3.applyCombinedTransformation(-1.5f, -1.5f, 2.0f, rotationX, rotationY3);
    cylinder1.applyCombinedTransformation(0.0f, 0.0f, 0.0f, rotationX, rotationY);
    cylinder2.applyCombinedTransformation(0.0f, 0.0f, 0.0f, rotationX, rotationY);
    cylinder3.applyCombinedTransformation(0.0f, 0.0f, 0.0f, rotationX, -rotationY2);
    cylinder4.applyCombinedTransformation(0.0f, 0.0f, 0.0f, rotationX, -rotationY2);
    cylinder5.applyCombinedTransformation(0.0f, 0.0f, 0.0f, rotationX, rotationY);

    if (wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

        cylinder1.renderLambert();
        cylinder2.renderLambert();
        cylinder3.renderLambert();
        cylinder4.renderLambert();
        cylinder5.renderLambert();
        polyhedron1.renderLambert();
        polyhedron2.renderLambert();
        polyhedron3.renderLambert();
        polyhedron4.renderLambert();
        sphere1.renderLambert();
        sphere2.renderLambert();
        sphere3.renderLambert();
    
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, 10.0f,    // Camera position
        0.0f, 0.0f, 0.0f,      // Look-at point
        0.0f, 1.0f, 0.0f);     // Up vector

  
    scaleMatrix(scale,scale,scale);
    renderScene();
    glutSwapBuffers();
}

void idle() {

    rotationY += 0.1f; 
    rotationY1 += 0.25f;
    rotationY2 += 0.08f;
    rotationY3 += 0.15f;
    if (rotationY >= 360.0f) rotationY -= 360.0f; 
    glutPostRedisplay(); 
}

// Mouse motion handler
void motion(int x, int y) {
    static int lastX = x;
    static int lastY = y;

    int deltaX = x - lastX;
    int deltaY = y - lastY;

    rotationY += deltaX * 0.5f;
    rotationY1 += deltaX * 0.5f;
    rotationY2 += deltaX * 0.5f;
    rotationY3 += deltaX * 0.5f;
    rotationX += deltaY * 0.5f;

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

   
    if (orthographicMode) {
        setOrtho(-10, 10, -10, 10, -100, 100); // S? d?ng hàm setOrtho
    }
    else {
        setPerspective(80.0f, (float)w / h, 0.1f, 100.0f); // S? d?ng hàm setPerspective
    }
    

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
   
    if (key == 27) {  
        exit(0);
    }
    else if (key == 'w' || key == 'W') {
        wireframeMode = !wireframeMode;
        glutPostRedisplay();
    }
    else if (key == 'o' || key == 'O') {
        orthographicMode = !orthographicMode;
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
    }
    else if (key == '+' || key == '=') {
        scale += 0.1f; 
        glutPostRedisplay();
    }
    else if (key == '-' || key == '_') {
        scale -= 0.1f; 
        if (scale < 0.1f) scale = 0.1f; 
        glutPostRedisplay();
    }
  
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Computer Graphics Project");

    // OpenGL initialization
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;

}
