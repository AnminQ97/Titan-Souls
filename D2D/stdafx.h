#pragma once

//Window
#include <Windows.h>
#include <assert.h>

//STL
#include <vector>
#include <string>
#include <bitset>
#include <map>
#include <functional>
#include <mutex>
#include<algorithm>
using namespace std;

//DX Lib
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <D3DX10math.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "d3dcompiler.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <FMOD/fmod.hpp>
#pragma comment(lib,"FMOD/fmodex_vc.lib")


//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

//TypeDef
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXCOLOR Color;
typedef D3DXMATRIX Matrix;
typedef D3DXQUATERNION Quaternion;

//Framework
#include "Systems/Time.h"
#include "Systems/Keyboard.h"
#include "Systems/Mouse.h"
#include "Systems/DirectWrite.h"
#include "Systems/SoundTool.h"

#include "Renders/Shader.h"
#include "Renders/Sprite.h"
#include "Renders/Clip.h"
#include "Renders/Animation.h"

#include "Collider/Collider.h"
#include "Collider/LineCollider.h"


#include "Utilities/String.h"
#include "Utilities/Math.h"
#include "Utilities/Path.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

#include "Viewer/Camera.h"
#include "Viewer/Following.h"
#include "Viewer/Freedom.h"
#include "Viewer/RenderTarget.h"

//Macro
#define SAFE_DELETE(p) { if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) { if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p) { if(p) {(p)->Release(); (p) = NULL;}}

#define GENERATE_MOUSE_UNPROJECT(out)\
out = Mouse->Position();\
out.x = ((out.x / Width) * 2.0f) - 1.0f;\
out.y = (((out.y / Height) * 2.0f) - 1.0f) * -1.0f;\
\
Matrix world;\
D3DXMatrixIdentity(&world);\
Matrix wvp = world * V * P;\
\
D3DXMatrixInverse(&wvp, NULL, &wvp);\
D3DXVec2TransformCoord(&mouse, &mouse, &wvp);\

//Global
extern UINT Width;
extern UINT Height;

extern HWND Hwnd;
extern wstring Title;

//DX Interface
extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;

extern Keyboard* Key;
extern CMouse* Mouse;
extern SoundTool* Audio;
