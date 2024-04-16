/*!
@file RailGuide.h
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
	class RailGuide : public TemplateObject
	{
		// �`��R���|�[�l���g
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

		Mat4x4 m_mtxScale;	   // �C���X�^���X�`��p�̃X�P�[��
		Mat4x4 m_mtxRotation;  // �C���X�^���X�`��p�̃��[�e�[�V����
		const Col4 m_defColor; // �f�t�H���g�̐F

		float m_posY;		// �|�W�V����Y��
		float m_blinkRatio;	// �_�Ŋ���
		
		const float m_blinkTime; // �_�ł��鎞��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		RailGuide(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_defColor(1.0f, 1.0f, 1.0f, 0.25f),
			m_posY(1.0f),
			m_blinkRatio(0.0f),
			m_blinkTime(0.5f)
		{
			Quat quatRot;
			quatRot.rotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f));
			m_mtxRotation.rotation(quatRot);
			m_mtxScale.scale(Vec3(0.675f));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~RailGuide() {}

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