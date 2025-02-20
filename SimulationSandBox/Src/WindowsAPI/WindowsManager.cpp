#include "WindowsAPI/WindowsManager.h"
#include "Info/ErrorCodes.h"


WindowsManager::WindowsManager(const std::wstring& windowName, unsigned int width, unsigned int height)
	: mhInstance(GetModuleHandle(nullptr)), mHandleWindow(nullptr), mWindowName(windowName), mWidth(width), mHeight(height)
{}

WindowsManager::~WindowsManager()
{
	Shutdown();
}

void WindowsManager::Initialize()
{
	WNDCLASSEX wnd{};
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(wnd);
	wnd.cbWndExtra = 0;
	wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wnd.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wnd.hInstance = mhInstance;
	wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wnd.lpfnWndProc = HandleMsgSetup;
	wnd.lpszClassName = L"WindowsManager";
	wnd.lpszMenuName = nullptr;
	wnd.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wnd)) {
		MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	mHandleWindow = CreateWindowEx(
		0,             
		L"WindowsManager",
		GetWindowName().c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		mWidth, mHeight,
		nullptr, nullptr,
		mhInstance, nullptr
	);

	if (!mHandleWindow) {
		MessageBox(nullptr, L"Failed to create window!", L"Error", MB_ICONERROR);
		return;
	}

	ShowWindow(GetHandleWindow(), SW_SHOW);
	UpdateWindow(GetHandleWindow());

	ImGui_ImplWin32_Init(mHandleWindow);
}

std::optional<int> WindowsManager::ProcessMessages() noexcept
{
	ImGui_ImplWin32_NewFrame();
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return static_cast<int>(msg.wParam);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

void WindowsManager::Shutdown()
{
	DestroyWindow(mHandleWindow);
}

HINSTANCE WindowsManager::GetHandleInstance() const
{
	return mhInstance;
}

HWND WindowsManager::GetHandleWindow() const
{
	return mHandleWindow;
}

std::wstring WindowsManager::GetWindowName() const
{
	return mWindowName;
}

unsigned int WindowsManager::GetWidth() const
{
	return mWidth;
}

unsigned int WindowsManager::GetHeight() const
{
	return mHeight;
}

LRESULT WindowsManager::HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	if (message == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowsManager* const pWnd = static_cast<WindowsManager*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsManager::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT WindowsManager::HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	// Retrieve pointer and forward message to window class
	WindowsManager* const pWnd = reinterpret_cast<WindowsManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, message, wParam, lParam);
}

LRESULT WindowsManager::HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
