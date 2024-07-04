/*!
@file RailGuideIcon.h
@brief ���[����ݒu����ꏊ�̃K�C�h�\��
@author ���V���M
*/

#pragma once
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief ���[���ݒu�K�C�h
	*/
	class RailGuideIcon : public GameObject
	{
		// �A�C�R���z��
		vector<weak_ptr<Billboard>> m_iconVec;

		// �O��̃K�C�h�|�C���g
		vector<Point2D<size_t>> m_pastGuidePoint;

		// �v���C���[�̃|�C���^
		weak_ptr<TemplateObject> m_playerPtr;

		const Vec2 m_deffScale;		// �X�P�[��
		const float m_boundValue;	// �o�E���h�����
		const float m_drawRange;	// �\�����鋗��
		const float m_deffPosY;		// �f�t�H���g�̍��WY

		float m_totalTime;	// �o�ߎ���

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		RailGuideIcon(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_drawRange(3.0f),
			m_deffScale(0.75f),
			m_deffPosY(1.75f),
			m_boundValue(0.5f)
		{
			m_totalTime = 0.0f;

			m_iconVec.resize(3);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~RailGuideIcon() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �ڗ������̍X�V�����֐�
		*/
		void UpdateIconMove();

		/*!
		@brief �K�C�h�̔z�u�X�V�����֐�
		*/
		void UpdateGuide();

		/*!
		@brief �v���C���[�Ƃ̋����ŕ\�����邩�̍X�V�����֐�
		*/
		void UpdateRange();

		/*!
		@brief �K�C�h�̕\���ݒ�֐�
		@param �\�����邩�̐^�U
		*/
		void SetIconDraw(bool b);
	};
}