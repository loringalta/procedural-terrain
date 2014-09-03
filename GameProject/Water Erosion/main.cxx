/*! \file main.cxx
 * Water Erosion Algorithm: uses freeglut library for a quick and dirty GLUT API. Since I just 
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
#define FLT_MAX     3.40282347E+38F

const int size = 50;          
const float step = 1.0;                          
const float MAX_X = step * (size/2);       
const float MIN_X =  - MAX_X;                       
const float MAX_Z = step * (size/2);      
const float MIN_Z =  - MAX_Z;
const float rand_m = 2147483647.0f; //largest signed integer in 32 bits

float erosion[size][size]; //heightmap array
float water[size][size]; 

/******************************************************************************
 * random: enter a max value and return a random value between -max, and max.
 ******************************************************************************/
float random(float max){
    float r = rand();
    return (r/rand_m) * max - (max * 0.5);
}

/******************************************************************************
 * initHeightField: uses the Fault-Line Algorithm to write heightmap values to 
 * the array. 
 ******************************************************************************/
void initHeightField() {
    erosion[0][0] = MIN_Z + random(2.0f);
    erosion[0][size-1] = MIN_Z + random(2.0f);
    erosion[size-1][size-1] = MIN_Z +random(2.0f);
    erosion[size-1][0] = MIN_Z + random(2.0f);

    float a, b, c;
    float disp = 0.1f; 
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
                erosion[r][t] += aux;
            }
        }
    }
}

/******************************************************************************
 * zeroOut: initializing every value in array to 0.0f;
 ******************************************************************************/
void zeroOut(){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            water[i][j] = 0.0f;
        }
    }
}

/******************************************************************************
 * initHeightField: simulate water movement due to gravity
 ******************************************************************************/
void movement(){
    float curr, diff, max= 0.0f;
    int low_x, low_y;
    for(int m = 1; m < (size -1); m++){
        for(int n = 1; n < (size -1); n++){
            curr = erosion[m][n] + water[m][n];
            max = -FLT_MAX;
            for(int x = -1; x < 2; x += 1){
                for (int y = -1; y < 2; y += 1){
                    diff = curr - erosion[m+x][n+y] - water[m+x][n+x];
                    if(diff > max){
                        max = diff;
                        low_x = x;
                        low_y = y;
                    }
                }
            }
            if (max > 0.0f)
            {
                if(water[m][n] < max)
                {
                    water[m+low_x][n+low_y] += water[m][n];
                    water[m][n] = 0.0f;
                }
                else 
                {
                    water[m+low_x][n+low_y] += max/2.0f;
                    water[m][n] -= max/2.0f;
                }
            }
        }
    }
}

/******************************************************************************
 * waterErosion: emulates the steps of natural erosion
 * rainfall, erosion, movement.
 ******************************************************************************/
void waterErosion(int iter){
    for(int i = 0; i < iter; i++){
        //it's raining, it's pouring...incrementing each cells with "rain"
        for (int rainx = 0; rainx < size; rainx++)
            for (int rainy = 0; rainy < size; rainy++)
                water[rainx][rainy] += 0.01f;
        //simulating the "erosion" of height from erosion heightmap
        for (int solublex = 0; solublex < size; solublex++)
            for(int solubley = 0; solubley < size; solubley++)
                erosion[solublex][solubley] -= water[solublex][solubley] * 0.01f;
        //particle movement
        movement();
        for(int r = 0; r < size; r++)
            for (int c = 0; c < size; c++){
                float water_lost = water[r][c] * 0.9f;
                water[r][c] -= water_lost;
                erosion[r][c] += water_lost * 0.01f;
            }
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
                glVertex3f( j, erosion[r][c], i );
                glVertex3f( j, erosion[r+1][c], i + step );
                glVertex3f( j + step, erosion[r][c+1], i );
                glEnd();
                
                glBegin( GL_LINE_LOOP );
                glVertex3f( j + step, erosion[r][c+1], i );
                glVertex3f( j, erosion[r+1][c], i + step );
                glVertex3f( j + step, erosion[r+1][c+1], i + step );
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
    fp = fopen("erosion1000.txt", "a+");//open for writing 
    
    clock_t t1,t2;
    // Initialize windows and input:

    srand(beginning);//set the random seed
    glutInit( &argc, argv );
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( 500, 500 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH );
    glutCreateWindow("Erosion");
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );

    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( -1.0, 10.0, 20.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 1.0, 100.0 );
    
    zeroOut();
    initHeightField();
    t1=clock();
    waterErosion(5000);
    t2=clock();

    float diff ((float)t2-(float)t1);
    float seconds = diff / CLOCKS_PER_SEC;
    fprintf(fp, "%f\n", seconds); 
    fclose(fp);//closing the file
    glutMainLoop();
    return 0;
}