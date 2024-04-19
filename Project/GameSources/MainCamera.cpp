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
		// ターゲットがオブジェクトがなければ処理を行わない
		if (GetTargetObject() == nullptr) return;

		m_targetPos = GetTargetObject()->GetComponent<Transform>()->GetPosition(); // ターゲットの位置を取得

		// 状態ごとの処理
		if (m_cameraState == Fixed)
		{
			SetEye(m_DefaultPos);
		}
		else if (m_cameraState == Follow)
		{
			FollowTarget();
		}
		else if (m_cameraState == Zoom)
		{
			ZoomInProcess(); 
		}
		Camera::OnUpdate();
	}

	void MainCamera::FollowTarget()
	{
		Vec3 newEye = Vec3(m_targetPos.x, m_DefaultPos.y, m_DefaultPos.z);
		Vec3 newAt = m_targetPos;

		SetAt(newAt);
		SetEye(newEye);
	}

	void MainCamera::ZoomInProcess()
	{
		// 線形補間でズームさせる
		SetEye(Utility::Lerp(m_currentEye, m_targetPos, m_zoomRatio));
		m_zoomRatio += DELTA_TIME * m_zoomSpeed;

		// ズーム処理が終わったら
		if (m_zoomRatio >= 1.0f)
		{
			// 値を初期化してカメラの状態をもとに戻す
			m_zoomRatio = 0.0f;
			m_cameraState = m_defaultState;
		}
	}
}