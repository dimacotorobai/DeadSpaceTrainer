#pragma once
void KeyDown(BYTE vk)
{
	return keybd_event(
		vk,
		MapVirtualKey(vk, MAPVK_VK_TO_VSC),
		KEYEVENTF_EXTENDEDKEY | 0,
		0
	);
}
void KeyUp(BYTE vk)
{
	return keybd_event(
		vk,
		MapVirtualKey(vk, MAPVK_VK_TO_VSC),
		KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
		0
	);
}