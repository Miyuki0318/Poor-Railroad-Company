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
		const float m_ScrollSpeed;	// �X�N���[���X�s�[�h
		const float m_ZoomRatioC;	// �Œ��Ԏ��̃Y�[������

		weak_ptr<GameObject> m_targetObject; // �ڕW�ƂȂ�I�u�W�F�N�g
		shared_ptr<Transform> m_targetTrans; // �ڕW�̃g�����X�t�H�[��

		Vec3 m_targetPos;	// �^�[�Q�b�g�̈ʒu
		Vec3 m_zoomEye;		// �Y�[����̈ʒu
		Vec3 m_currentEye;	// �Y�[���̃X�^�[�g�ʒu
		Vec3 m_currentAt;
		Vec3 m_defScrollEye;// �J�����̏����ʒu(�X�N���[���O)
		Vec3 m_initialEye;	// �J�����̏����ʒu
		Vec3 m_initialAt;	// �J�����̏��������_

		float m_zoomAtY;	// �Y�[����̒����_
		float m_zoomRatio;	// �Y�[���̊���
		float m_zoomSpeed;	// �Y�[�����x
		float m_scrollRatio; // �X�N���[������

	public:
		enum State {
			Fixed,	// �Œ�
			Follow,	// �Ǐ]
			ZoomIn,	// �Y�[���C��
			ZoomOut, // �Y�[���A�E�g
			ZoomedIn, // �Y�[���C����
			Scroll,	// �X�N���[��
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
			m_defScrollEye(eyePos),
			m_targetPos(Vec3(0.0f)),
			m_initialEye(m_defScrollEye),
			m_initialAt(atPos),
			m_MaxEye(Vec3(400.0f, 20.0f, -22.0f)),
			m_currentEye(0.0f),
			m_currentAt(0.0f),
			m_ZoomRatioC(0.6f),
			m_ScrollSpeed(0.3f),
			m_zoomEye(Vec3(0.0f)),
			m_zoomAtY(0.0f),
			m_zoomRatio(0.0f),
			m_zoomSpeed(0.8f),
			m_scrollRatio(0.0f)
		{
		}
		/// <summary>
		/// �J�����̃R���X�g���N�^
		/// </summary>
		/// <param name="cameraState">�J�����̏������</param>
		MainCamera(State cameraState, const Vec3& defEyePos, const Vec3& startEyePos, const Vec3& atPos) : Camera(),
			m_cameraState(cameraState),
			m_DefaultState(cameraState),
			m_defScrollEye(defEyePos),
			m_targetPos(Vec3(0.0f)),
			m_initialEye(startEyePos),
			m_initialAt(atPos),
			m_MaxEye(Vec3(400.0f, 20.0f, -22.0f)),
			m_currentEye(0.0f),
			m_currentAt(0.0f),
			m_ZoomRatioC(0.6f),
			m_ScrollSpeed(0.3f),
			m_zoomEye(Vec3(0.0f)),
			m_zoomAtY(0.0f),
			m_zoomRatio(0.0f),
			m_zoomSpeed(0.8f),
			m_scrollRatio(0.0f)
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
		/// �Y�[���C����̏���
		/// </summary>
		void ZoomedInProcess();

		/// <summary>
		/// �Y�[������
		/// </summary>
		void ZoomProcess();

		/// <summary>
		/// �X�N���[������
		/// </summary>
		void ScrollProcess();

		/// <summary>
		/// �J�����̃��Z�b�g����
		/// </summary>
		/// <param name="defEyePos">�X�N���[���O�̈ʒu</param>
		/// <param name="startEyePos">�v���C�X�^�[�g���̈ʒu</param>
		/// <param name="atPos">�Q�[�����̒����_</param>
		void ResetCamera(Vec3 defEyePos, Vec3 startEyePos, Vec3 atPos);

		/// <summary>
		/// �Y�[�������X�^�[�g���ɌĂяo������
		/// </summary>
		/// <param name="zoomEye">�Y�[����̈ʒu</param>
		void ZoomStart(Vec3 zoomEye) {
			m_zoomEye = zoomEye;
			m_currentEye = GetEye();
			m_currentAt = GetAt();
			m_cameraState = ZoomIn;
			m_zoomRatio = 0.0f;
		}

		/// <summary>
		/// �Y�[�������X�^�[�g���ɌĂяo������
		/// </summary>
		/// <param name="zoomEye">�Y�[����̈ʒu</param>
		/// <param name="zoomAtY">�Y�[�����Y���W�ɉ��Z����l</param>
		void ZoomStart(Vec3 zoomEye, float zoomAtY) {
			m_zoomEye = zoomEye;
			m_zoomAtY = zoomAtY;
			m_currentEye = GetEye();
			m_currentAt = GetAt();
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
			m_currentEye = GetEye();
			m_currentAt = GetAt();
			m_cameraState = ZoomIn;
			m_zoomRatio = 0.0f;
		}

		/// <summary>
		/// �Y�[�������I�����ɌĂяo������
		/// </summary>
		void ZoomEnd()
		{
			if (m_cameraState != ZoomedIn) return;
			m_cameraState = ZoomOut;
		}

		/// <summary>
		/// �X�N���[���J�n����
		/// </summary>
		void ScrollStart()
		{
			m_cameraState = Scroll;
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

		/// <summary>
		/// �X�N���[�����I�����Ă��邩�ǂ���
		/// </summary>
		/// <returns>�I�����Ă�����true</returns>
		bool GetScrollEnd() const
		{
			return m_scrollRatio >= 1.0f;
		}
	};
}