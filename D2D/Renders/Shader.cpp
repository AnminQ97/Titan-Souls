#include "stdafx.h"
#include "Shader.h"

Shader::Shader(wstring file)
	: file(L"./_Shaders/" +  file)
{
	initialStateBlock = new StateBlock();
	{
		DeviceContext->RSGetState(&initialStateBlock->RSRasterizerState);
		DeviceContext->OMGetBlendState(&initialStateBlock->OMBlendState, initialStateBlock->OMBlendFactor, &initialStateBlock->OMSampleMask);
		DeviceContext->OMGetDepthStencilState(&initialStateBlock->OMDepthStencilState, &initialStateBlock->OMStencilRef);
	}

	CreateEffect();
}

Shader::~Shader()
{
	effect->Release();
}

void Shader::CreateEffect()
{
	ID3DBlob* fxBlob;
	ID3DBlob* error;
	
	//INT flag = D3D10_SHADER_ENABLE_STRICTNESS;
	INT flag = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
	HRESULT hr = D3DCompileFromFile(file.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "fx_5_0", flag, NULL, &fxBlob, &error);
	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}
	
	hr = D3DX11CreateEffectFromMemory(fxBlob->GetBufferPointer(), fxBlob->GetBufferSize(), 0, Device, &effect);
	assert(SUCCEEDED(hr));

	effect->GetDesc(&effectDesc);
	for (UINT t = 0; t < effectDesc.Techniques; t++)
	{
		Technique technique;
		technique.ITechnique = effect->GetTechniqueByIndex(t);
		technique.ITechnique->GetDesc(&technique.Desc);
		technique.Name = String::ToWString(technique.Desc.Name);

		for (UINT p = 0; p < technique.Desc.Passes; p++)
		{
			Pass pass;
			pass.IPass = technique.ITechnique->GetPassByIndex(p);
			pass.IPass->GetDesc(&pass.Desc);
			pass.Name = String::ToWString(pass.Desc.Name);
			pass.IPass->GetVertexShaderDesc(&pass.PassVsDesc);
			pass.PassVsDesc.pShaderVariable->GetShaderDesc(pass.PassVsDesc.ShaderIndex, &pass.EffectVsDesc);

			for (UINT s = 0; s < pass.EffectVsDesc.NumInputSignatureEntries; s++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC desc;
				
				HRESULT hr = pass.PassVsDesc.pShaderVariable->GetInputSignatureElementDesc(pass.PassVsDesc.ShaderIndex, s, &desc);
				assert(SUCCEEDED(hr));

				pass.SignatureDescs.push_back(desc);
			}

			pass.InputLayout = CreateInputLayout(fxBlob, &pass.EffectVsDesc, pass.SignatureDescs);
			pass.StateBlock = initialStateBlock;

			technique.Passes.push_back(pass);
		}

		techniques.push_back(technique);
	}

	for (UINT i = 0; i < effectDesc.ConstantBuffers; i++)
	{
		EffectBuffer cbuffer;

		ID3DX11EffectConstantBuffer* iBuffer;
		iBuffer = effect->GetConstantBufferByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		iBuffer->GetDesc(&vDesc);

		cbuffer.index = i;
		cbuffer.Buffer = iBuffer;
		cbuffer.Desc = vDesc;

		cBufferMap.insert(pair<string, EffectBuffer>(vDesc.Name, cbuffer));
	}

	for (UINT i = 0; i < effectDesc.GlobalVariables; i++)
	{
		EffectVariable variable;

		ID3DX11EffectVariable* iVariable;
		iVariable = effect->GetVariableByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		iVariable->GetDesc(&vDesc);

		variable.index = i;
		variable.Variable = iVariable;

		variableMap.insert(pair<string, EffectVariable>(vDesc.Name, variable));
	}

	fxBlob->Release();
}

ID3D11InputLayout * Shader::CreateInputLayout(ID3DBlob * fxBlob, D3DX11_EFFECT_SHADER_DESC* effectVsDesc, vector<D3D11_SIGNATURE_PARAMETER_DESC>& params)
{
	vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (D3D11_SIGNATURE_PARAMETER_DESC& paramDesc : params)
	{
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string name = paramDesc.SemanticName;
		if (name == "POSITION")
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		if (name == "INSTANCE")
		{
			elementDesc.InputSlot = 1;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}

		inputLayoutDesc.push_back(elementDesc);
	}

	
	const void* pCode =  effectVsDesc->pBytecode;
	UINT pCodeSize = effectVsDesc->BytecodeLength;

	if (inputLayoutDesc.size() > 0)
	{
		ID3D11InputLayout* inputLayout = NULL;
		HRESULT hr = Device->CreateInputLayout
		(
			&inputLayoutDesc[0]
			, inputLayoutDesc.size()
			, pCode
			, pCodeSize
			, &inputLayout
		);
		assert(SUCCEEDED(hr));

		return inputLayout;
	}

	return NULL;
}

void Shader::Pass::Draw(UINT vertexCount, UINT startVertexLocation)
{
	BeginDraw();
	{
		DeviceContext->Draw(vertexCount, startVertexLocation);
	}
	EndDraw();
}

void Shader::Pass::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	BeginDraw();
	{
		DeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	}
	EndDraw();
}

void Shader::Pass::DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	BeginDraw();
	{
		DeviceContext->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
	}
	EndDraw();
}

void Shader::Pass::DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	BeginDraw();
	{
		DeviceContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startIndexLocation);
	}
	EndDraw();
}

void Shader::Pass::BeginDraw()
{
	IPass->ComputeStateBlockMask(&StateBlockMask);

	DeviceContext->IASetInputLayout(InputLayout);
	IPass->Apply(0, DeviceContext);
}

void Shader::Pass::EndDraw()
{
	if (StateBlockMask.RSRasterizerState == 1)
		DeviceContext->RSSetState(StateBlock->RSRasterizerState);

	if (StateBlockMask.OMDepthStencilState == 1)
		DeviceContext->OMSetBlendState(StateBlock->OMBlendState, StateBlock->OMBlendFactor, StateBlock->OMSampleMask);

	if (StateBlockMask.OMBlendState == 1)
		DeviceContext->OMSetDepthStencilState(StateBlock->OMDepthStencilState, StateBlock->OMStencilRef);
}

ID3DX11EffectVariable * Shader::Variable(string name)
{
	return variableMap[name].Variable;
}

ID3DX11EffectScalarVariable * Shader::AsScalar(string name)
{
	return variableMap[name].Variable->AsScalar();
}

ID3DX11EffectVectorVariable * Shader::AsVector(string name)
{
	return variableMap[name].Variable->AsVector();
}

ID3DX11EffectMatrixVariable * Shader::AsMatrix(string name)
{
	return variableMap[name].Variable->AsMatrix();
}

ID3DX11EffectStringVariable * Shader::AsString(string name)
{
	return variableMap[name].Variable->AsString();
}

ID3DX11EffectShaderResourceVariable * Shader::AsShaderResource(string name)
{
	return variableMap[name].Variable->AsShaderResource();
}

ID3DX11EffectRenderTargetViewVariable * Shader::AsRenderTargetView(string name)
{
	return variableMap[name].Variable->AsRenderTargetView();
}

ID3DX11EffectDepthStencilViewVariable * Shader::AsDepthStencilView(string name)
{
	return variableMap[name].Variable->AsDepthStencilView();
}

ID3DX11EffectConstantBuffer * Shader::AsConstantBuffer(string name)
{
	return variableMap[name].Variable->AsConstantBuffer();
}

ID3DX11EffectShaderVariable * Shader::AsShader(string name)
{
	return variableMap[name].Variable->AsShader();
}

ID3DX11EffectBlendVariable * Shader::AsBlend(string name)
{
	return variableMap[name].Variable->AsBlend();
}

ID3DX11EffectDepthStencilVariable * Shader::AsDepthStencil(string name)
{
	return variableMap[name].Variable->AsDepthStencil();
}

ID3DX11EffectRasterizerVariable * Shader::AsRasterizer(string name)
{
	return variableMap[name].Variable->AsRasterizer();
}

ID3DX11EffectSamplerVariable * Shader::AsSampler(string name)
{
	return variableMap[name].Variable->AsSampler();
}

void Shader::Technique::Draw(UINT pass, UINT vertexCount, UINT startVertexLocation)
{
	Passes[pass].Draw(vertexCount, startVertexLocation);
}

void Shader::Technique::DrawIndexed(UINT pass, UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	Passes[pass].DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Shader::Technique::DrawInstanced(UINT pass, UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	Passes[pass].DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}

void Shader::Technique::DrawIndexedInstanced(UINT pass, UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	Passes[pass].DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void Shader::Draw(UINT technique, UINT pass, UINT vertexCount, UINT startVertexLocation)
{
	techniques[technique].Passes[pass].Draw(vertexCount, startVertexLocation);
}

void Shader::DrawIndexed(UINT technique, UINT pass, UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	techniques[technique].Passes[pass].DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Shader::DrawInstanced(UINT technique, UINT pass, UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	techniques[technique].Passes[pass].DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}

void Shader::DrawIndexedInstanced(UINT technique, UINT pass, UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	techniques[technique].Passes[pass].DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}