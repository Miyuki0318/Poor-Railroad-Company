/*!
@file MainCamera.cpp
@brief �J����
@prod ��I�t
*/

#include "stdafx.h"
#include "MathFunc.h"
#include "MainCamera.h"
#include "OzawaUtility.h"
#include "DebugLog.h"

namespace basecross {

	using namespace MathF;

	void MainCamera::OnCreate() {

	}

	void MainCamera::OnUpdate() {
		if (m_cameraState == State::Fixed) // �Œ���
		{
			SetEye(m_initialEye);
		}
		Debug::Log("��� : ", (int)m_cameraState);

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
		if (m_cameraState == State::Scroll) // �X�N���[�����
		{
			ScrollProcess();
		}
		Camera::OnUpdate();
	}

	void MainCamera::FollowTarget()
	{
		Vec3 newEye, newAt;
		if (!GetScrollEnd()) // �X�N���[�����I����Ă��Ȃ��Ȃ�
		{
			// �X�N���[���O�̈ʒu�ŌŒ肷��
			newEye = Vec3(m_defScrollEye.x, m_initialEye.y, m_initialEye.z);
			newAt = Vec3(m_defScrollEye.x, m_initialAt.y, m_initialAt.z);
		}
		else // �I����Ă�����
		{
			// �^�[�Q�b�g��X���ړ��̂ݒǔ�����(����ȊO�͌Œ�)
			newEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialEye.y, m_initialEye.z);
			newAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialAt.y, m_initialAt.z);
		}
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
		SetAt(Utility::Lerp(m_currentAt, Vec3(m_targetPos.x, m_targetPos.y + m_zoomAtY, m_targetPos.z), m_zoomRatio));
		SetEye(Utility::Lerp(m_currentEye, Vec3(m_targetPos.x, m_targetPos.y + m_zoomEye.y, m_targetPos.z + m_zoomEye.z), m_zoomRatio));

		if (m_cameraState == State::ZoomIn)
		{
			if (m_zoomRatio >= 1.0f) m_cameraState = State::ZoomedIn; // �Y�[�����I�������Y�[����̏�ԂɈڍs
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
		// �X�N���[���̊J�n�ʒu
		Vec3 startAt = Vec3(m_defScrollEye.x, m_initialAt.y, m_initialAt.z);
		Vec3 startEye = Utility::Lerp(startAt, Vec3(m_defScrollEye.x, m_initialEye.y, m_initialEye.z), m_ZoomRatioC);

		// �X�N���[���̏I���ʒu
		Vec3 endAt = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialAt.y, m_initialAt.z);
		Vec3 targetEye = Vec3(Clamp(m_targetPos.x, m_MaxEye.x, m_initialEye.x), m_initialEye.y, m_initialEye.z);
		Vec3 endEye = Utility::Lerp(endAt, targetEye, m_ZoomRatioC);

		// �ʒu�ƒ����_�̍X�V
		SetEye(Utility::Lerp(startEye, endEye, m_scrollRatio));
		SetAt(Utility::Lerp(startAt, endAt, m_scrollRatio));

		m_scrollRatio = Clamp01(m_scrollRatio);

		if (GetScrollEnd()) m_cameraState = m_DefaultState; // �X�N���[�����I������珉����ԂɈڍs
		m_scrollRatio += DELTA_TIME * m_ScrollSpeed;
	}

	void MainCamera::ResetCamera(Vec3 defEyePos, Vec3 startEyePos, Vec3 atPos)
	{
		m_cameraState = m_DefaultState;
		m_defScrollEye = defEyePos;
		m_initialEye = startEyePos;
		m_initialAt = atPos;
		m_scrollRatio = 0.0f;
		SetEye(defEyePos);
		SetAt(atPos);
	}
}