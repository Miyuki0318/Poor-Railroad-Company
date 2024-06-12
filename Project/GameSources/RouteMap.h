/*!
@file RouteMap.h
@brief �H���}(��Փx�I��)�̐ݒ�
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class RouteMap : public TemplateObject
	{
		// ���f���̃e�N�X�`���L�[
		map<eMapLevel, wstring> m_modelTextures;
		
		// �X�v���C�g�̃e�N�X�`���L�[
		map<eMapLevel, wstring> m_mapTextures;

		// �^�O���擾
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		const Vec3 m_scale = Vec3(3.0f);
		const Vec3 m_position = Vec3(30.0f, 1.0f, -15.0f);

		const Vec2 m_spriteScale = Vec2(500.0f);

		const float m_maxStickValue = 0.2f;

		shared_ptr<PNTStaticDraw> m_drawComp;

		// ���݂̓��͒l
		float m_currentX;

		Mat4x4 m_spanMat;

		shared_ptr<Sprite> m_mapSprite;
		eMapLevel m_mapLevel;

	public:
		RouteMap(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_spanMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);

			m_currentX = 0.0f;
			m_mapLevel = eMapLevel::FirstMap;

			m_modelTextures.emplace(eMapLevel::FirstMap, L"FIRST_TX");
			m_modelTextures.emplace(eMapLevel::SecondMap, L"SECOND_TX");
			m_modelTextures.emplace(eMapLevel::ThirdMap, L"THIRD_TX");
			m_modelTextures.emplace(eMapLevel::FourthMap, L"FOURTH_TX");
			m_modelTextures.emplace(eMapLevel::FifthMap, L"FIFTH_TX");

			m_mapTextures.emplace(eMapLevel::FirstMap, L"FIRSTMAP_TX");
			m_mapTextures.emplace(eMapLevel::SecondMap, L"SECONDMAP_TX");
			m_mapTextures.emplace(eMapLevel::ThirdMap, L"THIRDMAP_TX");
			m_mapTextures.emplace(eMapLevel::FourthMap, L"FOURTHMAP_TX");
			m_mapTextures.emplace(eMapLevel::FifthMap, L"FIFTHMAP_TX");
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		/*
		@brief �}�b�v�̓�Փx�I��
		*/
		virtual void MapSelect();
	};
}