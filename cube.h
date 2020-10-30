/*
 * cube.h
 *
 * Created: 20/10/2020 12:35:22
 *  Author: Petervg
 */ 


#ifndef CUBE_H_
#define CUBE_H_

float SIN(uint16_t angle);
float COS(uint16_t angle);

void ResizeCube(uint8_t l_CubeSize);
void DrawCubeRotated(struct _SSD1306FrameBuffer* l_FrameBuffer, uint8_t l_Xoffset, uint8_t l_Yoffset, uint16_t l_AngleX, uint16_t l_AngleY, uint16_t l_AngleZ);
void RotateCube(uint16_t l_AngleX, uint16_t l_AngleY, uint16_t l_AngleZ);

#endif /* CUBE_H_ */