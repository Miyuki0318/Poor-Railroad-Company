/*!
@file MathFunc.h
@brief ���w�֐��܂Ƃ�
@author ��I�t
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
}