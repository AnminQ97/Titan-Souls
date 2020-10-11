#include"stdafx.h"
#include"RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height, DXGI_FORMAT format)
{
	this->width = (width < 1) ? Width : width;
	this->height = (height < 1) ? Height : height;

	//2D텍스쳐 만들기
	//DESC
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = this->width; 
	textureDesc.Height = this->height;
	textureDesc.ArraySize = 1; //texture2D배열
	textureDesc.Format = format;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;//사용 용도 보내기 띄우기
	textureDesc.MipLevels = 1; //멀어질수록 픽셀 떨구기  
	textureDesc.SampleDesc.Count = 1;
	assert(SUCCEEDED(Device->CreateTexture2D(&textureDesc, NULL, &backBuffer)));

	//RTV
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	assert(SUCCEEDED(Device->CreateRenderTargetView(backBuffer, &rtvDesc, &rtv)));
	
	//SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;//차원
	srvDesc.Texture2D.MipLevels = 1;				  //리소스	  //Desc	//받을곳
	assert(SUCCEEDED(Device->CreateShaderResourceView(backBuffer, &srvDesc, &srv)));

}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(backBuffer);
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(srv);
}

void RenderTarget::SaveTexture(wstring file)//스샷
{
	HRESULT hr =D3DX11SaveTextureToFile(DeviceContext, backBuffer, D3DX11_IFF_PNG,file.c_str());
	assert(SUCCEEDED(hr));
}

void RenderTarget::Set() //om stage세팅
{
	DeviceContext->OMSetRenderTargets(1, &rtv, NULL); //백버퍼 화면 전면에 내보낼 준비
	DeviceContext->ClearRenderTargetView(rtv, Color(0, 0, 0, 0));
}
