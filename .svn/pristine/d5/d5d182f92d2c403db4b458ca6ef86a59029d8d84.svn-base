/*! \file main.cxx
 * Perlin Algorithm: uses freeglut library for a quick and dirty GLUT API. Since I just 
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
//perlin129
const int size = 55;  // 2n+1 ex: 65, 129, 257, 513, 1025             
const float step = 1.0;                          
const float MAX_X = step * (size/2);       
const float MIN_X =  - MAX_X;                       
const float MAX_Z = step * (size/2);      
const float MIN_Z =  - MAX_Z;
const float rand_m = 2147483647.0f; //largest signed integer in 32 bits

float perlin[size][size]; //heightmap array
int permutation[512]; //random number array
static float gradients[8][2] = 
{
  { -1.0f, -1.0f }, { 1.0f, 0.0f } , { -1.0f, 0.0f } , { 1.0f, 1.0f } ,
  { -1.0f, 1.0f } , { 0.0f, -1.0f } , { 0.0f, 1.0f } , { 1.0f, -1.0f }
};

/******************************************************************************
 * random: enter a max value and return a random value between -max, and max.
 ******************************************************************************/
float random(float max){
    float r = rand();
    return (r/rand_m) * max - (max * 0.5);
}

float dot(float grad[], float x, float y)
{
    return (grad[0] * x + grad[1] * y);
}

float lerp(float t, float a, float b) 
{ 
    return a + t * (b - a); 
}

/******************************************************************************
 * grad: taken directly from Ken Perlin's 2002 SIGGRAPH paper. 
 * http://mrl.nyu.edu/~perlin/noise/
 ******************************************************************************/
float grad(int h1, float i, float j, float k) {
  int h = 0; 
  float u = 0.0f;
  float v = 0.0f;
  h = h1 & 15;                     
  u = h < 8 ? i : j,                 
  v = h < 4 ? j : h == 12 || h==14 ? i : k;
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
} 

/******************************************************************************
 * permute: set up random numbers table 
 ******************************************************************************/
void permute(){ 
    int p[256];
    for (int i = 0; i < 256; i++)
        p[i] = (int)random(256);//put each number in once
    for (int i = 0; i < 256; i++)
    {
        p[256+i] = p[i] = permutation[i];
    }
}

/******************************************************************************
 * noise: 2D noise. for each pixel coordindate (i, j) calculate the 
 * dot product of the distance and gradient vectors. Interpolate these noise values
 ******************************************************************************/
float noise(int i, int j, float freq){
    int x    = floor(j * freq);
    int y    = floor(i * freq);

    //fractional grid points
    float fx = (j * freq) - x;
    float fy = (i * freq) - y;
    
    //indexing into the gradients for the four nearby points
    int g1   = permutation[(x + permutation[y & 255]) & 255] & 7;
    int g2   = permutation[(x + 1 + permutation[y & 255]) & 255] & 7;
    int g3   = permutation[(x + permutation[(y + 1) & 255]) & 255] & 7;
    int g4   = permutation[(x + 1 + permutation[(y + 1) & 255]) & 255] & 7;//
    
    //dot products that get the noise from each corner.
    float n1 = dot(gradients[g1], fx, fy);
    float n2 = dot(gradients[g2], fx - 1.0f, fy);
    float n3 = dot(gradients[g3], fx, fy - 1.0f);
    float n4 = dot(gradients[g4], fx - 1.0f, fy - 1.0f);

    //fade value calculated by Ken Perlin
    float sx = fx * fx * fx * (fx * ( 6 * fx - 15) + 10);
    float sy = fx * fx * fx * (fx * ( 6 * fy - 15) + 10);

    //interpolation along x axis
    float i1 = lerp(n1, n2, sx);
    float i2 = lerp(n3, n4, sx);

    return lerp(i1, i2, sy);
}

/******************************************************************************
 * initHeightField: uses the perlin Algorithm to write heightmap values to 
 * the array perlin. 
 ******************************************************************************/
void initHeightField() {
    //initializes the four corners of the map
    perlin[0][0] = MIN_Z + random(1.0f);
    perlin[0][size-1] = MIN_Z + random(1.0f);
    perlin[size-1][0] = MIN_Z + random(1.0f);
    perlin[size-1][size-1] = MIN_Z + random(1.0f);
    int oct = 16;  
    float gain = 0.65f; 
    float lacunarity = 2.5f;

    float amp, freq, pix;
   
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            amp = 1.0f;
            freq = 1.0f / (float)size;
            pix = 0.0f;
            for (int k = 0; k < oct; ++k)
            {
                float lerped = noise(i, j, freq);
                pix += lerped * amp;
                amp *= gain;
                freq *= lacunarity;
            }
            perlin[j][i] = pix;
        }
    }
}

/******************************************************************************
 * display: displays heightmap as 3D terrain
 ******************************************************************************/
void display( void ) { 
    glClearColor( 0.4, 0.4, 0.4, 0.0 );
    glClearDepth( 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    for ( int r = 0; r < size; r++ ) {       
        float z = MIN_Z + r * step;       
        for ( int c = 0; c < size; c++ ) {            
            float x = MIN_X + c * step;
            if (r < size - 1  &&  c < size - 1  ) {
                
                glColor3f( 1.0, 1.0, 1.0 );
                
                glBegin( GL_LINE_LOOP );
                glVertex3f( x, perlin[r][c], z );
                glVertex3f( x, perlin[r+1][c], z + step );
                glVertex3f( x + step, perlin[r][c+1], z );
                glEnd();
                
                glBegin( GL_LINE_LOOP );
                glVertex3f( x + step, perlin[r][c+1], z );
                glVertex3f( x, perlin[r+1][c], z + step );
                glVertex3f( x + step, perlin[r+1][c+1], z + step );
                glEnd();
            }
        }
    }       
    // Flush output to the screen:
    glFlush();
}

/******************************************************************************
 * reshape: given new width and height does window resize callback 
 ******************************************************************************/
void reshape( const int width, const int height) {
    glViewport( 0, 0, width, height );
}

void smooth() {
    int x, y, sum;
  for (x = 1; x < size - 1; x++)
    for (y = 1; y < size - 1; y++)
    {
      sum = perlin[x][y];
      sum += perlin[x - 1][(y - 1)];
      sum += perlin[x][(y - 1)];
      sum += perlin[x + 1][(y - 1)];
      sum += perlin[x-1][y];
      sum += perlin[x+1][y];
      sum += perlin[x-1][y+1];
      sum += perlin[x][y+1];
      sum += perlin[x+1][y+1]; 

      if(sum % 9 > 4)
        sum = (sum / 9)+1;
      else 
        sum = sum / 9;
      perlin[x][y] = sum;
    }
}

/******************************************************************************
 * main: main function for the program.
 ******************************************************************************/
int main (int argc, char* argv[]) { 
    time_t beginning = time(NULL);
    // Writing Files
    FILE *fp;
    fp = fopen("perlintest.txt", "a+");//open for writing 
    
    clock_t t1,t2;
    // Initialize windows and input:

    srand(beginning);//set the random seed
    glutInit( &argc, argv );
    glutInitWindowPosition( 200, 0 );
    glutInitWindowSize( 500, 500 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH );
    glutCreateWindow("perlin");
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );

    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( -3.0, 20.0, 15.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 1.0, 100.0 );
    
    t1=clock();
    initHeightField();
    t2=clock();
    smooth();
    // terrainSmooth(0.75);
    float diff ((float)t2-(float)t1);
    float seconds = diff / CLOCKS_PER_SEC;
    fprintf(fp, "%f\n", seconds); 
    fclose(fp);//closing the file
    glutMainLoop();
    return 0;
}

