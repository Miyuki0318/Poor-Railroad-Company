#include "stdafx.h"
#include "Project.h"

#define ONE_PLACE(num) num / 1 % 10
#define TENS_PLACE(num) num / 10 % 10


namespace basecross
{
	void ItemCountUI::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();

		// 背景スプライト
		Vec2 backScale = Vec2(m_spriteScale * 3.75f, m_spriteScale * 2.0f);
		m_backSprite = stagePtr->AddGameObject<Sprite>(L"MENU_TX", backScale, m_startPos + Vec3(m_numberScale, 0.0f, 0.1f));

		// 木のカウンタ
		m_itemIconMap.insert(make_pair(eItemType::Wood, stagePtr->AddGameObject<Sprite>(L"I_WOOD_TX", Vec2(m_spriteScale * 1.25f), Vec3(m_startPos))));
		m_itemCountMap.insert(make_pair(eItemType::Wood, make_pair(
			stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin + m_numMargin)),
			stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin))
		)));
	}

	void ItemCountUI::OnUpdate()
	{
		//const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		UpdateNumbers();
	}

	void ItemCountUI::UpdateNumbers()
	{
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");

		int count = player->GetItemCount(eItemType::Wood);
		m_itemCountMap.at(eItemType::Wood).first.lock()->SetNumber(ONE_PLACE(count));
		m_itemCountMap.at(eItemType::Wood).second.lock()->SetNumber(TENS_PLACE(count));
	}
}