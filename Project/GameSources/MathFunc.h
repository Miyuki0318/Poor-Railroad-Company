/*!
@file MathFunc.h
@brief ���w�֐��܂Ƃ�
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#define BASECROSS __has_include("common.h")
using namespace basecross;
/// <summary>
/// �l�ƃt���O��ۑ�����\����
/// </summary>
struct ValueFlag {
	float value = 0;
	bool outRange = false;
};

namespace MathFuncs {
#define ELAPSED_TIME App::GetApp()->GetElapsedTime()
#define Infinity = 0xFFFFFFFFFFFFFFFF;
#define Deg2Rad = (XM_PI * 2.0f) / 360.0f
#define Rad2Deg = 360.0f / (XM_PI * 2.0f)

	//static ValueFlag vf;
	/// <summary>
	/// �^����ꂽvalue��max����min�͈̔͂ɐ�������֐�(float�^)
	/// </summary>
	/// <param name="value">��������l</param>
	/// <param name="max">����l</param>
	/// <param name="min">�����l</param>
	/// <returns>�␳��̒l</returns>
	inline float Clamp(float value, float max, float min)
	{
		value = value > max ? max : value;
		value = value < min ? min : value;
		return value;
	}

	/// <summary>
	/// �^����ꂽvalue��0����1�͈̔͂ɐ�������֐�(float�^)
	/// </summary>
	/// <param name="value">��������l</param>
	/// <returns>�␳��̒l</returns>
	inline float Clamp01(float value)
	{
		value = value > 1.0f ? 1.0f : value;
		value = value < 0.0f ? 0.0f : value;
		return value;
	}

	/// <summary>
	/// �^����ꂽvalue��0����1(�܂���1����0)�͈̔͂Ń��[�v������֐�
	/// </summary>
	/// <param name="value">���[�v������l</param>
	/// <param name="invert">false:0����1 true:1����0</param>
	/// <returns>value:�␳��̒l flag:0~1�͈̔͊O�ɂȂ�����true</returns>
	inline ValueFlag Repeat01(float value, float speed, bool invert)
	{
		ValueFlag vf;
		value += (invert ? -ELAPSED_TIME : ELAPSED_TIME) * speed;

		vf.outRange = value > 1.0f || value < 0.0f;

		value = value > 1.0f ? 0.0f : value;
		value = value < 0.0f ? 1.0f : value;
		vf.value = value;
		return vf;
	}
}