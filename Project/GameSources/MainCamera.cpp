/*!
@file MainCamera.cpp
@brief カメラ
@prod 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	using namespace MathF;

	void MainCamera::OnCreate() {
		
	}

	void MainCamera::OnUpdate() {
		if (m_cameraState == State::Fixed) // 固定状態
		{
			SetEye(Utility::Lerp(m_initialAt, m_initialEye, m_ZoomRatioC));
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
		if (m_cameraState == State::Zoomed)
		{
			ZoomedFollowTarget();
		}
		Camera::OnUpdate();
	}

	void MainCamera::FollowTarget()
	{
		// ターゲットのX軸移動のみ追尾する(それ以外は固定)
		Vec3 newEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialEye.y, m_initialEye.z);
		Vec3 newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialAt.y, m_initialAt.z);

		SetAt(newAt);
		SetEye(Utility::Lerp(newAt, newEye, m_ZoomRatioC));
	}

	void MainCamera::ZoomedFollowTarget()
	{
		SetAt(Utility::Lerp(m_initialAt, Vec3(m_targetPos.x, m_targetPos.y, m_targetPos.z), 1.0f));
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z), 1.0f));
	}

	void MainCamera::ZoomProcess()
	{
		m_zoomRatio = Clamp01(m_zoomRatio);
		// 線形補間でズームさせる
		SetAt(Utility::Lerp(m_initialAt, Vec3(m_targetPos.x, m_targetPos.y, m_targetPos.z), m_zoomRatio));
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z), m_zoomRatio));

		if (m_cameraState == State::ZoomIn)
		{
			if (m_zoomRatio >= 1.0f) m_cameraState = State::Zoomed;
			m_zoomRatio += DELTA_TIME * m_zoomSpeed;
		}
		if (m_cameraState == State::ZoomOut)
		{
			if (m_zoomRatio <= 0.0f) m_cameraState = m_DefaultState; // ズームが終わったら初期状態に戻る
			m_zoomRatio -= DELTA_TIME * m_zoomSpeed;
		}
	}

	void MainCamera::ResetCamera(Vec3 eyePos, Vec3 atPos)
	{
		m_cameraState = m_DefaultState;
		m_initialEye = eyePos;
		m_initialAt = atPos;
		SetEye(eyePos);
		SetAt(atPos);
	}
}