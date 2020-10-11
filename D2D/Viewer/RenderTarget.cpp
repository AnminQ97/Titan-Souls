#include"stdafx.h"
#include"RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height, DXGI_FORMAT format)
{
	this->width = (width < 1) ? Width : width;
	this->height = (height < 1) ? Height : height;

	//2D�ؽ��� �����
	//DESC
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = this->width; 
	textureDesc.Height = this->height;
	textureDesc.ArraySize = 1; //texture2D�迭
	textureDesc.Format = format;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;//��� �뵵 ������ ����
	textureDesc.MipLevels = 1; //�־������� �ȼ� ������  
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
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;//����
	srvDesc.Texture2D.MipLevels = 1;				  //���ҽ�	  //Desc	//������
	assert(SUCCEEDED(Device->CreateShaderResourceView(backBuffer, &srvDesc, &srv)));

}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(backBuffer);
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(srv);
}

void RenderTarget::SaveTexture(wstring file)//����
{
	HRESULT hr =D3DX11SaveTextureToFile(DeviceContext, backBuffer, D3DX11_IFF_PNG,file.c_str());
	assert(SUCCEEDED(hr));
}

void RenderTarget::Set() //om stage����
{
	DeviceContext->OMSetRenderTargets(1, &rtv, NULL); //����� ȭ�� ���鿡 ������ �غ�
	DeviceContext->ClearRenderTargetView(rtv, Color(0, 0, 0, 0));
}
