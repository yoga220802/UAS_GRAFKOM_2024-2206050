#include <GL/glut.h>

double rx = 0.0;
double ry = 0.0;

bool isCameraAbove = false; // variabel untuk menyimpan status kamera saat ini

float l[] = {0.0, 80.0, 0.0};  // koordinat sumber cahaya
float n[] = {0.0, -1.0, 0.0};  // vektor normal
float e[] = {0.0, -50.0, 0.0}; // titik dari objek yang akan di gunakan untuk memproyeksikan bayangan

// Fungsi untuk mengubah POV kamera menjadi di atas
void changeCameraPOV() {
    if (isCameraAbove) {
        gluLookAt(0.0, 0.0, 150.0,   // Camera position
                  0.0, 0.0, 0.0,      // Look-at position
                  0.0, 1.0, 0.0);     // Up vector
    } else {
        gluLookAt(0.0, 200.0, 10.0,   // Camera position
                  0.0, 0.0, 0.0,      // Look-at position
                  0.0, 1.0, 0.0);     // Up vector
    }
}

// objek yang akan di gambar
void draw()
{
    glutSolidTorus(10, 20, 50, 50);
    // glutSolidSphere(40, 16, 16);
    // glutSolidCube(40);
}

// membuat proyeksi bayangan
void glShadowProjection(float *l, float *e, float *n)
{
    float d, c;
    float mat[16];

    d = n[0] * l[0] + n[1] * l[1] + n[2] * l[2];
    c = e[0] * n[0] + e[1] * n[1] + e[2] * n[2] - d;

    mat[0] = l[0] * n[0] + c; // membuat matrik. OpenGL menggunakan kolom matrik
    mat[4] = n[1] * l[0];
    mat[8] = n[2] * l[0];
    mat[12] = -l[0] * c - l[0] * d;

    mat[1] = n[0] * l[1];
    mat[5] = l[1] * n[1] + c;
    mat[9] = n[2] * l[1];
    mat[13] = -l[1] * c - l[1] * d;

    mat[2] = n[0] * l[2];
    mat[6] = n[1] * l[2];
    mat[10] = l[2] * n[2] + c;
    mat[14] = -l[2] * c - l[2] * d;

    mat[3] = n[0];
    mat[7] = n[1];
    mat[11] = n[2];
    mat[15] = -d;

    glMultMatrixf(mat); // kalikan matrik
}

void render()
{
    glClearColor(0.913,0.282,0.282, 1.0); // rgba(233, 72, 72, 1.0)

    /*Mengaktifkan Fungsi Pencahayaan*/
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    /*Properti Pencahayaan*/
    GLfloat ambient_color[] = {1.0, 0.0, 0.0, 0.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

    /*Memanggil Properti Pencahayaan*/
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHTING, GL_POSITION, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // Reset modelview matrix

    // Set up the camera position and orientation using gluLookAt
changeCameraPOV();

    // Now draw the scene
    glLightfv(GL_LIGHT0, GL_POSITION, l);

    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    // Draw floor
    glColor3f(0.94, 0.392, 0.392); // rgb(240, 100, 100)
    glBegin(GL_QUADS);
    glNormal3f(0.0,1.0,0.0);
    glVertex3f(-1300.0,e[1]-0.1, 1300.0);
    glVertex3f( 1300.0,e[1]-0.1, 1300.0);
    glVertex3f( 1300.0,e[1]-0.1,-1300.0);
    glVertex3f(-1300.0,e[1]-0.1,-1300.0);
    glEnd();

    // Draw shadow
    glPushMatrix();
    glShadowProjection(l, e, n);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rx, 1, 0, 0);
    glDisable(GL_LIGHTING);
    glColor3f(0.4, 0.4, 0.4);
    draw();
    glPopMatrix();

    // Draw object
    glPushMatrix();
    glRotatef(ry, 0, 1, 0);
    glRotatef(rx, 1, 0, 0);
    glEnable(GL_LIGHTING);
    glColor3f(0.627, 0.0, 0.0); // rgb(160, 0, 0)
    draw();
    glPopMatrix();

    glutSwapBuffers();
}


void keypress(unsigned char c, int a, int b)
{
    if (c == 27) { // Tombol "Escape" untuk keluar
        exit(0);
    } else if (c == 'p') { // Tombol "p" untuk mengubah POV kamera
        isCameraAbove = !isCameraAbove;
    }
}

void idle()
{
    rx += 0.5;
    ry += 0.5;
    render();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Yoga Agustiansyah - T. Informatika B - UAS Grafikom 2024");
    glutReshapeFunc(resize);
    glutReshapeWindow(500, 500);
    glutKeyboardFunc(keypress);
    glutDisplayFunc(render);
    glutIdleFunc(idle);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(60.0f, 1.0, 1.0, 400.0);

    // Reset koordinat sebelum dimodifikasi/diubah
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(0.0, 0.0, -150.0);
    glutMainLoop();
    return 0;
}