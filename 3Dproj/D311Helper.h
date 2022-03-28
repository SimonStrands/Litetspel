#pragma once
#include <Windows.h>
#include <iostream>
#include <d3d11.h>
//git
//static DepthStencil depth;

bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsview);
void SetViewport(D3D11_VIEWPORT& viewPort, UINT width, UINT height);
bool CreateInsterface(UINT width, UINT height, HWND wnd, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain);
bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& renderTargetView);

bool SetupD3D11(UINT width,
	UINT height,
	HWND wnd,
	ID3D11Device*& device,
	ID3D11DeviceContext*& immediateContext,
	IDXGISwapChain*& swapChain,
	ID3D11RenderTargetView*& renderTarget,
	ID3D11Texture2D*& dsTexture,
	ID3D11DepthStencilView*& dsView,
	D3D11_VIEWPORT& viewPort,
	ID3D11RasterizerState*& pRS);
