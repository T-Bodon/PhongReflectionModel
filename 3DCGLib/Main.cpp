#include <Windows.h>
#include "Window.h"
#include "DirectX11.h"
#include "Model.h"
#include "Matrix.h"
#include "MathConstantValues.h"

using namespace Lib;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    auto w = std::make_shared<Lib::Window>(L"3DCGLib", 1026, 768);

    auto& directX = Lib::DirectX11::getInstance();
    directX.initDevice(w);

    // ViewMatrix�̏�����
    Vector3 eye = Vector3(0.0f, 1.0f, -5.0f); // �J�����̍��W
    Vector3 at  = Vector3(0.0f, 1.0f,  0.0f); // �����Ώ�
    Vector3 up  = Vector3(0.0f, 1.0f,  0.0f); // ���݂̃��[���h���W�̏����
    auto view   = Matrix::LookAtLH(eye, at, up);
    directX.setViewMatrix(view);

    // ProjectionMatrix�̏�����
    LONG windowWidth  = w->getWindowRect().right  - w->getWindowRect().left;
    LONG windowHeight = w->getWindowRect().bottom - w->getWindowRect().top;
    auto projection   = Matrix::perspectiveFovLH(PIDIV2, windowWidth / static_cast<float>(windowHeight), 0.01f, 100.0f);
    directX.setProjectionMatrix(projection);
    
    Model model = Model();
    Matrix world;
    world = Matrix::Identify;

    while (w->Update().message != WM_QUIT) {
        directX.begineFrame();

        // ��]
        static float t = 0.0f;
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;

        auto mtr = Matrix::rotateY(t);
        auto mtt = Matrix::translate(Vector3(0.0f, -1.0f, 0.0f));
        world = mtr * mtt;

        model.setWorldMatrix(world);
        model.render(Lib::Color(Lib::Color::BLUE));

        directX.endFrame();
    }

    return 0;
}