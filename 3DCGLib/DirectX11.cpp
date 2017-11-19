#include "DirectX11.h"

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
        initialize();
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

    // �t���[���̏I��
    void DirectX11::endFrame() const
    {
        swapChain->Present(0, 0);
    }

    // ������
    HRESULT DirectX11::initialize()
    {
        auto hr = createDeviceAndSwapChain();
        if (FAILED(hr)) {
            MessageBox(window->getHWND(), L"createDeviceAndSwapChain()�̎��s", nullptr, MB_OK);
            return E_FAIL;
        }
        
        hr = createRenderTargetView();
        if (FAILED(hr)) {
            MessageBox(window->getHWND(), L"createRenderTargetView()�̎��s", nullptr, MB_OK);
            return E_FAIL;
        }

        setRenderTargetView();

        return S_OK;
    }

    // �f�o�C�X�����_�[�^�[�Q�b�g�r���[�̍쐬
    HRESULT DirectX11::createDeviceAndSwapChain()
    {
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

        UINT windowWidth  = window->getWindowRect().right  - window->getWindowRect().left;
        UINT windowHeight = window->getWindowRect().bottom - window->getWindowRect().top;

        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;								   // �X���b�v�`�F�[���̃o�b�t�@�[��
        sd.BufferDesc.Width =  windowWidth;				   // �𑜓x�̕�
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
                0,                                     // �L���ɂ��郉���^�C�����C���[
                featureLevels,                         // D3D_FEATURE_LEVEL�̔z��ւ̃|�C���^�[
                numFeatureLevels,                      // ��L�z��̗v�f��
                D3D11_SDK_VERSION,                     // SDK�̃o�[�W����
                &sd,                                   // DXGI_SWAP_CHAIN�ւ̃|�C���^�[
                swapChain.GetAddressOf(),    // IDXGISwapChain�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
                device.GetAddressOf(),       // ID3D11Device�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
                &featureLevel,	                       // D3D_FEATURE_LEVEL�ւ̃|�C���^�[��Ԃ�
                deviceContext.GetAddressOf() // ID3D11DeviceContext�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
            );

            if (SUCCEEDED(hr)) {
                break;
            }
        }
        if (FAILED(hr)) {
            MessageBox(window->getHWND(), L"D3DCreateDeviceAndSwapChain()�̎��s", nullptr, MB_OK);
            return hr;
        }

        return S_OK;
    }

    // �����_�[�^�[�Q�b�g�r���[�̍쐬
    HRESULT DirectX11::createRenderTargetView()
    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

        // �o�b�N�o�b�t�@�̎擾
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*> (backBuffer.GetAddressOf()));
        // �����_�[�^�[�Q�b�g�r���[�̍쐬
        auto hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(window->getHWND(), L"CreateRenderTargetView()�̎��s", nullptr, MB_OK);
            return E_FAIL;
        }

        Microsoft::WRL::ComPtr<IDXGISurface> surf;
        hr = swapChain->GetBuffer(0, IID_PPV_ARGS(surf.GetAddressOf()));
        if (FAILED(hr)) {
            MessageBox(window->getHWND(), L"GetBuffer()�̎��s", nullptr, MB_OK);
            return E_FAIL;
        }

        return S_OK;
    }
    
    // �����_�[�^�[�Q�b�g�r���[�̃Z�b�g
    void DirectX11::setRenderTargetView()
    {
        deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

        UINT width  = window->getWindowRect().right  - window->getWindowRect().left;
        UINT height = window->getWindowRect().bottom - window->getWindowRect().top;

        // Viewport�̏�����
        D3D11_VIEWPORT vp;
        vp.Width    = static_cast<FLOAT>(width);  // �r���[�|�[�g������x�ʒu
        vp.Height   = static_cast<FLOAT>(height); // �r���[�|�[�g�㑤��y�ʒu
        vp.MinDepth = 0.0f;                       // �r���[�|�[�g��
        vp.MaxDepth = 1.0f;                       // �r���[�|�[�g�̍���
        vp.TopLeftX =    0;                       // �r���[�|�[�g�̍ŏ��[�x(0�`1)
        vp.TopLeftY =    0;                       // �r���[�|�[�g�̍ő�[�x(0�`1)
        deviceContext->RSSetViewports(1, &vp);
    }
}
