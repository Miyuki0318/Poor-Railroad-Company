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
		if (m_cameraState == Fixed) // �Œ���
		{
			SetEye(Utility::Lerp(m_initializeAt, m_DefaultEye, m_ZoomRatioC));
		}

		// ����ȍ~�̏����̓^�[�Q�b�g�I�u�W�F�N�g���Ȃ���΍s��Ȃ�
		if (GetTargetObject() == nullptr) return;

		m_targetPos = GetTargetObject()->GetComponent<Transform>()->GetPosition(); // �^�[�Q�b�g�̈ʒu���擾

		if (m_cameraState == Follow) // �ǔ����
		{
			FollowTarget();
		}
		if (m_cameraState == ZoomIn || m_cameraState == ZoomOut) // �Y�[�����
		{
			ZoomProcess();
		}
		Camera::OnUpdate();
	}

	void MainCamera::FollowTarget()
	{
		Vec3 newEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_DefaultEye.x), m_DefaultEye.y, m_DefaultEye.z);
		Vec3 newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_DefaultEye.x), m_initializeAt.y, m_initializeAt.z);

		SetAt(newAt);
		SetEye(Utility::Lerp(newAt, newEye, m_ZoomRatioC));
	}

	void MainCamera::ZoomProcess()
	{
		m_zoomRatio = Clamp01(m_zoomRatio);
		// ���`��ԂŃY�[��������
		SetAt(Utility::Lerp(m_initializeAt, Vec3(m_targetPos.x, m_targetPos.y, m_targetPos.z), m_zoomRatio));
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