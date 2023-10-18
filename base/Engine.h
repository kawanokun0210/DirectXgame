#pragma once
#include "DirectX.h"
#include <dxcapi.h>
#include "../String.h"
#include "MatrixCalculation.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "Triangle.h"
#include "../externals/DirectXTex/d3dx12.h"
#include <vector>
#pragma comment(lib,"dxcompiler.lib")

class MyEngine
{
public:
	void Initialize(const wchar_t* title, int32_t width, int32_t height);

	void BeginFrame();

	void EndFrame();

	void Finalize();

	void Update();

	void SettingTexture(const std::string& filePath, uint32_t index);

	void SettingObjTexture(uint32_t index);

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	DirectXCommon* GetDirectXCommon() { return dxCommon_; }

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[2];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[2];

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorheap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorheap, uint32_t descriptorSize, uint32_t index);

private:
	static	DirectXCommon* dxCommon_;

	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;

	IDxcIncludeHandler* includeHandler_;

	ID3DBlob* signatureBlob_;
	ID3DBlob* errorBlob_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};

	D3D12_BLEND_DESC blendDesc_{};

	IDxcBlob* vertexShaderBlob_;

	IDxcBlob* pixelShaderBlob_;

	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3];

	//頂点リソースにデータを書き込む
	Vector4* vertexData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource_[2];

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_[2];
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResources_[2];
	uint32_t descriptorSizeSRV;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;

	IDxcBlob* CompileShader(
		//CompileShaderするShaderファイルへのパス
		const std::wstring& filePath,
		//Compielerに使用するProfile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void InitializeDxcCompiler();
	void CreateRootSignature();
	void CreateInputlayOut();
	void BlendState();
	void RasterizerState();
	void InitializePSO();
	void ViewPort();
	void ScissorRect();
	void SettingDepth();

	DirectX::ScratchImage LoadTexture(const std::string& filePath);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata);
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadtextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages, uint32_t index);
};