/*! \file main.cxx
 * Fault Algorithm: uses freeglut library for a quick and dirty GLUT API. Since I just 
 * needed to display the terrain, I chose to use GLUT because it was the quickest
 * display method. GLUT/glut.h had many deprecated functions so I went with freeglut which
 * provides backwards compatibility with GLUT. 
 * \Jennifer Ma
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/freeglut.h>
#include <math.h>
#include <time.h>

using namespace std;

const int size = 50;            
const float step = 1.0;                          
const float MAX_X = step * (size/2);       
const float MIN_X =  - MAX_X;                       
const float MAX_Z = step * (size/2);      
const float MIN_Z =  - MAX_Z;
const float rand_m = 2147483647.0f; 

float fault[size][size];              // The height array itself

/******************************************************************************
 * random: enter a max value and return a random value between -max, and max.
 ******************************************************************************/
float random(float max){
    float r = rand();
    return (r/rand_m) * max - (max * 0.5);
}

/******************************************************************************
 * initHeightField: uses the Fault-Line Algorithm to write heightmap values to 
 * the array Fault. 
 ******************************************************************************/
void initHeightField() {
    fault[0][0] = MIN_Z + random(2.0f);
    fault[0][size-1] = MIN_Z + random(2.0f);
    fault[size-1][size-1] = MIN_Z +random(2.0f);
    fault[size-1][0] = MIN_Z + random(2.0f);

    float a, b, c;
    float disp = 0.1; 
    float aux; //line formula variables ax + bz = c
    for (int i = 0; i < 500; i++){
        a = cos(random(100.0));
        b = sin(random(100.0));
        c = ((float)rand() / rand_m) * 2 * sqrt(pow(size, 2)/2)  - sqrt(pow(size, 2)/2); 
        for (int r = 0; r < size; r++){
            for (int t = 0; t < size; t++){
                if ((r - (size/2)) * a + (t - (size/2)) * b + c > 0)
                    aux = disp;
                else
                    aux = -disp;
                fault[r][t] += aux;
            }
        }
    }
}


/******************************************************************************
 * initHeightField: uses the Fault Algorithm to write heightmap values to 
 * the array Fault. 
 ******************************************************************************/
void display( void ) { 
    glClearColor( 0.97, 0.89, 0.60, 0.0 );
    glClearDepth( 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    for ( int r = 0; r < size; r++ ) {       
        float i = MIN_Z + r * step;       
        for ( int c = 0; c < size; c++ ) {            
            float j = MIN_X + c * step;
            if (r < size - 1  &&  c < size - 1  ) {
                
                glColor3f( 1.0, 1.0, 1.0 );
                glBegin( GL_LINE_LOOP );
                glVertex3f( j, fault[r][c], i );
                glVertex3f( j, fault[r+1][c], i + step );
                glVertex3f( j + step, fault[r][c+1], i );
                glEnd();
                
                glBegin( GL_LINE_LOOP );
                glVertex3f( j + step, fault[r][c+1], i );
                glVertex3f( j, fault[r+1][c], i + step );
                glVertex3f( j + step, fault[r+1][c+1], i + step );
                glEnd();
            }
        }
    }       
    glFlush();
}

/******************************************************************************
 * reshape: given new width and height does window resize callback 
 ******************************************************************************/
void reshape( const int width, const int height) {
    glViewport( 0, 0, width, height );
}

/******************************************************************************
 * main: main function for the program.
 ******************************************************************************/
int main (int argc, char* argv[]) {
    time_t beginning = time(NULL);
    // Writing Files
    FILE *fp;
    fp = fopen("fault2000.txt", "a+");//open for writing 
    
    clock_t t1,t2;
    // Initialize windows and input:

    srand(beginning);//set the random seed
    glutInit( &argc, argv );
    glutInitWindowPosition( 200, 0 );
    glutInitWindowSize( 500, 500 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH );
    glutCreateWindow("Fault");
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );

    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( -1.0, 10.0, 20.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 1.0, 100.0 );
    
    t1=clock();
    initHeightField();
    t2=clock();

    float diff ((float)t2-(float)t1);
    float seconds = diff / CLOCKS_PER_SEC;
    fprintf(fp, "%f\n", seconds); 
    fclose(fp);//closing the file
    glutMainLoop();
    return 0;
}