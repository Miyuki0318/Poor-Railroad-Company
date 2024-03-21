/*!
@file Billboard.cpp
@brief �r���{�[�h�I�u�W�F�N�g�̌p����
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief �������Ɉ�x�����Ăяo�����֐�
	*/
	void Billboard::OnCreate()
	{
		DrawObject::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PCTStaticDraw>();
		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(m_vertex);
		m_ptrDraw->SetTextureResource(m_texWstr);
		m_ptrDraw->SetSamplerState(SamplerState::LinearWrap);

		// �����F�̕`����\��
		SetAlphaActive(true);
	}

	/*!
	@brief �r���{�[�h�X�V�֐�
	*/
	void Billboard::UpdateBillboard()
	{
		// �J���������Ƀr���{�[�h�֐��ŃN�H�[�^�j�I����ݒ�
		const auto& ptrCamera = GetStage()->GetView()->GetTargetCamera();
		Quat qt = Utility::GetBillboardQuat(ptrCamera->GetAt() - ptrCamera->GetEye());
		m_ptrTrans->SetQuaternion(qt);
	}

	/*!
	@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Billboard::SetDiffuseColor(const Col4& color)
	{
		// �`��R���|�[�l���g�̎擾���ăf�B�q���[�Y�F�ɐݒ�
		m_ptrDraw->SetDiffuse(color);

		// �F��ێ�
		m_diffuseColor = color;
	}

	/*!
	@brief �G�~�b�V�u�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Billboard::SetEmissiveColor(const Col4& color)
	{
		// �`��R���|�[�l���g�̎擾���ăG�~�b�V�u�F�ɐݒ�
		m_ptrDraw->SetEmissive(color);

		// �F��ێ�
		m_emissiveColor = color;
	}

	/*!
	@brief ���_�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Billboard::SetVerticesColor(const Col4& color)
	{
		// �F�𒸓_�F�ɐݒ�
		for (auto& v : m_vertex.vertices)
		{
			v.color = color;
		}

		// �`��R���|�[�l���g�̎擾���Ē��_�f�[�^�̍X�V
		m_ptrDraw->UpdateVertices(m_vertex.vertices);

		// �F��ێ�
		m_verticesColor = color;
	}

	/*!
	@brief �e�N�X�`����ύX����֐�
	@param �e�N�X�`���L�[
	*/
	void Billboard::SetTexture(const wstring& textureKey)
	{
		m_ptrDraw->SetTextureResource(textureKey);
	}

	/*!
	@brief �e�N�X�`�����x���V�e�B�ɉ����ĉ�]������֐�
	*/
	void Billboard::RotateTexture()
	{
		// ���_�̐������[�v
		for (auto& v : m_vertex.vertices)
		{
			// ���_��UV���W���ړ��ʁ~�f���^�^�C���ŉ��Z
			v.textureCoordinate += m_velocity * DELTA_TIME;
		}

		// ���_�̍X�V
		m_ptrDraw->UpdateVertices(m_vertex.vertices);
	}
}