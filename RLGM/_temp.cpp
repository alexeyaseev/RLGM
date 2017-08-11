#include "stdafx.h"

/*D3D11_BLEND_DESC BlendState;
ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
BlendState.RenderTarget[0].BlendEnable = FALSE;
BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
BlendState.RenderTarget[1].BlendEnable = FALSE;
BlendState.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

D3D11_BLEND_DESC oBlendStateDesc;

oBlendStateDesc.AlphaToCoverageEnable = 0;
oBlendStateDesc.IndependentBlendEnable = 0; //set to false, dont need loop below... but just incase

for (unsigned int a = 0; a < 8; ++a)
{
oBlendStateDesc.RenderTarget[a].BlendEnable = 0;
oBlendStateDesc.RenderTarget[a].SrcBlend = D3D11_BLEND_SRC_ALPHA;
oBlendStateDesc.RenderTarget[a].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
oBlendStateDesc.RenderTarget[a].BlendOp = D3D11_BLEND_OP_ADD;
oBlendStateDesc.RenderTarget[a].SrcBlendAlpha = D3D11_BLEND_ONE;
oBlendStateDesc.RenderTarget[a].DestBlendAlpha = D3D11_BLEND_ZERO;
oBlendStateDesc.RenderTarget[a].BlendOpAlpha = D3D11_BLEND_OP_ADD;
oBlendStateDesc.RenderTarget[a].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

hr = owner->g_pd3dDevice->CreateBlendState(&oBlendStateDesc, &g_pBlendStateNoBlend);
if (FAILED(hr)) return;// hr;

// Stencil test parameters
D3D11_DEPTH_STENCIL_DESC dsDesc;

// Depth test parameters
dsDesc.DepthEnable = false;
dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
dsDesc.StencilEnable = false;
dsDesc.StencilReadMask = 0xFF;
dsDesc.StencilWriteMask = 0xFF;

// Stencil operations if pixel is front-facing
dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

// Stencil operations if pixel is back-facing
dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

// Create depth stencil state
hr = owner->g_pd3dDevice->CreateDepthStencilState(&dsDesc, &g_pDepthStencilState);
if (FAILED(hr)) return;// hr;*/



/*
//создание квада поверхностей
quadSurfaces[0] = new TransformedTexturedVertex(new Vector4(0 - 0.5f, 0 - 0.5f, 1, 1), new Vector2(0, 0));
quadSurfaces[1] = new TransformedTexturedVertex(new Vector4(width - 0.5f, 0 - 0.5f, 1, 1), new Vector2(1, 0));
quadSurfaces[2] = new TransformedTexturedVertex(new Vector4(0 - 0.5f, height - 0.5f, 1, 1), new Vector2(0, 1));
quadSurfaces[3] = quadSurfaces[1];
quadSurfaces[4] = quadSurfaces[2];
quadSurfaces[5] = new TransformedTexturedVertex(new Vector4(width - 0.5f, height - 0.5f, 1, 1), new Vector2(1, 1));
vbQuadSurfaces = new VertexBuffer(owner.device, TransformedTexturedVertex.SizeInBytes * 6, Usage.Dynamic | Usage.WriteOnly, VertexFormat.None, Pool.Default);
stream = vbQuadSurfaces.Lock(0, 0, LockFlags.Discard);
stream.WriteRange(quadSurfaces);
vbQuadSurfaces.Unlock();

//формирование слоя КАРТА и верхнего слоя
layerMap = new Texture[2];
for (int i = 0; i < layerMap.Length; i++)
layerMap[i] = new Texture(owner.device, width, height, 1, Usage.None, Format.X8R8G8B8, Pool.Managed);

layerTop1 = new Texture[2];
for (int i = 0; i < layerTop1.Length; i++)
layerTop1[i] = new Texture(owner.device, width, height, 1, Usage.None, Format.X8R8G8B8, Pool.Managed);

layerTop2 = new Texture[2];
for (int i = 0; i < layerTop2.Length; i++)
layerTop2[i] = new Texture(owner.device, width, height, 1, Usage.None, Format.X8R8G8B8, Pool.Managed);*/