/*!
@file MainCamera.cpp
@brief カメラ
@prod 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void MainCamera::OnCreate() {
		
	}

	void MainCamera::OnUpdate() {
		if (GetTargetObject() == nullptr) return;

		m_targetPos = GetTargetObject()->GetComponent<Transform>()->GetPosition();

		Vec3 newEye = Vec3(m_targetPos.x, m_cameraArm.y, m_cameraArm.z);
		Vec3 newAt = m_targetPos;

		SetAt(newAt);
		SetEye(newEye);
		Camera::OnUpdate();
	}

	void MainCamera::ZoomIn(bool zoomFlag, Vec3 currentEye)
	{
		if (!zoomFlag) return; // ズームフラグがfalseなら何もしない

		// 線形補間でズームさせる
		Utility::Lerp(currentEye, m_targetPos, m_zoomRatio);
		m_zoomRatio += DELTA_TIME * m_zoomSpeed;

		// ズーム処理が終わったら
		if (m_zoomRatio >= 1.0f)
		{
			// 値を初期化してフラグをfalseに
			m_zoomRatio = 0.0f;
			zoomFlag = false; 
		}
	}
}