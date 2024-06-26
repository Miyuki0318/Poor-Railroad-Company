/*!
@file MainCamera.h
@brief �J����
@prod ��I�t
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MainCamera : public Camera {
		const Vec3 m_MaxEye;		// �J�����̍ő�ړ�����
		const float m_ZoomRatioC;	// �Œ��Ԏ��̃Y�[������

		weak_ptr<GameObject> m_targetObject; // �ڕW�ƂȂ�I�u�W�F�N�g
		shared_ptr<Transform> m_targetTrans; // �ڕW�̃g�����X�t�H�[��

		Vec3 m_targetPos;	// �^�[�Q�b�g�̈ʒu
		Vec3 m_zoomEye;		// �Y�[����̈ʒu
		Vec3 m_currentEye;	// �Y�[���̃X�^�[�g�ʒu
		Vec3 m_initialEye;	// �J�����̏����ʒu
		Vec3 m_initialAt;	// �J�����̏��������_

		float m_zoomRatio; // �Y�[���̊���
		float m_zoomSpeed; // �Y�[�����x

	public:
		enum State {
			Fixed,	// �Œ�
			Follow,	// �Ǐ]
			ZoomIn,	// �Y�[���C��
			ZoomOut	// �Y�[���A�E�g
		};
		State m_cameraState;	// �J�����̌��݂̏��
		const State m_DefaultState;	// �J�����̏������

		/// <summary>
		/// �J�����̃R���X�g���N�^
		/// </summary>
		/// <param name="cameraState">�J�����̏������</param>
		MainCamera(State cameraState, const Vec3& eyePos, const Vec3& atPos) : Camera(),
			m_cameraState(cameraState),
			m_DefaultState(cameraState),
			m_targetPos(Vec3(0.0f)),
			m_initialEye(eyePos),
			m_initialAt(atPos),
			m_MaxEye(Vec3(400.0f, 20.0f, -22.0f)),
			m_currentEye(0.0f),
			m_ZoomRatioC(0.6f),
			m_zoomEye(Vec3(0.0f)),
			m_zoomRatio(0.0f),
			m_zoomSpeed(0.8f)
		{
		}
		~MainCamera() {}

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// �ΏۂɒǏ]���鏈��
		/// </summary>
		void FollowTarget();

		/// <summary>
		/// �Y�[������
		/// </summary>
		void ZoomProcess();

		/// <summary>
		/// �J�����̃��Z�b�g����
		/// </summary>
		/// <param name="eyePos">�ʒu</param>
		/// <param name="atPos">�����_</param>
		void ResetCamera(Vec3 eyePos, Vec3 atPos);

		/// <summary>
		/// �Y�[�������X�^�[�g���ɌĂяo������
		/// </summary>
		/// <param name="zoomEye">�Y�[����̈ʒu</param>
		void ZoomStart(Vec3 zoomEye) {
			m_zoomEye = zoomEye;
			m_currentEye = GetEye();
			m_cameraState = ZoomIn;
			m_zoomRatio = 0.0f;
		}

		/// <summary>
		/// �Y�[�������X�^�[�g���ɌĂяo������
		/// </summary>
		/// <param name="zoomEye">�Y�[����̈ʒu</param>
		/// <param name="defAt">���������_</param>
		void ZoomStart(Vec3 zoomEye, Vec3 defAt) {
			m_zoomEye = zoomEye;
			m_initialAt = defAt;
			m_currentEye = GetEye();
			m_cameraState = ZoomIn;
			m_zoomRatio = 0.0f;
		}

		/// <summary>
		/// �Y�[�������I�����ɌĂяo������
		/// </summary>
		void ZoomEnd()
		{
			m_cameraState = ZoomOut;
		}

		// �J�������ǔ�����I�u�W�F�N�g���擾����֐�
		shared_ptr<GameObject> GetTargetObject() const {
			if (!m_targetObject.expired()) {
				return m_targetObject.lock();
			}
			return nullptr;
		}
		// �J�������ǔ�����I�u�W�F�N�g��ݒ肷��֐�
		void SetTargetObject(const shared_ptr<GameObject>& Obj) {
			m_targetObject = Obj;
		}
	};
}