/*!
@file MainCamera.h
@brief �J����
@prod ��I�t
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MainCamera : public Camera {
		weak_ptr<GameObject> m_targetObject;	// �ڕW�ƂȂ�I�u�W�F�N�g
		shared_ptr<Transform> m_targetTrans;	// �ڕW�̃g�����X�t�H�[��

		Vec3 m_targetPos;
		Vec3 m_cameraArm;
		Vec3 m_currentEye;

		float m_zoomRatio;
		float m_zoomSpeed;

	public:
		enum State {
			Fixed,	// �Œ�
			Follow,	// �Ǐ]
			Zoom	// �Y�[��
		};
		State m_cameraState;	// �J�����̌��݂̏��
		State m_defaultState;	// �J�����̏������

		/// <summary>
		/// �J�����̃R���X�g���N�^
		/// </summary>
		MainCamera() : Camera(),
			m_cameraState(Fixed),
			m_defaultState(Fixed),
			m_targetPos(Vec3(0.0f)),
			m_cameraArm(Vec3(0.0f, 20.0f, -22.0f)),
			m_zoomRatio(0.0f),
			m_zoomSpeed(1.0f)
		{
		}
		/// <summary>
		/// �J�����̃R���X�g���N�^
		/// </summary>
		/// <param name="cameraState">�J�����̏������</param>
		MainCamera(State cameraState) : Camera(),
			m_cameraState(cameraState),
			m_defaultState(cameraState),
			m_targetPos(Vec3(0.0f)),
			m_cameraArm(Vec3(0.0f, 20.0f, -22.0f)),
			m_zoomRatio(0.0f),
			m_zoomSpeed(1.0f)
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
		void ZoomInProcess();

		/// <summary>
		/// �Y�[�������X�^�[�g���ɌĂяo������
		/// </summary>
		/// <param name="currentEye"></param>
		void ZoomStart(Vec3 currentEye) {
			m_currentEye = currentEye;
			m_cameraState = Zoom;
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