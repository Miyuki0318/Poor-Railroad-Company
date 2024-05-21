/*!
@file MathFunc.h
@brief ”ŠwŠÖ”‚Ü‚Æ‚ß
@author –î—I—t
*/

#pragma once
#include "stdafx.h"
#define BASECROSS __has_include("common.h")

namespace MathFuncs {
	inline float ClampF(float value, float max, float min)
	{
		value = value > max ? max : value;
		value = value < min ? min : value;
		return value;
	}

	inline float Clamp01(float value)
	{
		value = value > 1.0f ? 1.0f : value;
		value = value < 0.0f ? 0.0f : value;
		return value;
	}
}