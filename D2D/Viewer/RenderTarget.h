#pragma once

class RenderTarget
{
public:
	RenderTarget(UINT width = 0, UINT height = 0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	~RenderTarget();

	ID3D11RenderTargetView* GetRTV() { return rtv; }
	ID3D11ShaderResourceView* SRV() { return srv; }

	void SaveTexture(wstring file); //스샷
	
	void Set(); //파이프 라인 세팅

private:
	//화면
	UINT width, height;
	DXGI_FORMAT format;

	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv; //백버퍼 내용 쉐이더나 다른데로 보내줌
};