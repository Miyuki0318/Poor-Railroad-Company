/*!
@file BillboardNumber.h
@brief �r���{�[�h�\�����鐔���I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"
#include "BillBoard.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	BillboardNumber : public Billboard �r���{�[�h�\�����鐔���I�u�W�F�N�g
	// ----------------------------------------------------------------------------------
	class BillboardNumber : public Billboard
	{
		int m_number; // ����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �X�P�[��
		*/
		BillboardNumber(const shared_ptr<Stage>& stagePtr) :
			Billboard(stagePtr)
		{
			m_number = 0;
			Utility::NumberVerticesIndices(m_vertex, m_number);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~BillboardNumber() {};

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �\�����鐔�l��ݒ肷��֐�
		@param �ݒ肷�鐔�l
		*/
		void SetNumber(const int& number)
		{
			// ���l�̕ێ�
			m_number = number;

			// ���l�ɉ�����UV���W��ύX
			m_vertex.vertices[0].textureCoordinate.x = m_number / 10.0f;
			m_vertex.vertices[1].textureCoordinate.x = m_number / 10.0f + 0.1f;
			m_vertex.vertices[2].textureCoordinate.x = m_number / 10.0f;
			m_vertex.vertices[3].textureCoordinate.x = m_number / 10.0f + 0.1f;

			// ���_�f�[�^�̍X�V
			auto ptrMesh = GetComponent<PCTStaticDraw>();
			ptrMesh->UpdateVertices(m_vertex.vertices);
		}
	};
}