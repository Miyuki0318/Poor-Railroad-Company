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
		// ターゲットがオブジェクトがなければ処理を行わない
		if (GetTargetObject() == nullptr) return;

		m_targetPos = GetTargetObject()->GetComponent<Transform>()->GetPosition(); // ターゲットの位置を取得

		// 状態ごとの処理
		if (m_cameraState == Fixed)
		{
			SetEye(m_DefaultEye);
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
		Vec3 newEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_DefaultEye.x), m_DefaultEye.y, m_DefaultEye.z);
		Vec3 newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_DefaultEye.x), m_DefaultAt.y, m_DefaultAt.z);

		SetAt(newAt);
		SetEye(newEye);
	}

	void MainCamera::ZoomInProcess()
	{
		m_zoomRatio = Clamp01(m_zoomRatio);
		// 線形補間でズームさせる
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_targetPos.getY(), m_targetPos.z + m_targetPos.getZ()), m_zoomRatio));
		SetAt(Utility::Lerp(m_DefaultAt, Vec3(m_targetPos.x, m_targetPos.y + m_targetPos.getY(), m_targetPos.z), m_zoomRatio));
		m_zoomRatio += DELTA_TIME * m_zoomSpeed;
		//m_zoomRatio = Repeat01(m_zoomRatio, m_zoomSpeed);

		// ズーム処理が終わったら
		if (m_zoomRatio >= 1.0f)
		{
			// 値を初期化してカメラの状態をもとに戻す
			//m_cameraState = m_DefaultState;
		}
	}
}