/*!
@file Sprite.cpp
@brief �X�v���C�g�e�N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief �������Ɉ�x�����Ăяo�����֐�
	*/
	void Sprite::OnCreate()
	{
		// �p�����̐������̊֐������s
		DrawObject::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PCTSpriteDraw>(m_vertex);
		m_ptrDraw->SetTextureResource(m_texWstr);
		m_ptrDraw->SetSamplerState(SamplerState::LinearWrap);

		// �����F�̕`��ݒ�
		SetAlphaActive(true);
	}

	/*!
	@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Sprite::SetDiffuseColor(Col4& color)
	{
		// �F�̏C��
		CorrectionColor(color);

		// �f�B�q���[�Y�F�ɐݒ�
		m_ptrDraw->SetDiffuse(color);

		// �F��ێ�
		m_diffuseColor = color;
	}

	/*!
	@brief �G�~�b�V�u�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Sprite::SetEmissiveColor(Col4& color)
	{
		// �F�̏C��
		CorrectionColor(color);

		// �G�~�b�V�u�F�ɐݒ�
		m_ptrDraw->SetEmissive(color);

		// �F��ێ�
		m_emissiveColor = color;
	}

	/*!
	@brief ���_�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Sprite::SetVerticesColor(Col4& color)
	{
		// �F�̏C��
		CorrectionColor(color);

		// �F�𒸓_�F�ɐݒ�
		for (auto& v : m_vertex.vertices)
		{
			v.color = color;
		}

		// ���_�f�[�^�̍X�V
		m_ptrDraw->UpdateVertices(m_vertex.vertices);

		// �F��ێ�
		m_verticesColor = color;
	}

	/*!
	@brief �e�N�X�`����ύX����֐�
	@param �e�N�X�`���L�[
	*/
	void Sprite::SetTexture(const wstring& textureKey)
	{
		m_ptrDraw->SetTextureResource(textureKey);
	}

	/*!
	@brief �`��ύX�ݒ�֐�
	@param eVerticesRect
	*/
	void Sprite::SetVerticesRect(eRectType rect)
	{
		// ���_���W�̏�����
		Utility::SimpleVertices(m_vertex.vertices);

		// ���Z�ʗp�ϐ�
		const float HELF = 0.5f;
		Vec3 addPos;

		// �����ɉ����Đݒ�
		switch (rect)
		{
		case eRectType::UpRight: // �E��Ȃ�
			addPos = Vec2(HELF, HELF);
			break;

		case eRectType::UpLeft: // ����Ȃ�
			addPos = Vec2(-HELF, HELF);
			break;

		case eRectType::DownRight: // �E���Ȃ�
			addPos = Vec2(HELF, -HELF);
			break;

		case eRectType::DownLeft: // �����Ȃ�
			addPos = Vec2(-HELF, -HELF);
			break;

		default:
			break;
		}

		// ���_�f�[�^�̍��W�����Z
		for (auto& v : m_vertex.vertices)
		{
			v.position += addPos;
		}

		// ���_�f�[�^�̍X�V
		m_ptrDraw->UpdateVertices(m_vertex.vertices);
	}

	/*!
	@brief �t�F�[�h�C����������֐�
	@param �t�F�[�h�Ɋ|���鎞��
	@return �t�F�[�h�������������̐^�U
	*/
	bool Sprite::FadeInColor(const float time)
	{
		// ���ɓ����Ȃ�I��
		if (m_diffuseColor.w >= 1.0f) return true;

		// �F�̎擾
		Col4 color = m_diffuseColor;

		// �F�̒l���f���^�^�C���ŉ��Z����
		color.w += DELTA_TIME / time;

		// ���b�V���̃f�B�t���[�Y�F��ݒ�
		SetDiffuseColor(color);

		// �t�F�[�h�������������̐^�U��Ԃ�
		return color.w >= 1.0f;
	}

	/*!
	@brief �t�F�[�h�C����������֐�
	@param �t�F�[�h�̏I���l
	@param �t�F�[�h�Ɋ|���鎞��
	@return �t�F�[�h�������������̐^�U
	*/
	bool Sprite::FadeInColor(const float time, const float alpha)
	{
		// ���ɓ����Ȃ�I��
		if (m_diffuseColor.w >= alpha) return true;

		// �F�̎擾
		Col4 color = m_diffuseColor;

		// �F�̒l���f���^�^�C���ŉ��Z����
		color.w += DELTA_TIME / time;

		// ���b�V���̃f�B�t���[�Y�F��ݒ�
		SetDiffuseColor(color);

		// �t�F�[�h�������������̐^�U��Ԃ�
		return color.w >= alpha;
	}

	/*!
	@brief �t�F�[�h�C����������֐�
	@param �t�F�[�h�Ɋ|���鎞��
	@return �t�F�[�h�������������̐^�U
	*/
	bool Sprite::FadeOutColor(const float time)
	{
		// ���ɓ����Ȃ�I��
		if (m_diffuseColor.w <= 0.0f) return true;

		// �F�̎擾
		Col4 color = m_diffuseColor;

		// �F�̒l���f���^�^�C���ŉ��Z����
		color.w -= DELTA_TIME / time;

		// ���b�V���̃f�B�t���[�Y�F��ݒ�
		SetDiffuseColor(color);

		// �t�F�[�h�������������̐^�U��Ԃ�
		return color.w <= 0.0f;
	}

	/*!
	@brief �t�F�[�h�C����������֐�
	@param �t�F�[�h�̏I���l
	@param �t�F�[�h�Ɋ|���鎞��
	@return �t�F�[�h�������������̐^�U
	*/
	bool Sprite::FadeOutColor(const float time, const float alpha)
	{
		// ���ɓ����Ȃ�I��
		if (m_diffuseColor.w <= alpha) return true;

		// �F�̎擾
		Col4 color = m_diffuseColor;

		// �F�̒l���f���^�^�C���ŉ��Z����
		color.w -= DELTA_TIME / time;

		// ���b�V���̃f�B�t���[�Y�F��ݒ�
		SetDiffuseColor(color);

		// �t�F�[�h�������������̐^�U��Ԃ�
		return color.w <= alpha;
	}

	/*!
	@brief �e�N�X�`�����x���V�e�B�ɉ����ĉ�]������֐�
	*/
	void Sprite::RotateTexture()
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

	/*!
	@brief �X�v���C�g���x���V�e�B�ɉ����Ĉړ�������֐�
	*/
	void Sprite::MoveSprite()
	{
		Vec3 velo = Vec3(m_velocity);
		SetPosition(GetPosition() + velo);
	}
}