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
		if (m_cameraState == Zoom) // ズーム状態
		{
			ZoomInProcess(); 
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

	void MainCamera::ZoomInProcess()
	{
		m_zoomRatio = Clamp01(m_zoomRatio);
		// 線形補間でズームさせる
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z), m_zoomRatio));
		SetAt(Utility::Lerp(m_DefaultAt, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z), m_zoomRatio));
		m_zoomRatio += DELTA_TIME * m_zoomSpeed;
		//m_zoomRatio = Repeat01(m_zoomRatio, m_zoomSpeed);
	}
}