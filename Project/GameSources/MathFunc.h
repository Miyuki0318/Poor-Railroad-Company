/*!
@file MathFunc.h
@brief ���w�֐��܂Ƃ�
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#define BASECROSS __has_include("common.h")
using namespace basecross;

namespace MathF {
#define ELAPSED_TIME App::GetApp()->GetElapsedTime()
#define Infinity = 0xFFFFFFFFFFFFFFFF;
#define Deg2Rad = (XM_PI * 2.0f) / 360.0f
#define Rad2Deg = 360.0f / (XM_PI * 2.0f)

	static bool outRange = false;

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
	/// �O�������`���
	/// </summary>
	/// <param name="start">�J�n�ʒu</param>
	/// <param name="end">�I���ʒu</param>
	/// <param name="time">����</param>
	/// <returns>��Ԍ�̒l</returns>
	inline Vec3 Lerp3D(Vec3 start, Vec3 end, float time)
	{
		time = Clamp01(time);
		return start + (end - start) * time;
	}

	/// <summary>
	/// ���`���
	/// </summary>
	/// <param name="start">�J�n�l</param>
	/// <param name="end">�I���l</param>
	/// <param name="time">����</param>
	/// <returns>��Ԍ�̒l</returns>
	inline float Lerp(float start, float end, float time)
	{
		time = Clamp01(time);
		return start + (end - start) * time;
	}

	/// <summary>
	/// �^����ꂽvalue��0����1(�܂���1����0)�͈̔͂Ń��[�v������֐�
	/// </summary>
	/// <param name="value">���[�v������l</param>
	/// <param name="speed">���[�v���x(�b��)</param>
	/// <returns>�␳��̒l</returns>
	inline float Repeat01(float value, float speed)
	{
		value += ELAPSED_TIME * speed;
		outRange = value >= 1.0f;
		if (outRange) value = 0.0f;

		return value;
	}

	/// <summary>
	/// repeat�֐��Ő��l���͈͊O�ɂȂ������ǂ���
	/// </summary>
	/// <returns></returns>
	inline bool GetOutRange()
	{
		return outRange;
	}
}