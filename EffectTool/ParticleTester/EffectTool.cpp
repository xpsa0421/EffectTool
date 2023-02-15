#include "EffectTool.h"

bool EffectTool::Init()
{
    // initialise camera
    cam_ = new Camera;
    cam_->Init();
    cam_->SetView(XMFLOAT3(0, 0, -10), XMFLOAT3(0, 0, 0));
    cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f,
        (float)g_rectClient.right / (float)g_rectClient.bottom);

    // initialise object
    particle_system_ = new ParticleSystem;
    particle_system_->Init();
    particle_system_->SetVertexShader(L"VertexShader.hlsl", L"main");
    particle_system_->SetPixelShader(L"PixelShader.hlsl", L"main");
    particle_system_->SetGeometryShader(L"GeometryShader.hlsl", L"main");
    
    //template code for adding textures
    std::vector<W_STR> tex_names = {
        L"../../data/image/shine0.bmp",
        L"../../data/image/shine1.bmp",
        L"../../data/image/shine2.bmp",
        L"../../data/image/shine3.bmp",
        L"../../data/image/shine4.bmp",
        L"../../data/image/shine5.bmp",
        L"../../data/image/shine6.bmp",
        L"../../data/image/shine7.bmp",
        L"../../data/image/shine8.bmp",
        L"../../data/image/shine9.bmp"
    };
    particle_system_->num_textures = tex_names.size();
    particle_system_->SetTextureSRV(CreateTexture2DArraySRV(device_.Get(), device_context_.Get(), tex_names));


    particle_system_->Create(device_.Get(), device_context_.Get());
    
    // generate per frame geometry shader constant buffer
    D3D11_BUFFER_DESC constant_desc;
    ZeroMemory(&constant_desc, sizeof(constant_desc));
    constant_desc.ByteWidth = sizeof(CdPerFrame) * 1;
    constant_desc.Usage = D3D11_USAGE_DEFAULT;
    constant_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT result = device_->CreateBuffer(&constant_desc, nullptr, gs_cbuffer_per_frame_.GetAddressOf());
    #ifdef _DEBUG
    if (FAILED(result))
    {
        printf("[EffectTool] Failed to build per frame GS constant buffer\n");
    }
    #endif // _DEBUG

    // if burst template
    particle_system_->particles_.resize(1);

    return true;
}

bool EffectTool::Frame()
{
    cam_->Frame();

    // generate billboard matrix
    XMFLOAT4X4 cam_view_in;
    XMStoreFloat4x4(&cam_view_in, XMMatrixInverse(nullptr, XMLoadFloat4x4(&cam_->view_)));
    //cam_view_in._12 = cam_view_in._22 = cam_view_in._32 = 0.0f;
    cam_view_in._41 = cam_view_in._42 = cam_view_in._43 = 0.0f;
    
    XMFLOAT4X4 cdata_billboard;
    XMFLOAT4X4 cdata_view;
    XMFLOAT4X4 cdata_proj;

    XMStoreFloat4x4(&cdata_billboard, XMMatrixTranspose(XMLoadFloat4x4(&cam_view_in)));
    XMStoreFloat4x4(&cdata_view, XMMatrixTranspose(XMLoadFloat4x4(&cam_->view_)));
    XMStoreFloat4x4(&cdata_proj, XMMatrixTranspose(XMLoadFloat4x4(&cam_->proj_)));

    gs_cdata_per_frame_.billboard = cdata_billboard;
    gs_cdata_per_frame_.view = cdata_view;
    gs_cdata_per_frame_.proj = cdata_proj;
   
    particle_system_->Frame();

	return true;
}

bool EffectTool::Render()
{
    device_context_->UpdateSubresource(gs_cbuffer_per_frame_.Get(), 0, 0, &gs_cdata_per_frame_, 0, 0);
    device_context_->GSSetConstantBuffers(0, 1, gs_cbuffer_per_frame_.GetAddressOf());

    particle_system_->Render();
	return true;
}

bool EffectTool::Release()
{
    particle_system_->Release();
    delete particle_system_;
    gs_cbuffer_per_frame_ = nullptr;

    return true;
}

ID3D11ShaderResourceView* EffectTool::CreateTexture2DArraySRV(
    ID3D11Device* device, ID3D11DeviceContext* context, std::vector<W_STR>& filenames)
{
    int num_textures = filenames.size();
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
    std::vector<ComPtr<ID3D11Texture2D>> src_textures(num_textures);

    for (int i = 0; i < num_textures; i++)
    {
        HRESULT hr = DirectX::CreateWICTextureFromFileEx(device,
            filenames[i].c_str(),
            0, D3D11_USAGE_STAGING,
            0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
            0, WIC_LOADER_DEFAULT,
            (ID3D11Resource**)src_textures[i].GetAddressOf(),
            nullptr);
    }

    D3D11_TEXTURE2D_DESC texElementDesc;
    src_textures[0]->GetDesc(&texElementDesc);

    D3D11_TEXTURE2D_DESC texArrayDesc;
    ZeroMemory(&texArrayDesc, sizeof(texArrayDesc)); 
    texArrayDesc.Width = texElementDesc.Width;
    texArrayDesc.Height = texElementDesc.Height;
    texArrayDesc.MipLevels = texElementDesc.MipLevels;
    texArrayDesc.ArraySize = num_textures;
    texArrayDesc.Format = texElementDesc.Format;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = 0;

    ComPtr<ID3D11Texture2D> texArray = 0;
    device->CreateTexture2D(&texArrayDesc, 0, texArray.GetAddressOf());

    for (UINT texElement = 0; texElement < num_textures; ++texElement)
    {
        // for each mipmap level...
        for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
        {
            D3D11_MAPPED_SUBRESOURCE mappedTex2D;
            context->Map(src_textures[texElement].Get(), mipLevel, D3D11_MAP_READ, 0, &mappedTex2D);

            context->UpdateSubresource(texArray.Get(),
                D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
                0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

            context->Unmap(src_textures[texElement].Get(), mipLevel);
        }
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texArrayDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
    viewDesc.Texture2DArray.FirstArraySlice = 0;
    viewDesc.Texture2DArray.ArraySize = num_textures;

    ID3D11ShaderResourceView* texArraySRV = 0;
    device->CreateShaderResourceView(texArray.Get(), &viewDesc, &texArraySRV);

    texArray = nullptr;
    for (int i = 0; i < num_textures; i++) src_textures[i] = nullptr;

    return texArraySRV;   
}


// EffectTool function
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    EffectTool effect_tool;
    effect_tool.SetWindow(hInstance, L"effect_tool", 800, 600);
    effect_tool.Run();

    return 1;
}