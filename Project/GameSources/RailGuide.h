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
		shared_ptr<PCTStaticInstanceDraw> m_ptrDraw;

		VertexData m_vertex;  // ���_�f�[�^
		Mat4x4 m_mtxScale;	  // �C���X�^���X�`��p�̃X�P�[��
		Mat4x4 m_mtxRotation; // �C���X�^���X�`��p�̃��[�e�[�V����

		float m_posY; // �|�W�V����Y��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		RailGuide(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_posY(1.5f)
		{
			Quat quatRot;
			quatRot.rotationRollPitchYawFromVector(Vec3(0.0f));
			m_mtxRotation.rotation(quatRot);
			m_mtxScale.scale(Vec3(1.0f));
			Utility::SimpleVerticesIndices(m_vertex);
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
		@brief �r���{�[�h�̍X�V�����֐�
		*/
		void UpdateBillboard();

		/*!
		@brief �K�C�h�̔z�u�X�V�����֐�
		*/
		void UpdateGuide();
	};
}