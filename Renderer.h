
#include <Windows.h>
#include <D3D11.h> 
#include <D3D11Shader.h> 


#include <D3DX10math.h>
#include <d3d9types.h>

#include <D3DX11async.h>

#include "imgui.h"
#include "imgui_internal.h"

void RenderText(int x, int y, DWORD color/*ARGB*/, char *szText, ...);
void _RenderText(int x, int y, DWORD color/*ARGB*/, float Size, UINT Flags, char *szText, ...);
void FillARGB(const ImVec2& pos_a, const ImVec2& pos_b, DWORD color, float rounding/*=0.0f*/, int rounding_corners /*=-1*/);			// void FillARGB(int x, int y, int w, int h, DWORD color/*ARGB*/);
void DrawLine(const ImVec2& from, const ImVec2& to, DWORD color/*ARGB*/);																				// void DrawLine(float x, float y, float x1, float y1, DWORD color/*ARGB*/);
void DrawCircle(const ImVec2& centre, float radius, DWORD color/*ARGB*/);				
float DrawText2(const ImVec2& pos, float size, DWORD color, bool center, char* stext, ...);// void DrawCircle(int x, int y, int radius, DWORD color/*ARGB*/);
void DrawBorder(const ImVec2& pos_a, const ImVec2& pos_b, DWORD color, float rounding/*=0.0f*/, int rounding_corners /*=-1*/, float thickness);			// void DrawBorder(int x, int y, int w, int h, int px, DWORD BorderColor);
void DrawBox(int x, int y, int w, int h, DWORD BoxColor, DWORD BorderColor);
void DrawHealthBox(int x, int y, DWORD m_dColorOut, DWORD m_dColorIn, int m_iHealth, int m_iMaxHealth);
void DrawRadar(int x, int y, int size);
void DrawHealthBox2(int x, int y, DWORD m_dColorOut, DWORD m_dColorIn, int m_iHealth, int m_iMaxHealth);
void DrawPoint(int x, int y, DWORD color);
void DrawPixel(int x, int y, DWORD color);
inline DWORD D3DCOLOR_ABGR(int a, int r, int g, int b);

#define CIRCLE_NUMPOINTS 30 
#define MAX_VERTEX_COUNT ( CIRCLE_NUMPOINTS + 1 ) 

#ifndef DX11_RENDERER_H 
#define DX11_RENDERER_H 

const DWORD txtRed            = D3DCOLOR_ARGB( 255,    255,    0,        0    ); 
const DWORD txtGreen        = D3DCOLOR_ARGB( 255,    0,        255,    0    ); 
const DWORD txtBlue            = D3DCOLOR_ARGB( 255,    0,        0,        255    ); 
const DWORD txtYellow        = D3DCOLOR_ARGB( 255,    255,    255,    0    ); 
const DWORD txtOrange        = D3DCOLOR_ARGB( 255,    255,    100,    0    ); 
const DWORD txtWhite        = D3DCOLOR_ARGB( 255,    255,    255,    255 ); 
const DWORD txtBlack        = D3DCOLOR_ARGB( 255,    0,        0,        0    ); 
const DWORD txtGrey            = D3DCOLOR_ARGB( 255,    86,        86,        86    ); 
const DWORD SpecialBlue        = D3DCOLOR_ARGB( 255,    51,        102,    255 ); 
const DWORD txtPink            = D3DCOLOR_ARGB( 255,    255,    0,        255    ); 
const DWORD txtTur            = D3DCOLOR_ARGB( 255,    150,    0,        255    ); 

const DWORD EspGreen        = D3DCOLOR_ARGB( 255,    15,        200,    15    ); 
const DWORD EspGrey            = D3DCOLOR_ARGB( 255,    80,        80,        80    ); 

#endif 

//(c) Apflmus