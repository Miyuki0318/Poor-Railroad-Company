/*!
@file Ground.h
@brief ���I�u�W�F�N�g
@author	���V���M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief ���{�b�N�X
	*/
	class GroundBox : public TemplateObject
	{
		shared_ptr<PNTStaticDraw> m_ptrDraw; // �`��R���|�[�l���g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�̃|�C���^
		@param �X�P�[��
		*/
		GroundBox(const shared_ptr<Stage>& stagePtr,
			const Vec3& scale
		) :
			TemplateObject(stagePtr, Vec3(49.5f, 0.0f, 7.0f), Vec3(0.0f), scale)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GroundBox() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;
	};
}