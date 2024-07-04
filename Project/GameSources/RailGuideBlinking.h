/*!
@file RailGuideBlinking.h
@brief ���[����ݒu����ꏊ�̃K�C�h�\��
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief ���[���ݒu�K�C�h
	*/
	class RailGuideBlinking : public TemplateObject
	{
		// �`��R���|�[�l���g
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

		// �O��̃K�C�h�|�C���g
		vector<Point2D<size_t>> m_pastGuidePoint;

		const Mat4x4 m_mtxScale;	// �C���X�^���X�`��p�̃X�P�[��
		Mat4x4 m_mtxRotAxisX;		// �C���X�^���X�`��p�̃��[�e�[�V����X
		Mat4x4 m_mtxRotAxisZ;		// �C���X�^���X�`��p�̃��[�e�[�V����X
		const Col4 m_defColor;		// �f�t�H���g�̐F

		float m_blinkRatio;			// �_�Ŋ���
		const float m_blinkTime;	// �_�ł��鎞��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		RailGuideBlinking(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_defColor(1.0f, 1.0f, 1.0f, 0.5f),
			m_blinkRatio(0.0f),
			m_blinkTime(0.5f),
			m_mtxScale((Mat4x4)DirectX::XMMatrixScalingFromVector(Vec3(0.675f)))
		{
			m_mtxRotAxisX.rotation((Quat)DirectX::XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f)));
			m_mtxRotAxisZ.rotation((Quat)DirectX::XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f)));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~RailGuideBlinking() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �_�ł̍X�V�����֐�
		*/
		void UpdateBlinking();

		/*!
		@brief �K�C�h�̔z�u�X�V�����֐�
		*/
		void UpdateGuide();
	};
}