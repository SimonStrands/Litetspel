#include "D311Helper.h"
#include <DirectXMath.h>

bool CreateInsterface(UINT width, UINT height, HWND wnd, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain)
{
    //start createing description for swapchain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //def
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = wnd;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    UINT flags = 0;
    #ifdef _DEBUG
        flags = D3D11_CREATE_DEVICE_DEBUG;
    #endif
    D3D_FEATURE_LEVEL featureLvl[] = { D3D_FEATURE_LEVEL_11_0 };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLvl, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &immediateContext);
    
    return !FAILED(hr);
}

bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& renderTargetView)
{
    ID3D11Texture2D* backBuffer = nullptr;
    if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
    {
        printf("failed to swapchain");
        return false;
    }

    HRESULT hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();
    return !FAILED(hr);
}

bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsview)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
    {
        printf("failed create 2d texture");
        return false;
    }

    HRESULT hr = device->CreateDepthStencilView(dsTexture, nullptr, &dsview);
    return !FAILED(hr);
}

bool CreateRasterizerState(ID3D11Device* device , ID3D11RasterizerState*& pRS) {
    D3D11_RASTERIZER_DESC rasterState;
    rasterState.FillMode = D3D11_FILL_SOLID;
    //rasterState.FillMode = D3D11_FILL_WIREFRAME;
    //rasterState.CullMode = D3D11_CULL_NONE;
    rasterState.CullMode = D3D11_CULL_BACK;
    rasterState.FrontCounterClockwise = false;
    rasterState.DepthBias = 0;
    rasterState.SlopeScaledDepthBias = 0;
    rasterState.DepthBiasClamp = 0;
    rasterState.DepthClipEnable = true;
    rasterState.ScissorEnable = false;
    rasterState.MultisampleEnable = false;
    rasterState.AntialiasedLineEnable = false;

   HRESULT hr = device->CreateRasterizerState(&rasterState, &pRS);
   if (FAILED(hr)) {
       return false;
   }

   return !FAILED(hr);
}

void SetViewport(D3D11_VIEWPORT& viewPort, UINT width, UINT height)
{
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    viewPort.Width = static_cast<float>(width);
    viewPort.Height = static_cast<float>(height);
    viewPort.MinDepth = 0;
    viewPort.MaxDepth = 1;
}

bool SetupD3D11(UINT width, UINT height, 
    HWND wnd, ID3D11Device*& device, 
    ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, 
    ID3D11RenderTargetView*& renderTarget, ID3D11Texture2D*& dsTexture, 
    ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewPort, 
    ID3D11RasterizerState*& pRS
)
{
    if (!CreateInsterface(width, height, wnd, device, immediateContext, swapChain))
    {
        std::cerr << "create interface" << std::endl;
        return false;
    }
    
    if (!CreateRenderTargetView(device, swapChain, renderTarget))
    {
        std::cerr << "create rendertarget view" << std::endl;
        return false;
    }
    
    if (!CreateDepthStencil(device, width, height, dsTexture, dsView))
    {
        std::cerr << "create depth" << std::endl;
        return false;
    }
    if (!CreateRasterizerState(device, pRS))
    {
        std::cerr << "create rasterizer" << std::endl;
        return false;
    }
    
    SetViewport(viewPort, width, height);

    return true;
}