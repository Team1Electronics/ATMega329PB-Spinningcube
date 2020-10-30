/*
 * Cube.c
 *
 * Created: 20/10/2020 12:33:48
 *  Author: Petervg
 
 * The code for rotating the cube is based on code written by Colin Ord (https://sites.google.com/site/colinord) posted on 
 * blogspot (http://colinord.blogspot.com/2015/01/arduino-oled-module-with-3d-demo.html)
 
 */ 

#include <stdint.h>
#include <avr/pgmspace.h>

#include "global.h"

#include "cube.h"
#include "global.h"
#include "HAL_LCD/SSD1306.h"

#define XCOORD	0
#define YCOORD	1
#define ZCOORD	2

uint8_t g_CubeSize = 20;

// -------------------------------------------------------------------
// 0 to 90 degrees fixed point COSINE look up table (14-bit, 0..16383)
// -------------------------------------------------------------------
const float lut[] PROGMEM = 
{         
	0			,0.017452406	,0.034899497	,0.052335956	,0.069756474	,0.087155743	,0.104528463	,0.121869343	,0.139173101	,0.156434465	,
	0.173648178	,0.190808995	,0.207911691	,0.224951054	,0.241921896	,0.258819045	,0.275637356	,0.292371705	,0.309016994	,0.325568154	,
	0.342020143	,0.35836795		,0.374606593	,0.390731128	,0.406736643	,0.422618262	,0.438371147	,0.4539905		,0.469471563	,0.48480962		,
	0.5			,0.515038075	,0.529919264	,0.544639035	,0.559192903	,0.573576436	,0.587785252	,0.601815023	,0.615661475	,0.629320391	,
	0.64278761	,0.656059029	,0.669130606	,0.68199836		,0.69465837		,0.707106781	,0.7193398		,0.731353702	,0.743144825	,0.75470958		,
	0.766044443	,0.777145961	,0.788010754	,0.79863551		,0.809016994	,0.819152044	,0.829037573	,0.838670568	,0.848048096	,0.857167301	,
	0.866025404	,0.874619707	,0.882947593	,0.891006524	,0.898794046	,0.906307787	,0.913545458	,0.920504853	,0.927183855	,0.933580426	,
	0.939692621	,0.945518576	,0.951056516	,0.956304756	,0.961261696	,0.965925826	,0.970295726	,0.974370065	,0.978147601	,0.981627183	,
	0.984807753	,0.987688341	,0.990268069	,0.992546152	,0.994521895	,0.996194698	,0.99756405		,0.998629535	,0.999390827	,0.999847695	,
	1
};


// ------------------------------------------------
// Cube nodes and edges
// ------------------------------------------------
int8_t nodes[8][3] = 
{
	{-1,-1,-1},		// Node 0
	{-1,-1,1},		// Node 1
	{-1,1,-1},		// Node 2
	{-1,1,1},		// Node 3
	{1,-1,-1},		// Node 4
	{1,-1,1},		// Node 5
	{1,1,-1},		// Node 6
	{1,1,1}			// Node 7
};

int8_t nodes_resized[8][3];
int8_t nodes_rotated[8][3];

int8_t edges[12][2] =
{
	{0, 1},				// Edge 0 (from node 0 to node 1)
	{1, 3},				// Edge 1 (from node 1 to node 3)
	{3, 2},				// Edge 2 (from node 3 to node 2)
	{2, 0},				// Edge 3 (from node 2 to node 0)
	{4, 5},				// Edge 4 (from node 4 to node 5)
	{5, 7},				// Edge 5 (from node 5 to node 7)
	{7, 6},				// Edge 6 (from node 7 to node 6)
	{6, 4},				// Edge 7 (from node 6 to node 4)
	{0, 4},				// Edge 8 (from node 0 to node 4)
	{1, 5},				// Edge 9 (from node 1 to node 5)
	{2, 6},				// Edge 10 (from node 2 to node 6)
	{3, 7}				// Edge 11 (from node 3 to node 7)
};

// ----------------------------------------------------------------------
// Create a the matrix containing the 3D coordinates of an unrotated cube
// ----------------------------------------------------------------------
void ResizeCube(uint8_t l_CubeSize)
{
	for (uint8_t node=0 ; node<8; node++)
	{
		for (uint8_t coord=0 ; coord<3 ; coord++)
		{
			nodes_resized[node][coord] = nodes[node][coord] * l_CubeSize;
		}
	}
}


// ----------------------------------------------
// SIN/COS from 90 degrees LUT
// ----------------------------------------------
float SIN(uint16_t angle) 
{
	uint32_t temp;
	float result;
	
	if (angle > 0 && angle < 91)
	{
		temp = pgm_read_dword(&lut[angle]);
		result = *((float*)&temp);
		return result;
	}
	
	if (angle > 90 && angle < 181)
	{
		temp = pgm_read_dword(&lut[180-angle]);
		result = *((float*)&temp);
		return result;
	}
		
	if (angle > 180 && angle < 271) 
	{
		temp = pgm_read_dword(&lut[angle-180]);
		result = *((float*)&temp);
		return -result;
	}
	
	if (angle > 270 && angle < 361) 
	{
		temp = pgm_read_dword(&lut[360-angle]);
		result = *((float*)&temp);
		return -result;
	}
	
	return 0;
}


float COS(uint16_t angle) 
{
	uint32_t temp;
	float result;
	
	if (angle > 0 && angle < 91)
	{
		temp = pgm_read_dword(&lut[90-angle]);
		result = *((float*)&temp);
		return result;
	}
	
	if (angle > 90 && angle < 181)
	{
		temp = pgm_read_dword(&lut[angle-90]);
		result = *((float*)&temp);
		return -result;
	}
	
	if (angle > 180 && angle < 271)
	{
		temp = pgm_read_dword(&lut[270-angle]);
		result = *((float*)&temp);
		return -result;
	}
	
	if (angle > 270 && angle < 361)
	{
		temp = pgm_read_dword(&lut[angle-270]);
		result = *((float*)&temp);
		return result;
	}
	
	return 0;
}


void DrawCubeRotated(struct _SSD1306FrameBuffer* l_FrameBuffer, uint8_t l_Xoffset, uint8_t l_Yoffset, uint16_t l_AngleX, uint16_t l_AngleY, uint16_t l_AngleZ)
{
	// This code loops through the array of edges. It gets the two numbers defined by an edge and looks up the corresponding
	// node in the nodes array. Then it draws a line from the (x, y) coordinate of the first node to the (x, y) coordinate of the second node.
	
	uint8_t l_edge, l_n0, l_n1;

	
	RotateCube(l_AngleX, l_AngleY, l_AngleZ);
	
	for (l_edge=0; l_edge<12; l_edge++)
	{
		l_n0 = edges[l_edge][0];	// Start node
		l_n1 = edges[l_edge][1];	// end node	
		
		ssd1306_drawline(&SSD1306FrameBuffer1, nodes_rotated[l_n0][XCOORD] + l_Xoffset, nodes_rotated[l_n0][YCOORD]+l_Yoffset, nodes_rotated[l_n1][XCOORD]+l_Xoffset, nodes_rotated[l_n1][YCOORD]+l_Yoffset);
	}
}


void RotateCube(uint16_t l_AngleX, uint16_t l_AngleY, uint16_t l_AngleZ)
{
	uint8_t l_Node;
	float rotx, roty, rotz, rotxx, rotyy, rotzz, rotxxx, rotyyy, rotzzz;
	
	float AngleSineX = SIN(l_AngleX);
	float AngleCosineX = COS(l_AngleX);
	float AngleSineY = SIN(l_AngleY);
	float AngleCosineY = COS(l_AngleY);
	float AngleSineZ = SIN(l_AngleZ);
	float AngleCosineZ = COS(l_AngleZ);	
	
		
	for (l_Node = 0; l_Node<8; l_Node++)
	{
		//rotateY
		rotz = (float)nodes_resized[l_Node][ZCOORD] * AngleCosineY - (float)nodes_resized[l_Node][XCOORD] * AngleSineY;
		rotx = (float)nodes_resized[l_Node][ZCOORD] * AngleSineY + (float)nodes_resized[l_Node][XCOORD] * AngleCosineY;
		roty = (float)nodes_resized[l_Node][YCOORD];
		
		//rotateX
		rotyy = roty * AngleCosineX - rotz * AngleSineX;
		rotzz = roty * AngleSineX + rotz * AngleCosineX;
		rotxx = rotx;
		
		//rotateZ
		rotxxx = rotxx * AngleCosineZ - rotyy * AngleSineZ;
		rotyyy = rotxx * AngleSineZ + rotyy * AngleCosineZ;
		rotzzz = rotzz;

		//store new vertices values for wireframe drawing
		nodes_rotated[l_Node][XCOORD] = rotxxx;
		nodes_rotated[l_Node][YCOORD] = rotyyy;
		nodes_rotated[l_Node][ZCOORD] = rotzzz;
	}
}
