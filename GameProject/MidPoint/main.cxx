/*! \file main.cxx
 * Midpoint Algorithm: uses freeglut library for a quick and dirty GLUT API. Since I just 
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

const int size = 33;  // 2n+1 ex: 65, 129, 257, 513, 1025, 2049            
const float step = 1.0;                          
const float MAX_X = step * (size/2);       
const float MIN_X =  - MAX_X;                       
const float MAX_Z = step * (size/2);      
const float MIN_Z =  - MAX_Z;
const float rand_m = 2147483647.0f; //largest signed integer in 32 bits

float midpoint[size][size]; //heightmap array

/******************************************************************************
 * random: enter a max value and return a random value between -max, and max.
 ******************************************************************************/
float random(float max){
    float r = rand();
    return (r/rand_m) * max - (max * 0.5);
}

/******************************************************************************
 * height: finds the height of the corner depending on i, j placement. 
 ******************************************************************************/
float height(int i, int j) {
    if (i > size - 1) 
        i -= (size - 1); 
    else if (i < 0)
        i += (size -1); 

    if (j > size -1)
        j -= (size - 1);
    return (midpoint[i][j]);
}

/******************************************************************************
 * diamond: computes the average height of the four corners. random(range) 
 * represents the maximum displacement allowed. 
 ******************************************************************************/
void diamond(int i, int j, int incr, int range){
    float total_corners = (height(i, j)
                        +  height(i + incr, j) 
                        +  height(i + incr, j + incr)
                        +  height(i, j + incr));

    midpoint[(i+incr/2)][j+incr/2] = total_corners / 4; 
    midpoint[(i+incr/2)][j+incr/2] += random(range);
}

/******************************************************************************
 * non_border: regular nonborder averages.
 ******************************************************************************/
float non_border(int l, int m, int incr){
    return (height(l,m+incr/2) 
        + height(l,m-incr/2) 
        + height(l-incr/2,m) 
        + height(l+incr/2,m)) / 4;
}

/******************************************************************************
 * square: midpoints between the four corners are computed as an 
 * average of connected points. 
 ******************************************************************************/
 void square(int i, int j, int incr, int range){
    int l, m = 0;

    l = (i + incr/2) + incr/2;
    m = (j + incr/2);
    switch(l) 
    {
        case (size-1):{
            midpoint[l][m] = (height(l, m + incr/2) + height(l,m - incr/2) + height(l + incr/2,m))/3;
            break;
        }
        default: {
            midpoint[l][m] = non_border(l, m, incr);
            break;
        }
    }
    midpoint[l][m] += random(range);

    l = (i + incr/2);
    m = (j + incr);

    switch(m)
    {
        case (size-1):{
            midpoint[l][m] = (height(l,m-incr/2) + height(l-incr/2,m) + height(l+incr/2,m))/3;
            break;
        }
        default:{
            midpoint[l][m] = non_border(l, m, incr);
            break;
        }
    }
    midpoint[l][m] += random(range);
    
    l = i;
    m = (j + incr/2);
    if (!l){
        midpoint[l][m] = (height(l,m+incr/2)+height(l,m-incr/2)+height(l+incr/2,m))/3;
        midpoint[l][m] += random(range);
    }

    l = (i + incr/2);
    m = j;
    if (!m){
        midpoint[l][m] = (height(l,m+incr/2)+height(l-incr/2,m)+height(l+incr/2,m))/3;
        midpoint[l][m] += random(range);
    }
}

/******************************************************************************
 * initHeightField: uses the Midpoint Algorithm to write heightmap values to 
 * the array midpoint. 
 ******************************************************************************/
void initHeightField() {

    //initializing the four corners of the matrix. seeding the values 
    midpoint[0][0] = MIN_Z + random(2.0f);
    midpoint[0][size-1] = MIN_Z + random(2.0f);
    midpoint[size-1][size-1] = MIN_Z +random(2.0f);
    midpoint[size-1][0] = MIN_Z + random(2.0f);

    int incr = (size - 1)/2; 
    float range = 20.0f; //initial max displacement

    for (incr = size-1; incr > 1; incr /= 2){
        for (int i = 0; i < size-2; i += incr){
            for (int j = 0; j < size-2; j += incr){
                diamond(i, j, incr, range);
            }
        }
        for (int i = 0; i < size -2; i += incr){
            for (int j = 0; j < size-2; j += incr){
                square(i, j, incr, range);
            }
        }
        range *= pow(2.0,-0.55);
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
                glVertex3f( j, midpoint[r][c], i );
                glVertex3f( j, midpoint[r+1][c], i + step );
                glVertex3f( j + step, midpoint[r][c+1], i );
                glEnd();
                
                glBegin( GL_LINE_LOOP );
                glVertex3f( j + step, midpoint[r][c+1], i );
                glVertex3f( j, midpoint[r+1][c], i + step );
                glVertex3f( j + step, midpoint[r+1][c+1], i + step );
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
 * smooth: band smoothing. takes into account its neightbor's heights
 ******************************************************************************/
void smooth() {
    int x, y, sum;
  for (x = 1; x < size - 1; x++)
    for (y = 1; y < size - 1; y++)
    {
      sum = midpoint[x][y];
      sum += midpoint[x - 1][(y - 1)];
      sum += midpoint[x][(y - 1)];
      sum += midpoint[x + 1][(y - 1)];
      sum += midpoint[x-1][y];
      sum += midpoint[x+1][y];
      sum += midpoint[x-1][y+1];
      sum += midpoint[x][y+1];
      sum += midpoint[x+1][y+1]; 

      if(sum % 9 > 4)
        sum = (sum / 9)+1;
      else 
        sum = sum / 9;
      midpoint[x][y] = sum;
    }
}

/******************************************************************************
 * main: main function for the program.
 ******************************************************************************/
int main (int argc, char* argv[]) { 
    time_t beginning = time(NULL);
    // Writing Files
    FILE *fp;
    fp = fopen("2049.txt", "a+");//open for writing 
    
    clock_t t1,t2;
    // Initialize windows and input:

    srand(beginning);//set the random seed
    glutInit( &argc, argv );
    glutInitWindowPosition( 200, 0 );
    glutInitWindowSize( 500, 500 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH );
    glutCreateWindow("Midpoint");
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );

    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( -1.0, 10.0, 10.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 1.0, 100.0 );
    
    t1=clock();
    initHeightField();
    t2=clock();
    smooth();

    float diff ((float)t2-(float)t1);
    float seconds = diff / CLOCKS_PER_SEC;
    fprintf(fp, "%f\n", seconds); 
    fclose(fp);//closing the file
    glutMainLoop();
    return 0;
}