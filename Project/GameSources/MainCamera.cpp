/*!
@file MainCamera.cpp
@brief �J����
@prod ��I�t
*/

#include "stdafx.h"
#include "MathFunc.h"
#include "MainCamera.h"
#include "OzawaUtility.h"

namespace basecross {

	using namespace MathF;

	void MainCamera::OnCreate() {
		
	}

	void MainCamera::OnUpdate() {
		if (m_cameraState == State::Fixed) // �Œ���
		{
			SetEye(m_initialEye);
		}

		// ����ȍ~�̏����̓^�[�Q�b�g�I�u�W�F�N�g���Ȃ���΍s��Ȃ�
		if (GetTargetObject() == nullptr) return;

		m_targetPos = GetTargetObject()->GetComponent<Transform>()->GetPosition(); // �^�[�Q�b�g�̈ʒu���擾

		if (m_cameraState == State::Follow) // �ǔ����
		{
			FollowTarget();
		}
		if (m_cameraState == State::ZoomIn || m_cameraState == State::ZoomOut) // �Y�[�����
		{
			ZoomProcess();
		}
		if (m_cameraState == State::ZoomedIn) // �Y�[���C����
		{
			ZoomedInProcess();
		}
		if (m_cameraState == State::Scroll)
		{
			ScrollProcess();
		}
		Camera::OnUpdate();
	}

	void MainCamera::FollowTarget()
	{
		// �^�[�Q�b�g��X���ړ��̂ݒǔ�����(����ȊO�͌Œ�)
		Vec3 newEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialEye.y, m_initialEye.z);
		Vec3 newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialAt.y, m_initialAt.z);

		SetAt(newAt);
		SetEye(Utility::Lerp(newAt, newEye, m_ZoomRatioC));
	}

	void MainCamera::ZoomedInProcess()
	{
		SetAt(Vec3(m_targetPos.x, m_targetPos.y + m_zoomAtY, m_targetPos.z));
		SetEye(Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z));
	}

	void MainCamera::ZoomProcess()
	{
		m_zoomRatio = Clamp01(m_zoomRatio);
		// ���`��ԂŃY�[��������
		SetAt(Utility::Lerp(m_initialAt, Vec3(m_targetPos.x, m_targetPos.y + m_zoomAtY, m_targetPos.z), m_zoomRatio));
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z), m_zoomRatio));

		if (m_cameraState == State::ZoomIn)
		{
			if (m_zoomRatio >= 1.0f) m_cameraState = State::ZoomedIn;
			m_zoomRatio += DELTA_TIME * m_zoomSpeed;
		}
		if (m_cameraState == State::ZoomOut)
		{
			if (m_zoomRatio <= 0.0f) m_cameraState = m_DefaultState; // �Y�[�����I������珉����Ԃɖ߂�
			m_zoomRatio -= DELTA_TIME * m_zoomSpeed;
		}
	}

	void MainCamera::ScrollProcess()
	{
		//SetEye(m_stationPos);
		//SetAt(m_stationPos);
		//m_scrollRatio = Clamp01(m_scrollRatio);
		//Vec3 trainPos = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialEye.y, m_initialEye.z);
		//Vec3 stationPos = Vec3(Clamp(m_stationPos.x, m_MaxEye.x, m_initialEye.x), m_initialAt.y, m_initialAt.z);

		//SetAt(Utility::Lerp(stationPos, trainPos, m_scrollRatio));
		//SetEye(Utility::Lerp(stationPos, trainPos, m_scrollRatio));
		//if (m_scrollRatio >= 1.0f) m_cameraState = Scrolled;
		//m_scrollRatio += DELTA_TIME;
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