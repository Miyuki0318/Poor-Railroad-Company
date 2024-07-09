/*!
@file Number.cpp
@brief ������\������X�v���C�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief �������Ɉ�x�����Ăяo�����֐�
	*/
	void Number::OnCreate()
	{
		// �p�����̐������̊֐������s
		Sprite::OnCreate();

		// ���_��������
		m_vertex.vertices.clear();

		// �����\���p�̒��_�ɕύX
		Utility::NumberVerticesIndices(m_vertex, 0);

		// ���_���X�V
		SetNumber(m_number);
	}

	/*!
	@brief �\�����鐔����ύX����֐�
	@param ����
	*/
	void Number::SetNumber(const int number)
	{
		m_number = number;

		// ���l�ɉ�����UV���W��ύX
		m_vertex.vertices[0].textureCoordinate.x = m_number / 10.0f;
		m_vertex.vertices[1].textureCoordinate.x = m_number / 10.0f + 0.1f;
		m_vertex.vertices[2].textureCoordinate.x = m_number / 10.0f;
		m_vertex.vertices[3].textureCoordinate.x = m_number / 10.0f + 0.1f;

		// ���_�f�[�^�̍X�V
		auto ptrMesh = GetComponent<PCTSpriteDraw>();
		ptrMesh->UpdateVertices(m_vertex.vertices);
	}
}