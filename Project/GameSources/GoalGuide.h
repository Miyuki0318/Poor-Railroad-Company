/*!
@file GoalGuide.h
@brief �S�[���w������\������K�C�hUI
@author ���V���M
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief �S�[�������K�C�h
	*/
	class GoalGuide : public Sprite
	{
		weak_ptr<TemplateObject> m_goalStation; // �S�[���w
		weak_ptr<Sprite> m_arrowSprite; // ���̃X�v���C�g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GoalGuide(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"GOAL_GUIDE_TX", Vec2(75.0f))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GoalGuide() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;
	};
}