/*!
@file TitleGuide.h
@brief �^�C�g���X�e�[�W�̃K�C�h�A�C�R���`��
@author ���V���M
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{
	/*!
	@brief �I�u�W�F�N�g�K�C�h�A�C�R��
	*/
	class TitleGuide : public CraftUI
	{
		// �^�C�g���I�u�W�F�N�g�}�b�v
		map<wstring, weak_ptr<TemplateObject>> m_titleObjMap;
		
		// �v���C���[�|�C���^
		weak_ptr<TemplateObject> m_playerPtr;
	
		const Vec3 m_diffPosition;	// �Y�����̍��W
		const float m_drawRange;	// �\������܂ł̎���

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		TitleGuide(const shared_ptr<Stage>& stagePtr) :
			CraftUI(stagePtr, L"WHITE_TX", Vec2(300.0f), 0.25f),
			m_diffPosition(50.0f, 100.0f, 0.0f),
			m_drawRange(3.0f)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~TitleGuide() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �������r���A�e�N�X�`����ݒ肷��֐�
		@param �͈͓��ɑ��݂�����
		*/
		bool SetRangeTexture();

		/*!
		@brief �v���C���[�̍��W��ϊ����A�K�C�h�̍��W���X�V����֐�
		*/
		void UpdatePosition();
	};
}