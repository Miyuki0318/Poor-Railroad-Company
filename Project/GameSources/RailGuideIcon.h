#pragma once
#include "Billboard.h"

namespace basecross
{
	class RailGuideIcon : public GameObject
	{
		vector<weak_ptr<Billboard>> m_iconVec;

		// �O��̃K�C�h�|�C���g
		vector<Point2D<size_t>> m_pastGuidePoint;

		const Vec2 m_deffScale;
		const float m_boundValue;
		const float m_drawRange;
		const float m_deffPosY;

		float m_totalTime;

	public:

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
		@brief �_�ł̍X�V�����֐�
		*/
		void UpdateIconMove();

		/*!
		@brief �K�C�h�̔z�u�X�V�����֐�
		*/
		void UpdateGuide();

		/*!
		@brief �K�C�h�̕\���ݒ�֐�
		@param �\�����邩�̐^�U
		*/
		void SetIconDraw(bool b);
	};
}