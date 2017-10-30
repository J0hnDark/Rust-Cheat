#include <stdio.h> 
#include <string> 
#include <sstream> 

#include <iostream> 
#include "Renderer.h"

#pragma comment( lib, "DXErr" ) 
#pragma comment (lib, "D3D11.lib") 
#pragma comment (lib, "D3DX11.lib") 

#pragma comment (lib, "d3dcompiler.lib") 
//#pragma comment (lib, "Effects11.lib") 

#pragma warning ( disable: 4102 ) 
#pragma warning ( disable: 4311 ) 
#pragma warning ( disable: 4312 ) 
#pragma warning ( disable: 4996 )
#pragma warning ( disable: 4005 ) 
#pragma warning ( disable: 4244 ) 
#pragma warning ( disable: 4996 ) 

extern ImFont* m_pFont;
struct COLOR_VERTEX
{
	D3DXVECTOR3 Position;
	D3DXCOLOR    Color;
};

inline DWORD D3DCOLOR_ABGR(int a, int r, int g, int b)
{
	return (((a)& 0xff) << 24) | (((b)& 0xff) << 16) | (((g)& 0xff) << 8) | ((r)& 0xff);
}

static std::string asciiEncode( const std::wstring & w ); 
static std::wstring asciiDecode( const std::string & s ); 

static std::string asciiEncode( const std::wstring & w ) 
{ 
    std::ostringstream  s; 
    char *              c; 
    int                    r; 

    c = new char[ MB_CUR_MAX + 1 ]; 
    for( size_t i = 0; i < w.length(); i++ ) { 
        r = wctomb( c, w[i] ); 
        c[r] = '\0'; 
        if( r <= 1 && c[0] > 0 ) { 
            s << c; 
        } 
    } 
    return s.str(); 
} 

static std::wstring asciiDecode( const std::string & s ) 
{ 
    std::wostringstream    w; 
    wchar_t                c; 

    for( size_t i = 0; i < s.length(); i++ ) { 
        mbtowc( &c, &s[i], 1 ); 
        w << c; 
    } 
    return w.str(); 
} 

float DrawText2(const ImVec2& pos, float size, DWORD color, bool center, char* stext, ...)
{
	va_list argptr;
	va_start(argptr, stext);

	char Text[256];
	vsprintf(Text, stext, argptr);

	va_end(argptr);

	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float b = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float r = (color) & 0xff;

	std::stringstream stream(Text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = m_pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			window->DrawList->AddText(m_pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(m_pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(m_pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(m_pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());

			window->DrawList->AddText(m_pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}
		else
		{
			window->DrawList->AddText(m_pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(m_pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(m_pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(m_pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());

			window->DrawList->AddText(m_pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void FillARGB(const ImVec2& pos_a, const ImVec2& pos_b, DWORD color, float rounding/*=0.0f*/, int rounding_corners /*=-1*/)
{ 
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float b = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float r = (color) & 0xff;

	window->DrawList->AddRectFilled(pos_a, { pos_a.x + pos_b.x, pos_a.y + pos_b.y }, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 0.0f, rounding_corners);
} 

void DrawLine(const ImVec2& from, const ImVec2& to,  DWORD color/*ARGB*/ ) 
{ 
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float b = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float r = (color) & 0xff;

	window->DrawList->AddLine(from, to, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 1.0f);
} 

void DrawCircle( const ImVec2& centre, float radius, DWORD color/*ARGB*/ ) 
{ 
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircle(centre, radius, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 32);
} 

void DrawBorder(const ImVec2& pos_a, const ImVec2& pos_b, DWORD color, float rounding/*=0.0f*/, int rounding_corners /*=-1*/, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	//window->DrawList->AddQuad(pos_a, pos_b, )
	window->DrawList->AddRect(pos_a, { pos_a.x + pos_b.x, pos_a.y + pos_b.y }, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 0.0f, rounding_corners, thickness);
}

void DrawBox( int x, int y, int w, int h, DWORD BoxColor, DWORD BorderColor ) 
{ 
	
} 
void DrawHealthBox2(int x, int y, DWORD m_dColorOut, DWORD m_dColorIn, int m_iHealth, int m_iMaxHealth)
{
	if (m_iHealth == 0)
		m_iHealth = 1;
	float mx = (float)m_iMaxHealth / 36;
	float w = (float)m_iHealth / 36;
	x -= ((int)mx / 2);

	FillARGB(ImVec2(x, y), ImVec2(mx, 4), m_dColorOut, 0.0f, -1);
	FillARGB(ImVec2(x, y), ImVec2(w, 4), m_dColorIn, 0.0f, -1);

	DrawBorder(ImVec2(x - 1, y - 1), ImVec2(mx + 2, 6), D3DCOLOR_ABGR(255, 30, 30, 30), 0.0f, -1, 1.5);

}

void DrawHealthBox( int x, int y, DWORD m_dColorOut, DWORD m_dColorIn, int m_iHealth, int m_iMaxHealth ) 
{ 
	if (m_iHealth == 0)
		m_iHealth = 1;
	float mx = (float)m_iMaxHealth / 2;
	float w = (float)m_iHealth / 2;
	x -= ((int)mx / 2);

	FillARGB(ImVec2(x, y), ImVec2(mx, 4), m_dColorOut, 0.0f, -1);
	FillARGB(ImVec2(x, y), ImVec2(w, 4), m_dColorIn, 0.0f, -1);

	DrawBorder(ImVec2(x - 1, y - 1), ImVec2(mx + 2, 6), D3DCOLOR_ABGR(255, 30, 30, 30), 0.0f, -1, 1.5);

} 

void DrawRadar( int x, int y, int size ) 
{ 
	//TODO
} 

void DrawPoint( int x, int y, DWORD color ) 
{ 
	//TODO
} 

void DrawPixel( int x, int y, DWORD color ) 
{ 
	//TODO
} 