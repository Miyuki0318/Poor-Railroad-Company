/*!
@file RouteMap.h
@brief �H���}(��Փx�I��)�̐ݒ�
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"
#include "Scene.h"

namespace basecross {
	class RouteMap : public TemplateObject
	{
		// ���f���̃e�N�X�`���L�[
		map<eMapLevel, wstring> m_modelTextures;
		
		// �^�O���擾
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		const Vec3 m_position = Vec3(18.0f, 1.0f, -16.0f);

		const Vec2 m_spriteScale = Vec2(500.0f);

		const float m_maxStickValue = 0.5f;

		shared_ptr<PNTStaticDraw> m_drawComp;

		Mat4x4 m_spanMat;

		eMapLevel m_mapLevel;

		bool m_isMove;

		bool holdTag;

	public:
		RouteMap(const shared_ptr<Stage>& stagePtr,
			eMapLevel& mapLevel) :
			TemplateObject(stagePtr),
			m_mapLevel(mapLevel),
			holdTag(false)
		{
			m_spanMat.affineTransformation(
				Vec3(0.65f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);

			m_modelTextures.emplace(eMapLevel::FirstMap, L"FIRST_TX");
			m_modelTextures.emplace(eMapLevel::SecondMap, L"SECOND_TX");
			m_modelTextures.emplace(eMapLevel::ThirdMap, L"THIRD_TX");
			m_modelTextures.emplace(eMapLevel::FourthMap, L"FOURTH_TX");
			m_modelTextures.emplace(eMapLevel::FifthMap, L"FIFTH_TX");
			m_modelTextures.emplace(eMapLevel::SixthMap, L"SIXTH_TX");
			m_modelTextures.emplace(eMapLevel::SeventhMap, L"SEVENTH_TX");
			m_modelTextures.emplace(eMapLevel::EighthMap, L"EIGHTH_TX");
			m_modelTextures.emplace(eMapLevel::NinthMap, L"NINTH_TX");
			m_modelTextures.emplace(eMapLevel::TenthMap, L"TENTH_TX");
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		/*
		@brief �}�b�v�̓�Փx�I��
		*/
		virtual void MapSelect();

		bool GetIsInputX()
		{
			return m_isMove;
		}

	};
}