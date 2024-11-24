#include <GL/glut.h> 
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#define M_PI 3.14159265358979323846

// Cau truc Vector3D de luu toa do vector
struct Vector3D {
    float x, y, z;
    Vector3D(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
	//chuan hoa, bien vecto thanh vecto don vi thuan tien tinh toan
    void normalize() {
        float length = sqrt(x*x + y*y + z*z);
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

    float dot(const Vector3D& v) const {//tich vo huong
        return x * v.x + y * v.y + z * v.z;
    }
     Vector3D cross(const Vector3D& v) const {//tich co huong
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

// Cau truc Vertex de luu thong tin dinh
struct Vertex {
    Vector3D position;
    Vector3D normal;
    Vector3D color;
    Vertex(const Vector3D& pos = Vector3D(), 
           const Vector3D& norm = Vector3D(), 
           const Vector3D& col = Vector3D(1,1,1))
        : position(pos), normal(norm), color(col) {}
};

// Cau truc Material de luu thong tin vat lieu
struct Material {
    Vector3D ambient;
    Vector3D diffuse;
    Vector3D specular;
    float shininess;

    Material() {
        ambient = Vector3D(0.2f, 0.2f, 0.2f);
        diffuse = Vector3D(0.8f, 0.8f, 0.8f);
        specular = Vector3D(1.0f, 1.0f, 1.0f);
        shininess = 32.0f;
    }
};

// Bien toan cuc
std::vector<Vertex> vertices;
std::vector<int> indices;
Vector3D lightPos(5.0f, 5.0f, 5.0f);
Material material;
float rotationX = 0.0f;
float rotationY = 0.0f;
int currentShader = 0; // 0: Lambert, 1: Gouraud, 2: Phong
bool wireframe = false;

void initializePolyhedron(float radius) {
    vertices.clear();
    indices.clear();

    // Dinh nghia cac dinh
    std::vector<Vector3D> positions = {
        Vector3D(0, 0, radius),   // Dinh tren
        Vector3D(0, 0, -radius),  // Dinh duoi
        Vector3D(-radius, 0, 0),  // Trai
        Vector3D(radius, 0, 0),   // Phai
        Vector3D(0, -radius, 0),  // Truoc
        Vector3D(0, radius, 0)    // Sau
    };

    // Tao dinh voi thong tin vi tri va phap tuyen
    for (const auto& pos : positions) {
        Vector3D normal = pos;
        normal.normalize();
        vertices.push_back(Vertex(pos, normal));
    }

    // Dinh nghia cac mat tam giac
    std::vector<std::vector<int>> faces = {
        {0, 3, 5}, {0, 5, 2}, {0, 2, 4}, {0, 4, 3}, 
        {1, 5, 3}, {1, 2, 5}, {1, 4, 2}, {1, 3, 4}  
    };

    // Chuyen mat tam giac thanh danh sach chi so
    for (const auto& face : faces) {
        for (int idx : face) {
            indices.push_back(idx);
        }
    }
}

// Tinh toan anh sang Lambert
Vector3D calculateLambert(const Vector3D& position, const Vector3D& normal) {
    Vector3D lightDir = lightPos - position;
    lightDir.normalize();

    float diff = std::max(normal.dot(lightDir), 0.0f);
    
    Vector3D ambient = material.ambient;   
    Vector3D diffuse = material.diffuse * diff;
    
    return ambient + diffuse;
}

//Lambert shading
void renderLambert() {
    glShadeModel(GL_FLAT);  
    glBegin(GL_TRIANGLES); 
    for (size_t i = 0; i < indices.size(); i += 3) {  
        // Tính toán ánh sáng cho tung mat tam giác
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        // Tính vecto pháp tuyen
        Vector3D edge1 = v1.position - v0.position;
        Vector3D edge2 = v2.position - v0.position;
        Vector3D normal = edge1.cross(edge2);
        normal.normalize();  
        // Tính toán ánh sáng Lambert cho mat tam giác
        Vector3D color = calculateLambert(v0.position, normal);  
        
        // Áp dung màu cho tat cac các dinh cua tam giác
        glColor3f(color.x, color.y, color.z); 
        glNormal3f(normal.x, normal.y, normal.z);  
        glVertex3f(v0.position.x, v0.position.y, v0.position.z);  
        glVertex3f(v1.position.x, v1.position.y, v1.position.z);  
        glVertex3f(v2.position.x, v2.position.y, v2.position.z);  
    }
    glEnd();  // K?t thúc v?
}

void renderGouraud() {
    glShadeModel(GL_SMOOTH); 
    glBegin(GL_TRIANGLES);

    // Tính pháp tuy?n cho t?ng m?t và t?ng d?nh
    std::vector<Vector3D> vertexNormals(vertices.size(), Vector3D(0, 0, 0)); // Luu pháp tuy?n c?a t?ng d?nh

    // Tính pháp tuy?n cho t?ng m?t
    for (size_t i = 0; i < indices.size(); i += 3) {
        // L?y 3 d?nh c?a m?t tam giác
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        // Tính pháp tuy?n m?t tam giác
        Vector3D edge1 = v1.position - v0.position;
        Vector3D edge2 = v2.position - v0.position;
        Vector3D faceNormal = edge1.cross(edge2);
        faceNormal.normalize();
		//tinh vecto tai moi dinh
        // C?ng d?n pháp tuy?n m?t vào pháp tuy?n c?a các d?nh
        vertexNormals[indices[i]] += faceNormal;
        vertexNormals[indices[i + 1]] += faceNormal;
        vertexNormals[indices[i + 2]] += faceNormal;
    }

    // Chu?n hóa pháp tuy?n cho t?ng d?nh
    for (auto& normal : vertexNormals) {
        normal.normalize();
    }

    // V? các tam giác v?i pháp tuy?n dã tính
    for (size_t i = 0; i < indices.size(); i += 3) {
        for (int j = 0; j < 3; ++j) {
            int vertexIndex = indices[i + j];
            Vertex& v = vertices[vertexIndex];
            Vector3D& normal = vertexNormals[vertexIndex]; // L?y pháp tuy?n trung bình

            Vector3D color = calculateLambert(v.position, normal); // Tính màu v?i pháp tuy?n
            glColor3f(color.x, color.y, color.z);  
            glNormal3f(normal.x, normal.y, normal.z); // Áp d?ng pháp tuy?n
            glVertex3f(v.position.x, v.position.y, v.position.z); 
        }
    }

    glEnd();
}



// Hien thi text
void renderText(float x, float y, const char* text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);
    
    for(const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Ham display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // Ap dung phuong phap xoay
    glRotatef(rotationX, 1, 0, 0);
    glRotatef(rotationY, 0, 1, 0);

    if(wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Ap dung shader tuong ung
    switch(currentShader) {
        case 0: renderLambert(); break;
        case 1: renderGouraud(); break;
    }

    // Hien thi thong tin
    std::string shaderName;
    switch(currentShader) {
        case 0: shaderName = "Lambert Shading"; break;
        case 1: shaderName = "Gouraud Shading"; break;
        case 2: shaderName = "Phong Shading"; break;
    }
    std::string info = "Shader: " + shaderName + 
                      " | Mode: " + (wireframe ? "Wireframe" : "Solid") +
                      " | Use 1-2-3 to switch shaders, W for wireframe";
    renderText(10, 20, info.c_str());

    glutSwapBuffers();
}

// Xu ly phim bam
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case '1': currentShader = 0; break;
        case '2': currentShader = 1; break;
        
        case 'w': case 'W': wireframe = !wireframe; break;
        case 27: exit(0); break;  // ESC de thoat
    }
    glutPostRedisplay();
}

// Xu ly chuot
void motion(int x, int y) {
    static int lastX = x;
    static int lastY = y;

    int deltaX = x - lastX;
    int deltaY = y - lastY;

    rotationY += deltaX * 0.5f;
    rotationX += deltaY * 0.5f;

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}

// Ham reshape
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w/h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shading Algorithms Demo");

    // Khoi tao OpenGL
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  	initializePolyhedron(1.0f); // Ban kinh khoi da giac la 1

    // Dang ky cac callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);

    std::cout << "Huong dan su dung:" << std::endl;
    std::cout << "1: Lambert Shading" << std::endl;
    std::cout << "2: Gouraud Shading" << std::endl;
    std::cout << "3: Phong Shading" << std::endl;
    std::cout << "W: Bat/tat che do khung day" << std::endl;
    std::cout << "Kéo chuôt de xoay hinh cau" << std::endl;
    std::cout << "ESC: Thoat" << std::endl;

    glutMainLoop();
    return 0;
}
