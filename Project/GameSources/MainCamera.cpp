/*!
@file MainCamera.cpp
@brief カメラ
@prod 矢吹悠葉
*/

#include "stdafx.h"
#include "MathFunc.h"
#include "MainCamera.h"
#include "OzawaUtility.h"

namespace basecross {

	using namespace MathF;

	void MainCamera::OnCreate() {

	}

	void MainCamera::OnUpdate() {
		if (m_cameraState == State::Fixed) // 固定状態
		{
			SetEye(m_initialEye);
		}

		// これ以降の処理はターゲットオブジェクトがなければ行わない
		if (GetTargetObject() == nullptr) return;

		m_targetPos = GetTargetObject()->GetComponent<Transform>()->GetPosition(); // ターゲットの位置を取得

		if (m_cameraState == State::Follow) // 追尾状態
		{
			FollowTarget();
		}
		if (m_cameraState == State::ZoomIn || m_cameraState == State::ZoomOut) // ズーム状態
		{
			ZoomProcess();
		}
		if (m_cameraState == State::ZoomedIn) // ズームイン後
		{
			ZoomedInProcess();
		}
		if (m_cameraState == State::Scroll) // スクロール状態
		{
			ScrollProcess();
		}
		Camera::OnUpdate();
	}

	void MainCamera::FollowTarget()
	{
		Vec3 newEye, newAt;
		if (!GetScrollEnd()) // スクロールが終わっていないなら
		{
			// スクロール前の位置で固定する
			newEye = Vec3(m_defScrollEye.x, m_initialEye.y, m_initialEye.z);
			newAt = Vec3(m_defScrollEye.x, m_initialAt.y, m_initialAt.z);
		}
		else // 終わっていたら
		{
			// ターゲットのX軸移動のみ追尾する(それ以外は固定)
			newEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialEye.y, m_initialEye.z);
			newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialAt.y, m_initialAt.z);
		}
		SetAt(newAt);
		SetEye(Utility::Lerp(newAt, newEye, m_ZoomRatioC));
	}

	void MainCamera::ZoomedInProcess()
	{
		SetAt(Vec3(m_targetPos.x, m_targetPos.y + m_zoomAtY, m_targetPos.z));
		SetEye(Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z));
	}

	void MainCamera::ZoomProcess()
	{
		m_zoomRatio = Clamp01(m_zoomRatio);
		// 線形補間でズームさせる
		SetAt(Utility::Lerp(m_initialAt, Vec3(m_targetPos.x, m_targetPos.y + m_zoomAtY, m_targetPos.z), m_zoomRatio));
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z), m_zoomRatio));

		if (m_cameraState == State::ZoomIn)
		{
			if (m_zoomRatio >= 1.0f) m_cameraState = State::ZoomedIn;
			m_zoomRatio += DELTA_TIME * m_zoomSpeed;
		}
		if (m_cameraState == State::ZoomOut)
		{
			if (m_zoomRatio <= 0.0f) m_cameraState = m_DefaultState; // ズームが終わったら初期状態に戻る
			m_zoomRatio -= DELTA_TIME * m_zoomSpeed;
		}
	}

	void MainCamera::ScrollProcess()
	{
		// スクロールの開始位置
		Vec3 startAt = Vec3(m_defScrollEye.x, m_initialAt.y, m_initialAt.z);
		Vec3 startEye = Utility::Lerp(startAt, Vec3(m_defScrollEye.x, m_initialEye.y, m_initialEye.z), m_ZoomRatioC);

		// スクロールの終了位置
		Vec3 endAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialAt.y, m_initialAt.z);
		Vec3 targetEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialEye.y, m_initialEye.z);
		Vec3 endEye = Utility::Lerp(endAt, targetEye, m_ZoomRatioC);

		// 位置と注視点の更新
		SetEye(Utility::Lerp(startEye, endEye, m_scrollRatio));
		SetAt(Utility::Lerp(startAt, endAt, m_scrollRatio));
		m_scrollRatio = Clamp01(m_scrollRatio);

		if (GetScrollEnd()) m_cameraState = m_DefaultState; // スクロールが終わったら初期状態に移行
		m_scrollRatio += DELTA_TIME * m_ScrollSpeed;
	}

	void MainCamera::ResetCamera(Vec3 defEyePos, Vec3 startEyePos, Vec3 atPos)
	{
		m_cameraState = m_DefaultState;
		m_defScrollEye = defEyePos;
		m_initialEye = startEyePos;
		m_initialAt = atPos;
		m_scrollRatio = 0.0f;
		SetEye(defEyePos);
		SetAt(atPos);
	}
}