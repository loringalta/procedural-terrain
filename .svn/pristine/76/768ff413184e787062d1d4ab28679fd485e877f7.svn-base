/*! \file main.cxx
 * Particle Algorithm: uses freeglut library for a quick and dirty GLUT API. Since I just 
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

const int size = 50;  // 2n+1 ex: 65, 129, 257, 513, 1025, 2049            
const float step = 1.0;                          
const float MAX_X = step * (size/2);       
const float MIN_X =  - MAX_X;                       
const float MAX_Z = step * (size/2);      
const float MIN_Z =  - MAX_Z;
const float rand_m = 2147483647.0f; //largest signed integer in 32 bits

float particle[size][size]; //heightmap array

/******************************************************************************
 * random: enter a max value and return a random value between -max, and max.
 ******************************************************************************/
float random(float max){
    float r = rand();
    return (r/rand_m) * max - (max * 0.5);
}

/******************************************************************************
 * add: deposits values onto a height map.
 ******************************************************************************/
void add(int i, int j){
    bool deposit = false; 
    int r1, c1;
    for (int r = -1; r < 2; r++){
        for (int c = -1; c < 2; c++){
            //checking to make sure don't access array out of bounds.
            if( (r!=0) && (c!=0) && (i + r) > -1 && (i+r < size)) 
                if(particle[(i+r)][j+c] < particle[i][j]){
                    deposit = true; 
                    r1 = r; 
                    c1 = c;
                } //checking height of adjacent particles

        }
    }
    if(deposit)
        add(i+r1, j+c1);
    else
        particle[i][j] += 1.5f;
}

/******************************************************************************
 * initHeightField: uses the Particle Deposition Algorithm to write heightmap 
 * values to  the array midpoint. 
 ******************************************************************************/
void initHeightField() {
    int w1 = rand()%size;
    int w2 = rand()%size;
    for (int i = 0; i < 1000; i++){
        int wall = rand()%4;
        switch(wall)
        {
            case 0: {
                w2--;
                if(w2 == -1)
                    w2 = size - 1;
                break;
            }
            case 1: {
                w2++;
                if(w2 >= size)
                    w2 = 0;
                break;
            }
            case 2: {
                w1++;
                if(w1 >= size)
                    w1 = 0;
                break;
            }
            case 3: {
                w1--;
                if(w1 == -1)
                    w1 = size - 1;
                break;
            }
        }
        add(w1, w2);
    }
}


/******************************************************************************
 * display: displays heightmap as 3D terrain
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
                glVertex3f( j, particle[r][c], i );
                glVertex3f( j, particle[r+1][c], i + step );
                glVertex3f( j + step, particle[r][c+1], i );
                glEnd();
                
                glBegin( GL_LINE_LOOP );
                glVertex3f( j + step, particle[r][c+1], i );
                glVertex3f( j, particle[r+1][c], i + step );
                glVertex3f( j + step, particle[r+1][c+1], i + step );
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
    fp = fopen("particle2000.txt", "a+");//open for writing 
    
    clock_t t1,t2;
    // Initialize windows and input:

    srand(beginning);//set the random seed
    glutInit( &argc, argv );
    glutInitWindowPosition( 200, 0 );
    glutInitWindowSize( 500, 500 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH );
    glutCreateWindow("particle");
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );

    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( -1.0, 25.0, 15.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );
    
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