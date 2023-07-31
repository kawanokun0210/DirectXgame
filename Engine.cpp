#include "Engine.h"
#include <assert.h>

IDxcBlob* CreateEngine::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	//これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{},profile:{}\n", filePath, profile)));

	/************************************************************************
	hlsl(High-Level Shader Language)はDirectX12の各種Shaderを記述するための言語
	************************************************************************/
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	dxCommon_->SetHr(dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource));

	//読めなかったら決める
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
	LPCWSTR arguments[] = {
		filePath.c_str(),//コンパイル対象のhlslファイル名
		L"-E",L"main",//エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,//ShaderProflieの設定
		L"-Zi",L"-Qembed_debug",//デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr",//メモリレイアウトは行優先
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	dxCommon_->SetHr(dxcCompiler->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		includeHandler, // includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	));

	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//警告・エラーが出たらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());
		//警告・エラーダメ絶対
		assert(false);
	}

	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	dxCommon_->SetHr(shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr));
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{},profile:{}\n", filePath, profile)));

	//もう使わないリソースを開放
	shaderSource->Release();
	shaderResult->Release();

	//実行用のバイナリを返却
	return shaderBlob;
}

void CreateEngine::InitializeDxcCompiler()
{
	HRESULT hr;
	dxcUtils_ = nullptr;
	dxcCompiler_ = nullptr;

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	//現時点でincludeはしないが、includeに対応するための設定を行っていく
	includeHandler_ = nullptr;
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

void CreateEngine::CreateRootSignature()
{
	/****************************************************************
	//RootSignature作成
	//ShaderとResourceをどのように関連付けるかを示したオブジェクト
	****************************************************************/
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	/*********************************************
	//RootParameter作成、複数設定できるので配列。
	//データそれぞれのBind情報である
	*********************************************/
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ

	//シリアライズしてバイナリにする
	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;
	HRESULT hr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);

	if (FAILED(dxCommon_->GetHr()))
	{
		Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}

	//バイナリを元に生成
	rootSignature_ = nullptr;
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
		signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void CreateEngine::CreateInputlayOut()
{
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);
}

void CreateEngine::BlendState()
{
	//すべての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//透明度
	blendDesc_.RenderTarget[0].BlendEnable = true;
	blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
}

void CreateEngine::RasterizerState()
{
	//裏面（時計回り）を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;

	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderをコンパイルする
	vertexShaderBlob_ = CompileShader(L"Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CompileShader(L"Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(pixelShaderBlob_ != nullptr);
}

/*********************************************************************
RenderingPipelineはモデルデータの入力から出力までのレンダリングのための加工手順
PipelineStateObject(PSO)は描画に関する設定が大量に詰め込まれたオブジェクト
*********************************************************************/

void CreateEngine::InitializePSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	//RootSignature
	graphicsPipelineStateDesc.pRootSignature = rootSignature_;

	/**********************************************************************
	InputlayoutはVertexShaderへ渡す頂点データがどのようなものかを指定するオブジェクト
	**********************************************************************/
	//Inputlayout
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;

	//vertexShader
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(),
		vertexShaderBlob_->GetBufferSize() };

	//pixcelShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(),
		pixelShaderBlob_->GetBufferSize() };

	/********************************************************************
	BlendStateとはPixelShaderからの出力を画面にどのように書き込むかを設定する項目
	********************************************************************/
	//BlendState
	graphicsPipelineStateDesc.BlendState = blendDesc_;

	/********************************************************************
	Rasterrizerは頂点のピクセル化
	RasterrizerStateはRasterrizerに対する設定
	********************************************************************/
	//rasterizerState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのかの設定（気にしなく良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//実際に生成
	graphicsPipelineState_ = nullptr;
	HRESULT hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

/*******************************
Viewportは表示領域
*******************************/
void CreateEngine::ViewPort()
{
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = WinApp::kClientWidth;
	viewport_.Height = WinApp::kClientHeight;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}

void CreateEngine::ScissorRect()
{
	//シザー短形
	scissorRect_.left = 0;
	scissorRect_.right = WinApp::kClientWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = WinApp::kClientHeight;
}

void CreateEngine::Initialize()
{
}

void CreateEngine::Initialization(WinApp* win, const wchar_t* title, int32_t width, int32_t height)
{
	dxCommon_ = new DirectXCommon;

	dxCommon_->Initialization(win, title, win->kClientWidth, win->kClientHeight);

	InitializeDxcCompiler();

	CreateRootSignature();

	CreateInputlayOut();

	BlendState();

	RasterizerState();

	InitializePSO();

	ViewPort();

	ScissorRect();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(win_->GetHwnd());
	ImGui_ImplDX12_Init(dxCommon_->device_,
		dxCommon_->GetSwapChainDesc().BufferCount,
		dxCommon_->GetRTVDesc().Format,
		dxCommon_->srvDescriptorHeap_,
		dxCommon_->srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(),
		dxCommon_->srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart()
	);
}

void CreateEngine::BeginFrame()
{

	dxCommon_->PreDraw();
	//viewportを設定
	dxCommon_->GetCommandList()->RSSetViewports(1, &viewport_);
	//scirssorを設定
	dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);
	//RootSignatureを設定。PS0とは別途設定が必要
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature_);
	//PS0を設定
	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState_);
}

void CreateEngine::EndFrame()
{
	dxCommon_->PostDraw();
}

void CreateEngine::Finalize()
{
	for (int i = 0; i < 3; i++)
	{
		triangle_[i]->Finalize();
	}

	graphicsPipelineState_->Release();
	signatureBlob_->Release();

	if (errorBlob_)
	{
		errorBlob_->Release();
	}

	rootSignature_->Release();
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();
	dxCommon_->Finalize();
}

void CreateEngine::Update() {
	transform_.rotate.y += 0.03f;
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
}

void CreateEngine::Draw() {
	Vector4 color = *triangle_[0]->materialData_;
	ImGui::Begin("Triangle");
	ImGui::ColorPicker4("color", &color.x);
	ImGui::End();
	*triangle_[0]->materialData_ = color;
	for (int i = 0; i < 3; i++) {
		triangle_[i]->Draw(worldMatrix_);
	}
}

void CreateEngine::VariableInialize() {

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Triangle tri[3] = {};

	tri[0].v1 = { -0.4f,0.5f,0.0f,2.0f };
	tri[0].v2 = { 0.0f,0.8f,0.0f,2.0f };
	tri[0].v3 = { 0.4f,0.5f,0.0f,2.0f };
	tri[0].material = { 1.0f,0.0f,0.0f,1.0f };

	tri[1].v1 = { -0.8f,-0.9f,0.0f,1.0f };
	tri[1].v2 = { -0.6f,-0.6f,0.0f,1.0f };
	tri[1].v3 = { -0.4f,-0.9f,0.0f,1.0f };
	tri[1].material = { 0.0f,1.0f,0.0f,1.0f };

	tri[2].v1 = { 0.4f,-0.7f,0.0f,1.0f };
	tri[2].v2 = { 0.6f,-0.4f,0.0f,1.0f };
	tri[2].v3 = { 0.8f,-0.8f,0.0f,1.0f };
	tri[2].material = { 0.0f,0.0f,1.0f,1.0f };

	for (int i = 0; i < 3; i++)
	{
		triangle_[i] = new CreateTriangle();
		triangle_[i]->Initialize(dxCommon_, tri[i].v1, tri[i].v2, tri[i].v3, tri[i].material);
	}
}

DirectX::ScratchImage LoadTexture(const std::string& filePath) {
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	return mipImages;
}

ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metaData) {
	//1.metaデータを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metaData.width);								//Textureの幅
	resourceDesc.Height = UINT(metaData.height);							//Textureの高さ
	resourceDesc.MipLevels = UINT16(metaData.mipLevels);					//mipmapの数		
	resourceDesc.DepthOrArraySize = UINT16(metaData.arraySize);				//奥行き or 配列Textureの配列数
	resourceDesc.Format = metaData.format;									//TextureのFormat
	resourceDesc.SampleDesc.Count = 1;										//サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metaData.dimension);	//Textureの次元数。普段使っているのは2次元

	//2.利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;							//細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;	//WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;				//プロセッサの近くに配置

	//3.REsourceを生成する
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);

		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(hr));
	}
}



WinApp* CreateEngine::win_;
DirectXCommon* CreateEngine::dxCommon_;