#include "Main.h"

bool Main::Init()
{
    // initialise camera
    _cam = new Camera;
    _cam->CreateViewMatrix(Vector(0, 3.8, -13), Vector(0, 0, 0), Vector(0, 1, 0));
    _cam->CreateProjMatrix(1.0f, 10000.0f, PI * 0.25f,
        (float)g_rectClient.right / (float)g_rectClient.bottom);

	BuildEnvironment();

    return true;
}

void Main::BuildEnvironment()
{
	// build floor
	int numCols = 1000;
	int numRows = 1000;
	_floor._vertices.resize(numCols * numRows);

	for (UINT row = 0; row < numRows; row++)
	{
		for (UINT col = 0; col < numCols; col++)
		{
			_floor._vertices[row * numCols + col].p = {
				(float)(col - float(numCols / 2.0)) * 1,
				0,
				(float)(float(numRows / 2.0) - row) * 1
			};
			_floor._vertices[row * numCols + col].c = { 1, 1, 1, 1 };
			_floor._vertices[row * numCols + col].t = {
				((float)col / (float)(numCols - 1)) * (float)(numCols - 1),
				((float)row / (float)(numRows - 1)) * (float)(numRows - 1)
			};
		}
	}

	_floor._indices.resize((numCols - 1) * (numRows - 1) * 2.0f * 3.0f);
	UINT idx = 0;
	for (UINT row = 0; row < numRows - 1; row++)
	{
		for (UINT col = 0; col < numCols - 1; col++)
		{
			UINT newRow = row + 1;

			_floor._indices[idx + 0] = row * numCols + col;
			_floor._indices[idx + 1] = _floor._indices[idx + 0] + 1;
			_floor._indices[idx + 2] = newRow * numCols + col;

			_floor._indices[idx + 3] = _floor._indices[idx + 2];
			_floor._indices[idx + 4] = _floor._indices[idx + 1];
			_floor._indices[idx + 5] = _floor._indices[idx + 3] + 1;

			idx += 6;
		}
	}

	_floor.Create(
		_device, _immediateContext,
		L"../../data/shader/default3DShader.txt",
		L"../../data/shader/default3DShader.txt",
		L"VSMain", L"PSTexture",
		L"../../data/image/floor.jpg"
	);
}

bool Main::Frame()
{
	return true;
}

bool Main::Render()
{
	_floor.SetTransformationMatrix(nullptr, &_cam->_viewMat, &_cam->_projMat);
	if (_renderFloor) _floor.Render();
    return true;
}

bool Main::Release()
{
	_floor.Release();
    return true;
}

// main function
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Main coreMain;
    coreMain.SetWindow(hInstance, L"ParticleTester", 800, 600);
    coreMain.Run();

    return 1;
}