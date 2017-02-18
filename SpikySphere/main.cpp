#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<bits/stdc++.h>
#include <stdlib.h>
using namespace std;

static int slices = 20;
static int stacks = 20;

vector<float>arrow_x1;
vector<float>arrow_x2;
vector<float>arrow_y1;
vector<float>arrow_y2;

vector<float>arrow_angle;

vector<bool>arrow_touched;
int Num_Arrows = 0;

float def_x1 = 0;
float def_y1 = -1.5;
float arrow_length = 0.5;

float arrow_speed = 0.01;
float ball_radius = 0.5;
float ball_x = 0;
float ball_y = 1.2;

int score = 0;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);


}

void Update_Arrow()
{
    for (int I = Num_Arrows - 1; I>= 0; I--)
    {
        if (arrow_touched[I] == false)
        {
            float y1 = arrow_y1[I];
            y1 = y1 + arrow_speed;

            if (y1 >= ball_y - ball_radius*2)
            {
                arrow_touched[I] = true;
                score++;
                system("CLS");
                cout << "\t\tCurrent Score : " << score << endl;
            }
            else
            {
                arrow_y1[I] = y1;
                arrow_y2[I] = y1 + arrow_length;
            }

        }
        else
        {
            float x1 = arrow_x1[I];
            float y1 = arrow_y1[I];
            float x2 = arrow_x2[I];
            float y2 = arrow_y2[I];
            float ang = arrow_angle[I];
            ang = ang + 0.05; // 0.05 angular speed
            if (ang >= 360)
                ang = 0;

            ang = ang * 3.1416 / 180.0;
            ang = ang - 3.1416/2;

            x1 = ball_x + cos(ang)*ball_radius;
            y1 = ball_y + sin(ang)*ball_radius;
            x2 = ball_x + cos(ang)*(ball_radius*2);
            y2 = ball_y + sin(ang)*(ball_radius*2);

            arrow_x1[I] = x1;
            arrow_x2[I] = x2;
            arrow_y1[I] = y1;
            arrow_y2[I] = y2;


            float new_ang = ((ang+3.1416/2) * 180.0) / 3.1416;

            for (int J = Num_Arrows - 1; J >= 0; J--)
            {
                if (J != I)
                {
                    float ang_diff = arrow_angle[I] - arrow_angle[J];
                    if (ang_diff < 0) ang_diff = -ang_diff;
                    if (ang_diff < 5)
                    {
                        system("CLS");
                        cout << "*********************************************" << endl;
                        cout << "\t\t    Your Score : " << score << endl;
                        cout << "\n\t\t    GAME OVER!" << endl;
                        cout << "*********************************************" << endl;
                        exit(1);
                    }
                }
            }

            arrow_angle[I] = new_ang;
        }
    }
}

void Add_Arrow()
{
    arrow_x1.push_back(def_x1);
    arrow_y1.push_back(def_y1);

    float ang = -90 * 3.1416 / 180.0;
    arrow_angle.push_back(ang);

    float x2 = def_x1 + cos(ang) * arrow_length;
    float y2 = def_y1 + sin(ang) * arrow_length;

    arrow_x2.push_back(x2);
    arrow_y2.push_back(y2);

    arrow_touched.push_back(false);

    Num_Arrows++;
}

void DrawArrows()
{
    for (int I = Num_Arrows - 1; I >= 0; I--)
    {
        glLineWidth(10);
        glPushMatrix();
        glBegin(GL_LINES);
        glVertex2f(arrow_x1[I], arrow_y1[I]);
        glVertex2f(arrow_x2[I], arrow_y2[I]);
        glEnd();
        glPopMatrix();
    }
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt(0,0,5,0,0,0,0,1,0);

    glPushMatrix();
    glTranslated(ball_x,ball_y,0);
    glutSolidSphere(ball_radius,slices,stacks);
    glPopMatrix();


    glColor3d(0,.6,0.2);
    Update_Arrow();
    DrawArrows();

    //
    glLineWidth(10);
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex2f(def_x1, def_y1);
    glVertex2f(def_x1, def_y1-arrow_length);
    glEnd();
    glPopMatrix();


    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;

    case 'a':
        Add_Arrow();
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Spiky Sphere: (Press a to fire arrow)");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
