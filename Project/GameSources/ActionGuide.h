#pragma once
#include "Billboard.h"
#include "GamePlayer.h"

namespace basecross
{
	enum class eActionIcon : size_t
	{
		None,	// 何も無し
		Pick,	// ツルハシ
		Axe,	// アックス
		Craft,	// クラフト
		Rail,	// レール
	};

	class ActionGuide : public Billboard
	{
		eActionIcon m_iconType;
		map<eActionIcon, wstring> m_texMap;
		const weak_ptr<GamePlayer> m_player;
		const weak_ptr<SelectIndicator> m_indicator;
		const Vec3 m_diffPosition;

	public:

		ActionGuide(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<GamePlayer>& playerPtr,
			const shared_ptr<SelectIndicator>& indicatorPtr
		) :
			Billboard(stagePtr, L"WHITE_TX", Vec2(1.0f), Vec3(0.0f)),
			m_player(playerPtr),
			m_indicator(indicatorPtr),
			m_diffPosition(Vec3(0.0f, 2.0f, 0.5f))
		{
			m_iconType = eActionIcon::Pick;

			m_texMap.insert(make_pair(eActionIcon::Pick, L"ICON_PICK_TX"));
			m_texMap.insert(make_pair(eActionIcon::Axe, L"ICON_AXE_TX"));
			m_texMap.insert(make_pair(eActionIcon::Craft, L"ICON_CRAFT_TX"));
			m_texMap.insert(make_pair(eActionIcon::Rail, L"ICON_RAIL_TX"));
		}

		virtual ~ActionGuide() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}