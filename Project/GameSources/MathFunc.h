/*!
@file MathFunc.h
@brief 数学関数まとめ
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#define BASECROSS __has_include("common.h")
using namespace basecross;
/// <summary>
/// 値とフラグを保存する構造体
/// </summary>
struct ValueFlag {
	float value = 0;
	bool outRange = false;
};

namespace MathF {
#define ELAPSED_TIME App::GetApp()->GetElapsedTime()
#define Infinity = 0xFFFFFFFFFFFFFFFF;
#define Deg2Rad = (XM_PI * 2.0f) / 360.0f
#define Rad2Deg = 360.0f / (XM_PI * 2.0f)

	/// <summary>
	/// 与えられたvalueをmaxからminの範囲に制限する関数(float型)
	/// </summary>
	/// <param name="value">制限する値</param>
	/// <param name="max">上限値</param>
	/// <param name="min">下限値</param>
	/// <returns>補正後の値</returns>
	inline float Clamp(float value, float max, float min)
	{
		value = value > max ? max : value;
		value = value < min ? min : value;
		return value;
	}

	/// <summary>
	/// 与えられたvalueを0から1の範囲に制限する関数(float型)
	/// </summary>
	/// <param name="value">制限する値</param>
	/// <returns>補正後の値</returns>
	inline float Clamp01(float value)
	{
		value = value > 1.0f ? 1.0f : value;
		value = value < 0.0f ? 0.0f : value;
		return value;
	}

	/// <summary>
	/// 三次元線形補間
	/// </summary>
	/// <param name="start">開始位置</param>
	/// <param name="end">終了位置</param>
	/// <param name="time">時間</param>
	/// <returns>補間後の値</returns>
	inline Vec3 Lerp3D(Vec3 start, Vec3 end, float time)
	{
		time = Clamp01(time);
		return start + (end - start) * time;
	}

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">時間</param>
	/// <returns>補間後の値</returns>
	inline float Lerp(float start, float end, float time)
	{
		time = Clamp01(time);
		return start + (end - start) * time;
	}

	/// <summary>
	/// 与えられたvalueを0から1(または1から0)の範囲でループさせる関数
	/// </summary>
	/// <param name="value">ループさせる値</param>
	/// <param name="invert">false:0から1 true:1から0</param>
	/// <returns>value:補正後の値 flag:0~1の範囲外になったらtrue</returns>
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