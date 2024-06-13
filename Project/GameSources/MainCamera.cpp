/*!
@file MainCamera.cpp
@brief �J����
@prod ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	using namespace MathF;

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
		Vec3 newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_DefaultEye.x), m_DefaultAt.y, m_DefaultAt.z);

		SetAt(newAt);
		SetEye(newEye);
	}

	void MainCamera::ZoomInProcess()
	{
		m_zoomRatio = Clamp01(m_zoomRatio);
		// ���`��ԂŃY�[��������
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z), m_zoomRatio));
		SetAt(Utility::Lerp(m_DefaultAt, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z), m_zoomRatio));
		m_zoomRatio += DELTA_TIME * m_zoomSpeed;
		//m_zoomRatio = Repeat01(m_zoomRatio, m_zoomSpeed);
	}
}