#include <d3dcompiler.h>
#include "DirectX11.h"

#pragma comment(lib, "d3dcompiler.lib")

namespace Lib
{
    // �R���X�g���N�^
    DirectX11::DirectX11(std::shared_ptr<Window> _window)
        :window(_window)
    {
        device           = nullptr;
        deviceContext    = nullptr;
        swapChain        = nullptr;
        renderTargetView = nullptr;
        initDevice();
    }

    // �f�X�g���N�^
    DirectX11::~DirectX11()
    {
    }

    // �t���[���̊J�n
    void DirectX11::begineFrame() const
    {
        float ClearColor[4] { 0.0f, 0.125f, 0.3f, 1.0f };
        deviceContext->ClearRenderTargetView(renderTargetView.Get(), ClearColor);
    }

    void DirectX11::render()
    {
        // Z�o�b�t�@�[�̃N���A
        deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

        ConstantBuffer cb;
        cb.world = XMMatrixTranspose(world);
        cb.view  = XMMatrixTranspose(view);
        cb.projection = XMMatrixTranspose(projection);
        deviceContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

        deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
        deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
        deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
        deviceContext->DrawIndexed(36, 0, 0);
    }

    // �t���[���̏I��
    void DirectX11::endFrame() const
    {
        swapChain->Present(0, 0);
    }

    // ������
    HRESULT DirectX11::initDevice()
    {
        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        // �h���C�o�[�̎�ނ̃I�v�V����
        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);

        // Direct3D�f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1,
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        UINT windowWidth = window->getWindowRect().right - window->getWindowRect().left;
        UINT windowHeight = window->getWindowRect().bottom - window->getWindowRect().top;

        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;								   // �X���b�v�`�F�[���̃o�b�t�@�[��
        sd.BufferDesc.Width = windowWidth;				   // �𑜓x�̕�
        sd.BufferDesc.Height = windowHeight;			   // �𑜓x�̍���
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �\���t�H�[�}�b�g
        sd.BufferDesc.RefreshRate.Numerator = 60;		   // �L�����̍ő�l
        sd.BufferDesc.RefreshRate.Denominator = 1;		   // �L�����̍ŏ��l
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // �o�b�N�o�b�t�@�[�̃T�[�t�F�X�g�p�@�����CPU�A�N�Z�X�I�v�V����
        sd.OutputWindow = window->getHWND();			   // �o�̓E�B���h�E�ւ�HWND�n���h��
        sd.SampleDesc.Count = 1;						   // �s�N�Z���P�ʂ̃}���`�T���v�����O�̐�
        sd.SampleDesc.Quality = 0;						   // �C���[�W�̕i�����x��
        sd.Windowed = TRUE;								   // �o�͂��E�B���h�E���[�h�̏ꍇ��TRUE����ȊO��FALSE

        HRESULT hr = S_OK;
        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            driverType = driverTypes[driverTypeIndex];
            hr = D3D11CreateDeviceAndSwapChain(
                nullptr,                               // �g�p����r�f�I�A�_�v�^�[�ւ̃|�C���^�[�i����̂��̂��g���ꍇNULL�j
                driverType,                            // �g�p����f�o�C�X�̎��
                nullptr,                               // �\�t�g�E�F�A���X�^���C�U�[����������DLL�̃n���h��
                createDeviceFlags,                     // �L���ɂ��郉���^�C�����C���[
                featureLevels,                         // D3D_FEATURE_LEVEL�̔z��ւ̃|�C���^�[
                numFeatureLevels,                      // ��L�z��̗v�f��
                D3D11_SDK_VERSION,                     // SDK�̃o�[�W����
                &sd,                                   // DXGI_SWAP_CHAIN�ւ̃|�C���^�[
                swapChain.GetAddressOf(),              // IDXGISwapChain�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
                device.GetAddressOf(),                 // ID3D11Device�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
                &featureLevel,	                       // D3D_FEATURE_LEVEL�ւ̃|�C���^�[��Ԃ�
                deviceContext.GetAddressOf()           // ID3D11DeviceContext�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
            );

            if (SUCCEEDED(hr)) {
                break;
            }
        }
        if (FAILED(hr)) {
            MessageBox(nullptr, L"D3DCreateDeviceAndSwapChain()�̎��s : " + hr, L"Error", MB_OK);
            return hr;
        }

        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

        // �o�b�N�o�b�t�@�̎擾
        hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
        if (FAILED(hr)) {
            MessageBox(nullptr, L"GetBuffer()�̎��s : " + hr, nullptr, MB_OK);
            return hr;
        }
        // �����_�[�^�[�Q�b�g�r���[�̍쐬
        hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"CreateRenderTargetView()�̎��s : " + hr, nullptr, MB_OK);
            return hr;
        }

        // ���[�x�X�e���V����Z�o�b�t�@�[�Ƃ��Ă΂��
        // �[�x�X�e���V���p�e�N�X�`�����\�[�X���쐬
        D3D11_TEXTURE2D_DESC descDepth;
        ZeroMemory(&descDepth, sizeof(descDepth));
        descDepth.Width              = windowWidth;                   // �e�N�X�`���[�̕�(�o�b�N�o�b�t�@�Ɠ����T�C�Y���w��)
        descDepth.Height             = windowHeight;                  // �e�N�X�`���[�̍���(�o�b�N�o�b�t�@�Ɠ����T�C�Y���w��)
        descDepth.MipLevels          = 1;                             // �~�b�v�}�b�v���x���̍ő吔
        descDepth.ArraySize          = 1;                             // �e�N�X�`���[�z����̃e�N�X�`���[�̐�
        descDepth.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT; // �e�N�X�`���[�̃t�H�[�}�b�g
        descDepth.SampleDesc.Count   = 1;                             // �s�N�Z���P�ʂ̃}���`�T���v�����O��
        descDepth.SampleDesc.Quality = 0;                             // �C���[�W�̕i�����x��
        descDepth.Usage              = D3D11_USAGE_DEFAULT;           // �e�N�X�`���[�̓ǂݍ��݂���я������ݕ��@�����ʂ���l
        descDepth.BindFlags          = D3D11_BIND_DEPTH_STENCIL;      // �p�C�v���C���X�e�[�W�ւ̃o�C���h�Ɋւ���t���O
        descDepth.CPUAccessFlags     = 0;                             // ������CPU�A�N�Z�X�̎�ނ��w�蔄��t���O
        descDepth.MiscFlags          = 0;                             // ���̈�ʐ��̒Ⴂ���\�[�X�I�v�V���������ʂ���t���O
        hr = device->CreateTexture2D(&descDepth, nullptr, depthStencil.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"CreateTexture2D()�̎��s : " + hr, L"Error", MB_OK);
            return hr;
        }

        // �[�x�X�e���V���r���[����A�N�Z�X�\�ȃe�N�X�`���[�̃T�u���\�[�X���w��
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));
        descDSV.Format             = descDepth.Format;              // ���\�[�X�f�[�^�̃t�H�[�}�b�g
        descDSV.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D; // ���\�[�X�̃^�C�v
        descDSV.Texture2D.MipSlice = 0;                             // �ŏ��Ɏg�p����~�b�v�}�b�v���x���̃C���f�b�N�X
        // ���\�[�X�f�[�^�ւ̃A�N�Z�X�p�ɐ[�x�X�e���V���r���[�̍쐬
        hr = device->CreateDepthStencilView(depthStencil.Get(), &descDSV, depthStencilView.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"CreateDepthStencilView()�̎��s : " + hr, L"Error", MB_OK);
            return hr;
        }
        // �[�x�X�e���V���r���[���^�[�Q�b�g�ɃZ�b�g
        deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
   
        // Viewport�̏�����
        D3D11_VIEWPORT vp;
        vp.Width    = static_cast<FLOAT>(windowWidth);  // �r���[�|�[�g������x�ʒu
        vp.Height   = static_cast<FLOAT>(windowHeight); // �r���[�|�[�g�㑤��y�ʒu
        vp.MinDepth = 0.0f;                             // �r���[�|�[�g��
        vp.MaxDepth = 1.0f;                             // �r���[�|�[�g�̍���
        vp.TopLeftX =   0;                              // �r���[�|�[�g�̍ŏ��[�x(0�`1)
        vp.TopLeftY =   0;                              // �r���[�|�[�g�̍ő�[�x(0�`1)
        deviceContext->RSSetViewports(1, &vp);

        // VertexShader�̓ǂݍ���
        auto VSBlob = shaderCompile(L"VertexShader.hlsl", "VS", "vs_4_0");
        if (VSBlob == nullptr) {
            MessageBox(nullptr, L"shaderCompile()�̎��s(VS)", L"Error", MB_OK);
            return hr;
        }

        // VertexShader�̍쐬
        hr = device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"VS�R���p�C�����s", L"Error", MB_OK);
            return hr;
        }

        // InputLayou�̒�`
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        // InputLayout�̍쐬
        hr = device->CreateInputLayout(layout, numElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), vertexLayout.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(NULL, L"CreateInputLayout�̎��s(VS)", L"Error", MB_OK);
            return hr;
        }

        // InputLayout���Z�b�g
        deviceContext->IASetInputLayout(vertexLayout.Get());

        // PixelShader�̓ǂݍ���
        auto PSBlob = shaderCompile(L"PixelShader.hlsl", "PS", "ps_4_0");
        if (PSBlob == nullptr) {
            MessageBox(nullptr, L"shaderCompile()�̎��s(VS)", L"Error", MB_OK);
            return hr;
        }

        // VertexShader�̍쐬
        hr = device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createPixelShader()�̎��s", L"Error", MB_OK);
            return hr;
        }

        // VertexBuffer�̒�`
        SimpleVertex vertices[] =
        {
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
        };
        
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.ByteWidth      = sizeof(SimpleVertex) * 8;
        bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        ZeroMemory(&initData, sizeof(initData));
        initData.pSysMem = vertices;
        hr = device->CreateBuffer(&bd, &initData, vertexBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()�̎��s", L"Error", MB_OK);
            return hr;
        }

        // VertexBuffer���Z�b�g
        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

        // ���_�o�b�t�@�̍쐬
        WORD indices[] = 
        {
            3, 1, 0,
            2, 1, 3,

            0, 5, 4,
            1, 5, 0,

            3, 4, 7,
            0, 4, 3,

            1, 6, 5,
            2, 6, 1,

            2, 7, 6,
            3, 7, 2,

            6, 4, 5,
            7, 4, 6,
        };
        bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.ByteWidth      = sizeof(WORD) * 36; // 36���_�A12�O�p�`
        bd.BindFlags      = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        initData.pSysMem  = indices;
        hr = device->CreateBuffer(&bd, &initData, indexBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()�̎��s", L"Error", MB_OK);
            return hr;
        }

        // ���_�o�b�t�@���Z�b�g
        deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        // PrimitiveTopology���Z�b�g
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // ConstantBuffer�̍쐬
        bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.ByteWidth      = sizeof(ConstantBuffer);
        bd.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        hr = device->CreateBuffer(&bd, nullptr, constantBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()�̎��s", L"Error", MB_OK);
            return hr;
        }

        // WorldMatrix�̏�����
        world = XMMatrixIdentity();
                
        // ViewMatrix�̏�����
        XMVECTOR eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
        XMVECTOR at  = XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f);
        XMVECTOR up  = XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f);
        view = XMMatrixLookAtLH(eye, at, up);

        // ProjectionMatrix�̏�����
        projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, windowWidth / static_cast<FLOAT>(windowHeight), 0.01f, 100.0f);

        return S_OK;
    }

    // �V�F�[�_�[�̓ǂݍ���
    Microsoft::WRL::ComPtr<ID3DBlob> DirectX11::shaderCompile(WCHAR * filename, LPCSTR entryPoint, LPCSTR shaderModel)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> blobOut = nullptr;
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

        DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
        shaderFlags |= D3DCOMPILE_DEBUG;
#endif

        auto hr = D3DCompileFromFile(
            filename,
            nullptr,
            nullptr,
            entryPoint,
            shaderModel,
            shaderFlags,
            0,
            blobOut.GetAddressOf(),
            errorBlob.GetAddressOf()
        );

        if (FAILED(hr)) {
            if (errorBlob != nullptr) {
                MessageBox(window->getHWND(), static_cast<LPWSTR>(errorBlob->GetBufferPointer()), nullptr, MB_OK);
            }
            if (errorBlob) {
                errorBlob.Get()->Release();
            }
        }
        if (errorBlob) {
            errorBlob.Get()->Release();
        }

        return Microsoft::WRL::ComPtr<ID3DBlob>(blobOut);
    }
