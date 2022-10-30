#include "stdafx.h"
#include "Window.h"
#include "DeadSpace.h"

DeadSpace deadspace;
Window::WindowClass Window::WindowClass::wndClass;

const TCHAR * Window::WindowClass::GetName() noexcept
{
	return wndCLassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInstance;
}

Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(deadspace.szDLLName))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndCLassName, GetInstance());
}

Window::Window(int width, int height, const TCHAR * name) noexcept
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	hWnd = CreateWindow(WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);

	//Show Window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

}

Window::~Window()
{
	DestroyWindow(hWnd);
}

HWND Window::GetWindowHandle()
{
	return hWnd;
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (Msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, Msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, Msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (Msg)
	{
		case WM_CREATE:
		{
			deadspace.CreateWindowControls(hWnd);
		}
		case WM_COMMAND:
		{
			deadspace.RunWindow(hWnd, wParam, lParam);
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			deadspace.CreateWindowGUI(hWnd, hdc);

			EndPaint(hWnd, &ps);
			return 0;
		}
		case WM_CLOSE:
		{
			deadspace.CloseWindowGUI(hWnd);
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
