/*!
@file MainCamera.cpp
@brief �J����
@prod ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	using namespace MathFuncs;

	void MainCamera::OnCreate() {
		
	}

	void MainCamera::OnUpdate() {
		// �^�[�Q�b�g���I�u�W�F�N�g���Ȃ���Ώ������s��Ȃ�
		if (GetTargetObject() == nullptr) return;

		m_targetPos = GetTargetObject()->GetComponent<Transform>()->GetPosition(); // �^�[�Q�b�g�̈ʒu���擾

		// ��Ԃ��Ƃ̏���
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
		Vec3 newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_DefaultEye.x), m_targetPos.y, m_targetPos.z);

		SetAt(newAt);
		SetEye(newEye);
	}

	void MainCamera::ZoomInProcess()
	{
		// ���`��ԂŃY�[��������
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y, m_targetPos.z - 1.0f), m_zoomRatio));
		SetAt(Utility::Lerp(m_DefaultAt, m_targetPos, m_zoomRatio));
		m_zoomRatio += DELTA_TIME * m_zoomSpeed;

		// �Y�[���������I�������
		if (m_zoomRatio >= 1.0f)
		{
			// �l�����������ăJ�����̏�Ԃ����Ƃɖ߂�
			m_zoomRatio = 0.0f;
			m_cameraState = m_defaultState;
		}
	}
}