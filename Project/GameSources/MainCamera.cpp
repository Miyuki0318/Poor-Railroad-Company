/*!
@file MainCamera.cpp
@brief �J����
@prod ��I�t
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
		if (!zoomFlag) return; // �Y�[���t���O��false�Ȃ牽�����Ȃ�

		// ���`��ԂŃY�[��������
		Utility::Lerp(currentEye, m_targetPos, m_zoomRatio);
		m_zoomRatio += DELTA_TIME * m_zoomSpeed;

		// �Y�[���������I�������
		if (m_zoomRatio >= 1.0f)
		{
			// �l�����������ăt���O��false��
			m_zoomRatio = 0.0f;
			zoomFlag = false; 
		}
	}
}