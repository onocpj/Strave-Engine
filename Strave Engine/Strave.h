#pragma once

#include "Core.h"

#if defined(SV_PLATFORM_WINDOWS)

// -----SFML API--------------	
	#include "SFML/Graphics.hpp"
// ---------------------------

// -----Strave Engine---------
	#include "Texture.h"
	#include "Thread.h"
	#include "UserInterface.h"
	#include "CharacterController2D.h"
	#include "Input.h"
	#include "Player2D.h"
	#include "RigidBody2D.h"
	#include "GameObject2D.h"
	#include "WindowsWindow.h"
	#include "GameCamera2D.h"
	#include "HudCamera2D.h"
	#include "Application.h"
	#include "Log.h"
	#include "Convert.h"
	#include "Keyboard.h"
	#include "Mouse.h"
	#include "Animation.h"
	#include "Additive.h"
	#include "Math.h"
	#include "DT.h"
	#include "UDT.h"
	#include "Shapes.h"
// ---------------------------

// -----Entry Point-----------
#include "EntryPoint.h"
// ---------------------------

#endif // SV_PLATFORM_WINDOWS

