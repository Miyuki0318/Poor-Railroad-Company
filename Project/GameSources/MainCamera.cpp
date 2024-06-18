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
		if (m_cameraState == Fixed) // 固定状態
		{
			SetEye(Utility::Lerp(m_DefaultAt, m_DefaultEye, m_ZoomRatioC));
		}

		// これ以降の処理はターゲットオブジェクトがなければ行わない
		if (GetTargetObject() == nullptr) return;

		m_targetPos = GetTargetObject()->GetComponent<Transform>()->GetPosition(); // ターゲットの位置を取得

		if (m_cameraState == Follow) // 追尾状態
		{
			FollowTarget();
		}
		if (m_cameraState == ZoomIn || m_cameraState == ZoomOut) // ズーム状態
		{
			ZoomProcess(); 
		}
		Camera::OnUpdate();
	}

	void MainCamera::FollowTarget()
	{
		Vec3 newEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_DefaultEye.x), m_DefaultEye.y, m_DefaultEye.z);
		Vec3 newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_DefaultEye.x), m_DefaultAt.y, m_DefaultAt.z);

		SetAt(newAt);
		SetEye(Utility::Lerp(newAt, newEye, m_ZoomRatioC));
	}

	void MainCamera::ZoomProcess()
	{
		m_zoomRatio = Clamp01(m_zoomRatio);
		// 線形補間でズームさせる
		SetAt(Utility::Lerp(m_DefaultAt, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z), m_zoomRatio));
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z), m_zoomRatio));

		if (m_cameraState == State::ZoomIn)
		{
			m_zoomRatio += DELTA_TIME * m_zoomSpeed;
		}
		if (m_cameraState == State::ZoomOut)
		{
			if (m_zoomRatio <= 0.0f) m_cameraState = State::Fixed;
			m_zoomRatio -= DELTA_TIME * m_zoomSpeed;
		}
	}
}