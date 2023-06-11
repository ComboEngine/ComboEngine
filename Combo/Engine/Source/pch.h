#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/matrix_inverse.hpp>
#include <detail/type_quat.hpp>
#include <gtx/quaternion.hpp>

//Windows header
#include <Windows.h>
#include <WindowsX.h>

//DirectX headers
#include <d3d11.h>
#include <xnamath.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <D2D1.h>
#include <D3Dcompiler.h>
#include <dinput.h>


//Include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")