#pragma once

#include "window-class.h"
#include "input-device.h"

using namespace Win;
using namespace Device;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

LPDIRECTINPUT8 Device::Joystick::DirectInput8;
LPDIRECTINPUTDEVICE8 Device::Joystick::DirectInputDevice8;
bool Win::Window::isWindowActive;