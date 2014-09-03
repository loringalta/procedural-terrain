/*! \file main.cxx
 * Diamond-Sqare Algorithm: uses freeglut library for a quick and dirty GLUT API. Since I just 
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

float diamond[size][size]; //heightmap array

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
    return (diamond[i][j]);
}

void square(int y, int x, int hs, int disp){
    float t = 0.0f;
    float total = 0.0f;
    if(x == 0)
        total += diamond[(size-1)-(hs/2)][y];
    else
        total += diamond[x-(hs/2)][y];
    if (x == (size-1))
        total += diamond[(hs/2)][y];
    else
        total += diamond[x+(hs)/2][y];

    if (y == 0)
       total += diamond[x][(size-1)-(hs/2)];
   else
        total += diamond[x][y-(hs)/2];
    if (y == (size-1))
        total += diamond[x][hs/2];
    else
        total += diamond[x][y+ (hs/2)];

    total /= 4.0f;
    t = total + random(disp);
    diamond[y][x] = t;
}

/******************************************************************************
 * initHeightField: uses the Diamond-Square Algorithm to write heightmap values to 
 * the array midpoint. 
 ******************************************************************************/
void initHeightField() {

    diamond[0][0] = MIN_Z + random(2.0f);
    diamond[0][size-1] = MIN_Z + random(2.0f);
    diamond[size-1][size-1] = MIN_Z +random(2.0f);
    diamond[size-1][0] = MIN_Z + random(2.0f);

    float disp = 10.0f;
    int incr = size;
    float t = 0.0f;

    while(incr > 1){
       for (int i = 0; i < size-1; i += incr){
            for (int j = 0; j < size-1; j += incr){
                //finding the mean of its four corners augmented bi a random num.
                t = (diamond[j][i]
                + diamond[j+incr][i]
                + diamond[j][i+incr]
                + diamond[j+incr][i+incr])/4 + random(disp);
                diamond[j+(incr/2)][i+(incr/2)] = t;
            }
        }
        //square step. involves setting square to the correct offset 
        for (int i = 0; i < size; i += (incr/2)){
            for (int j = ((i + (incr/2))%incr); j < size; j += incr){
                square(i, j, incr, disp);
            }
        } 
        disp *= pow(2.0,-0.55);
        incr /= 2;
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
                glVertex3f( j, diamond[r][c], i );
                glVertex3f( j, diamond[r+1][c], i + step );
                glVertex3f( j + step, diamond[r][c+1], i );
                glEnd();
                
                glBegin( GL_LINE_LOOP );
                glVertex3f( j + step, diamond[r][c+1], i );
                glVertex3f( j, diamond[r+1][c], i + step );
                glVertex3f( j + step, diamond[r+1][c+1], i + step );
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
      sum = diamond[x][y];
      sum += diamond[x - 1][(y - 1)];
      sum += diamond[x][(y - 1)];
      sum += diamond[x + 1][(y - 1)];
      sum += diamond[x-1][y];
      sum += diamond[x+1][y];
      sum += diamond[x-1][y+1];
      sum += diamond[x][y+1];
      sum += diamond[x+1][y+1]; 

      if(sum % 9 > 4)
        sum = (sum / 9)+1;
      else 
        sum = sum / 9;
      diamond[x][y] = sum;
    }
}

/******************************************************************************
 * main: main function for the program.
 ******************************************************************************/
int main (int argc, char* argv[]) { 
    time_t beginning = time(NULL);
    // Writing Files
    FILE *fp;
    fp = fopen("diamond2049.txt", "a+");//open for writing 
    
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
    gluLookAt( -3.0, 20.0, 30.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 1.0, 100.0 );
    
    t1=clock();
    initHeightField();
    t2=clock();
    // tSmooth(0.65f);

    float diff ((float)t2-(float)t1);
    float seconds = diff / CLOCKS_PER_SEC;
    fprintf(fp, "%f\n", seconds); 
    fclose(fp);//closing the file
    glutMainLoop();
    return 0;
}