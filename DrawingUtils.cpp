


#include "imgui.h"
#include "imgui_impl_dx11.h"
#include <Windows.h>
#include "imgui_internal.h"
// DirectX
#include <d3d11.h>

#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <dwmapi.h>
#include <iostream>
#include <D3DX10math.h>
#include <string>
#include <sstream>

#include "Font.h"
#include "Globals.h"
#include "Classes.h"
#include "include\FW1FontWrapper.h"
#include "tab.h"
#include "FullBright.h"
#include "Utils.h"
extern Entity entity[500];

static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceImmediateContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;
ID3D11Device *device = 0;
ImFont* m_pFont;
const char* tabLabels[] = { "Visuals", " Config ", " Aimbot ", "  Misc  " };
static int tabOrder[] = { 0,1,2,3 };
static int tabSelected = 0;
D3DXMATRIX viewMatrix;
int Height = GetSystemMetrics(SM_CYSCREEN);
int Width = GetSystemMetrics(SM_CXSCREEN);
extern Globals::Values values;
bool alwaysDay = false;
bool windowstate;
extern DWORD64 lookingAt;
void DrawRocks();
void DrawEnts();
void doRecoil();
bool isLocalPlayer(Entity player);

DWORD D3DCOLOR_ABGR(int a, int r, int g, int b)
{
	return (((a) & 0xff) << 24) | (((b) & 0xff) << 16) | (((g) & 0xff) << 8) | ((r) & 0xff);
}



ImVec4 tempColor;
void CreateRenderTarget()
{
	DXGI_SWAP_CHAIN_DESC sd;
	g_pSwapChain->GetDesc(&sd);
	ID3D11Texture2D* pBackBuffer;
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
	render_target_view_desc.Format = sd.BufferDesc.Format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &g_mainRenderTargetView);
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	pBackBuffer->Release();

}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

HRESULT CreateDeviceD3D(HWND hWnd)
{
	
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return E_FAIL;

	CreateRenderTarget();

	return S_OK;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	
	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX11_InvalidateDeviceObjects();
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
			ImGui_ImplDX11_CreateDeviceObjects();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) 
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}
		if (ImGui_ImplDX11_WndProcHandler(hWnd, msg, wParam, lParam) && values.menuEnabled)
			return true;
		
		
		return DefWindowProc(hWnd, msg, wParam, lParam);

		
}


void setStyle()
{
	ImGuiStyle * style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 1.5f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 3.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 12.0f;
	style->GrabRounding = 3.0f;
	style->WindowTitleAlign = ImVec2(0.5, 0.5);

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}


void BeginScene()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("BackBuffer", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);
}



void EndScene()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
}

bool WorldToScreen(D3DXVECTOR3 origin, D3DXVECTOR2 * out)
{
	D3DXMATRIX temp;

	D3DXMatrixTranspose(&temp, &viewMatrix);

	D3DXVECTOR3 translationVector = D3DXVECTOR3(temp._41, temp._42, temp._43);
	D3DXVECTOR3 up = D3DXVECTOR3(temp._21, temp._22, temp._23);
	D3DXVECTOR3 right = D3DXVECTOR3(temp._11, temp._12, temp._13);

	float w = D3DXVec3Dot(&translationVector, &origin) + temp._44;

	if (w < 0.098f)
		return false;

	float y = D3DXVec3Dot(&up, &origin) + temp._24;
	float x = D3DXVec3Dot(&right, &origin) + temp._14;

	out->x = (int)(Width / 2) * (1.f + x / w);
	out->y = (int)(Height / 2) * (1.f - y / w);

	return true;
}
int build = 8;
void DrawMenu()
{
	
	if (values.menuEnabled) {

		ImGui::Begin("Rust Cheat Beta Build (c) Apflmus: " + build, &values.menuEnabled, ImVec2(300, 500), 0.75f);
		ImGui::TabLabels(tabLabels, sizeof(tabLabels) / sizeof(tabLabels[0]), tabSelected, tabOrder);
		ImGui::BeginChild("0", ImVec2(ImGui::GetWindowWidth() - 15, ImGui::GetWindowHeight() - 60), true, 0);
		if (tabSelected == 0) {
			ImGui::Checkbox("Player Names", &values.playerESP);
			ImGui::Checkbox("Player Boxes", &values.playerBox);
			ImGui::Checkbox("Player Health Bars", &values.healthBars);
			ImGui::Checkbox("Player Active Item", &values.activeItemESP);
			ImGui::Checkbox("Sleepers", &values.sleepersESP);
			ImGui::Checkbox("Animals", &values.animalESP);
			ImGui::Checkbox("Corpse", &values.corpseESP);
			ImGui::Checkbox("Tool Cupboard", &values.tcESP);
			ImGui::Checkbox("Stashes", &values.stashESP);
			ImGui::Checkbox("Sulfur Nodes", &values.sulfurnodeESP);
			ImGui::Checkbox("Stone Nodes", &values.stonenodeESP);
			ImGui::Checkbox("Metal Nodes", &values.metalnodeESP);
			ImGui::Checkbox("Small Chests", &values.smallChestESP);
			ImGui::Checkbox("Large Chests", &values.largeChestESP);
			ImGui::Checkbox("Drops", &values.dropESP);
			ImGui::Checkbox("Food", &values.foodESP);
			ImGui::Checkbox("Loot", &values.lootESP);
			ImGui::Checkbox("Helicopter", &values.helicopterESP);
			ImGui::Checkbox("Sleeping Bags", &values.sleepingBagsESP);
			ImGui::Checkbox("Collectables", &values.collectablesESP);
		}
		else if (tabSelected == 1) {
			ImGui::SliderInt("ESP Font Size", &values.fontSize, 8, 36);
			ImGui::SliderInt("Player ESP Maximum Range", &values.espDistance, 25, 450);
			ImGui::SliderInt("Other ESP Maximum Range", &values.espOtherDistance, 25, 2500);
			ImGui::Checkbox("ESP Distance Color", &values.espScalingColor);
			if (ImGui::TreeNode("Color Config")) {
				if (ImGui::Button("Crosshair Color"))
					ImGui::OpenPopup("Crosshair Color");
				if (ImGui::Button("Close Player Color"))
					ImGui::OpenPopup("Close Player Color");
				else if (ImGui::Button("Far Player Color"))
					ImGui::OpenPopup("Far Player Color");
				else if (ImGui::Button("Drop Color"))
					ImGui::OpenPopup("Drop Color");
				else if (ImGui::Button("Food Color"))
					ImGui::OpenPopup("Food Color");
				else if (ImGui::Button("Corpse Color"))
					ImGui::OpenPopup("Corpse Color");
				else if (ImGui::Button("Player Box Color"))
					ImGui::OpenPopup("Player Box Color");
				else if (ImGui::Button("Sleeper Player Color"))
					ImGui::OpenPopup("Sleeper Color");
				else if (ImGui::Button("Animal Color"))
					ImGui::OpenPopup("Animal Color");
				else if (ImGui::Button("Sulfur Node Color"))
					ImGui::OpenPopup("Sulfur Node Color");
				else if (ImGui::Button("Stone Node Color"))
					ImGui::OpenPopup("Stone Node Color");
				else if (ImGui::Button("Metal Node Color"))
					ImGui::OpenPopup("Metal Node Color");
				else if (ImGui::Button("TC Color"))
					ImGui::OpenPopup("TC Color");
				else if (ImGui::Button("Helicopter Color"))
					ImGui::OpenPopup("Helicopter Color");
				else if (ImGui::Button("Stash Color"))
					ImGui::OpenPopup("Stash Color");
				else if (ImGui::Button("Chest Color"))
					ImGui::OpenPopup("Chest Color");
				else if (ImGui::Button("Hemp Color"))
					ImGui::OpenPopup("Hemp Color");
				else if (ImGui::Button("Stone Collectable Color"))
					ImGui::OpenPopup("Stone Collectable Color");
				else if (ImGui::Button("Metal Collectable Color"))
					ImGui::OpenPopup("Metal Collectable Color");
				else if (ImGui::Button("Log Collectable Color"))
					ImGui::OpenPopup("Log Collectable Color");
				else if (ImGui::Button("Sleeping Bag Color"))
					ImGui::OpenPopup("Sleeping Bag Color");
				else if (ImGui::Button("Barrel Color"))
					ImGui::OpenPopup("Barrel Color");
				else if (ImGui::Button("Trash Color"))
					ImGui::OpenPopup("Trash Color");
				else if (ImGui::Button("Airdrop Color"))
					ImGui::OpenPopup("Airdrop Color");


				if (ImGui::BeginPopupModal("Crosshair Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.crosshairColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.crosshairColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("Food Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.foodColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.foodColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}


				if (ImGui::BeginPopupModal("Close Player Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.playerColorClose);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.playerColorClose = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("Far Player Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.playerColorFar);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.playerColorFar = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Corpse Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.corpseColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.corpseColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Drop Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.dropColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.dropColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("Sleeper Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.sleepersColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.sleepersColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Animal Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.animalColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.animalColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Hemp Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.hempColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.hempColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Sulfur Node Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.sulfurNodeColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.sulfurNodeColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Stone Node Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.stoneNodeColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.stoneNodeColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Metal Node Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.metalNodeColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.metalNodeColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Stash Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.stashColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.stashColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Player Box Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.playerBoxColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.playerBoxColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("TC Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.tcColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.tcColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Helicopter Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.helicopterColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.helicopterColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Chest Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.chestColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.chestColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Log Collectable Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.logColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.logColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Metal Collectable Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.metalColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.metalColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Stone Collectable Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.stoneColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.stoneColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Sulfur Collectable Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.sulfurColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.sulfurColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Sleeping Bag Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.sleepingBagsColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.sleepingBagsColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Barrel Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.barrelColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.barrelColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Trash Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.trashColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.trashColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Airdrop Color", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					tempColor = ImGui::ColorConvertU32ToFloat4(values.airDropColor);
					ImGui::ColorEdit3("RGB", (float*)&(tempColor));
					values.airDropColor = ImGui::ColorConvertFloat4ToU32(tempColor);

					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
			}
		}
		if (tabSelected == 2) {
			ImGui::SliderInt("Aimbot FOV", &values.aimbotFOV, 1, 250);
			
			static int selected = 0;






		}
		if (tabSelected == 3) {
			//ImGui::Checkbox("Radar", &values.radar);
			ImGui::Checkbox("Crosshair", &values.crosshair);
			ImGui::Checkbox("NoFall", &values.noFall);
			ImGui::Checkbox("FreeCam", &values.freeCam);
			ImGui::Checkbox("Speed Key (C)", &values.speedHack);
			ImGui::Checkbox("NoGrass", &values.noGrass);
			ImGui::Checkbox("Fast Gather", &values.fastGather);
			ImGui::Checkbox("No Water Collision", &values.noWater);
			ImGui::Checkbox("Force Weapons Automatic", &values.forceAutomatic);
			ImGui::Checkbox("No Aim Sway", &values.swayEnabled);
			ImGui::Checkbox("No Bullet Spread", &values.spreadEnabled);
			ImGui::Checkbox("Always Day", &values.nightvision);
			if (ImGui::TreeNode("Recoil Configuration")) {
				ImGui::Checkbox("Recoil Modification", &values.recoilEnabled);
				ImGui::SliderInt("Weapon Recoil Percentage", &values.recoilMultiplier, 0, 100);
			}
			

		}
		ImGui::EndChild();
		ImGui::End();

	}


}

void ChangeClickability(bool canclick, HWND ownd)
{
	long style = GetWindowLong(
		ownd, GWL_EXSTYLE);
	if (canclick) {
		style &= ~WS_EX_LAYERED;
		SetWindowLong(ownd,
			GWL_EXSTYLE, style);
		SetForegroundWindow(ownd);
		windowstate = 1;
	}
	else {
		style |= WS_EX_LAYERED;
		SetWindowLong(ownd,
			GWL_EXSTYLE, style);
		windowstate = 0;
	}
}
void radar() {


	int xOffset = (Width - 305); //Global X offset for the radar, can be set to a public int to be moveable.
	int yOffset = (Height - 775);
	//RenderingUtil.rectangleBordered(xOffset, yOffset, xOffset + 100, yOffset + 100, 1.5, Colors.getColor(0, 0, 0, 150), Colors.getColor(200, 35, 35));
	FillARGB(ImVec2(Width-303, Height-305), ImVec2(Width, Height), D3DCOLOR_ABGR(200,0,0,0), 0, 0);

	for (int x = 0; x <= values.numberOfTaggedObjects; x++) { //All entities
		if (entity[x].tag == 6) { //Instance of Player
										//Player defined here
			int state = entity[x].state & 8;
			if (state != 8 && !isLocalPlayer(entity[x])) {
		
				float posX = (float)((entity[x].position.x) - entity[1].position.x) * 1;

				float posZ = (float)(((entity[x].position.z) - entity[1].position.z) * 1);


				float cosx = (float)cos(entity[2].bodyAngles.x * (3.141592654 * 2 / 360));
				float sinx = (float)sin(entity[2].bodyAngles.y * (3.141592654 * 2 / 360));
				float rotY = -(posZ * cosx + posX * sinx);
				float rotX = (posX * cosx - posZ * sinx);

				if (rotY > 150) {
					rotY = 150.0f;
				}
				else if (rotY < -150) {
					rotY = -150.0f;
				}
				if (rotX >  150) {
					rotX = 150.0f;
				}
				else if (rotX < -150) {
					rotX = -150.0f;
				}

				if (entity[x].distance < 500) {
					//FillRGB(xOffset + 152.5 + rotX, yOffset - 152.5 + rotY, 4, 4, 255, 0, 0, 255);
				}
			}
		}
	}

	FillARGB(ImVec2(xOffset + 152.5, yOffset - 152.5), ImVec2(xOffset + 156.5, yOffset - 156.5), D3DCOLOR_ABGR(255,0,0,255), 0, 0);

}
void crosshair() {

	int CenterX = Width / 2;
	int CenterY = Height / 2;
	//FillARGB(ImVec2(CenterX - 11, CenterY), ImVec2(CenterX, CenterY - 11),values.crosshair, 0, 0);//Green
	DrawLine(ImVec2(CenterX, CenterY - 11), ImVec2(CenterX, CenterY + 11), values.crosshairColor);
	DrawLine(ImVec2(CenterX-11, CenterY), ImVec2(CenterX+11, CenterY), values.crosshairColor);														  //FillARGB(CenterX, CenterY - 11, 1, 22, 0, 255, 0, 255);

}

void initializeImguiAndWindow()
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), ACS_TRANSPARENT, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, "ZX", NULL };
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, L"ZX", L"ZX", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, 0, NULL);
	MARGINS margins = { -1 };
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA | LWA_COLORKEY);
	DwmExtendFrameIntoClientArea(hwnd, &margins);
	

	
	// Initialize Direct3D
	if (CreateDeviceD3D(hwnd) < 0)
	{
		CleanupDeviceD3D();
		UnregisterClassA("ZX", wc.hInstance);
		return;
	}
	// Show the window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	// Setup ImGui binding
	ImGui_ImplDX11_Init(hwnd, g_pd3dDevice, g_pd3dDeviceContext);

	ImGuiIO& io = ImGui::GetIO();
	//m_pFont = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	m_pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ARIALUNI.TTF", 18.0f, NULL, io.Fonts->GetGlyphRangesAll());
	if (m_pFont == 0)
	{
		std::cout << "\nFailed to load font! Attempt to install? (1 for yes, 0 for no)";
		bool choice;
		std::cin >> choice;
		if (choice)
		{
			if (!URLDownloadToFileA(NULL, "https://raw.githubusercontent.com/bloomberg/scatteract/master/fonts/ARIALUNI.TTF", "C:\\Windows\\Fonts\\ARIALUNI.TTF", 0, NULL)) {
				std::cout << "\nFailed to install font!";
				Sleep(3000);
				exit(0);
			}
			else {
				std::cout << "\nFont installed. Continuing... ";
				m_pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ARIALUNI.TTF", 18.0f, NULL, io.Fonts->GetGlyphRangesAll());
			}
				
		}
		else
		{
			Sleep(3000);
			exit(0);
		}
		
	}
	setStyle();
	MSG msg;
	float color[4] = { 0,0,0,0 };
	ZeroMemory(&msg, sizeof(msg));
	values.menuEnabled = 0;
	char coords[50];

	while (true)
	{
		Sleep(1);
		if (PeekMessage(&msg, hwnd, 0U, 0U, PM_REMOVE))
		{
			DispatchMessage(&msg);
			TranslateMessage(&msg);
			continue;
		}

		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, color);

		ImGui_ImplDX11_NewFrame();

		doRecoil();
		
	
		if (values.nightvision)
		{
			setToDay();
		}
		if (values.menuEnabled)
		{
			if (windowstate != 1) //You might need to remove this check and call it every time to get keyboard input working. Mouse input works anyway.
			{
				ChangeClickability(true, hwnd);
			}
			DrawMenu();
		}
		else
		{
			if (windowstate != 0)
			{
				ChangeClickability(false, hwnd);
			}
		}

		if (values.inGame) {

			BeginScene();
			sprintf(coords, "Pos: X: %d Y: %d Z: %d", (int)entity[0].position.x, (int)entity[0].position.y, (int)entity[0].position.z);

			DrawText2(ImVec2(6, 5), 18, D3DCOLOR_ABGR(255, 0, 255, 0), false, coords);
			if (values.crosshair)
				crosshair();
			//if (values.radar)
			//	radar();
			if (values.menuEnabled)
				if (tabSelected == 2)
					DrawCircle(ImVec2(Width / 2, Height / 2), values.aimbotFOV, D3DCOLOR_ARGB(255, 255, 0, 0));
			DrawEnts();
			DrawRocks();
			EndScene();
		}
	
		ImGui::Render();
		g_pSwapChain->Present(0, 0);
	}

	ImGui_ImplDX11_Shutdown();
	CleanupDeviceD3D();
	UnregisterClassA("ZX", wc.hInstance);

	return;
}












//(c) Apflmus