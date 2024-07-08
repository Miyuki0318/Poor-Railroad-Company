/*!
@file CraftWindow.h
@brief �N���t�g�E�B���h�E
@author ���V���M
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{	
	/*!
	@brief �N���t�g�E�B���h�E
	*/
	class CraftWindow : public CraftUI
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		CraftWindow(const shared_ptr<Stage>& stagePtr) :
			CraftUI(stagePtr, L"C_WINDOW_TX", Vec2(300.0f), 0.25f)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~CraftWindow() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �`��ύX�ݒ�֐�
		@param eVerticesRect
		*/
		void SetRectType(eRectType rect) override
		{
			CraftUI::SetRectType(rect);

			for (size_t i = 0; i < m_vertex.vertices.size(); i++)
			{
				m_vertex.vertices.at(i).textureCoordinate = m_rectUVMap.at(rect).at(i);
			}

			m_ptrDraw->UpdateVertices(m_vertex.vertices);
		}
	};
}