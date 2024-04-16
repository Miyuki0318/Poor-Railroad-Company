#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void ActionGuide::OnCreate()
	{
		Billboard::OnCreate();
		SetTexture(m_texMap.at(m_iconType));
		SetDrawActive(false);
	}

	void ActionGuide::OnUpdate()
	{
		UpdateBillboard();

		// プレイヤーとインディケーターが存在するか
		const auto& player = m_player.lock();
		const auto& indicator = m_indicator.lock();
		if (!player || !indicator) return;

		m_iconType = eActionIcon::None;
		eActionIcon icon = m_iconType;
		SetPosition(player->GetPosition() + m_diffPosition);

		if (player->GetCraftPosshible())
		{
			m_iconType = eActionIcon::Craft;
		}

		const auto& stageMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuideMap();
		const auto& selectPoint = indicator->GetSelectPoint();
		if (Utility::WithInElemRange(selectPoint.x, selectPoint.y, stageMap))
		{
			eStageID id = static_cast<eStageID>(stageMap.at(selectPoint.x).at(selectPoint.y));
			if (id == eStageID::Tree)
			{
				m_iconType = eActionIcon::Axe;
			}
			if (id == eStageID::Rock)
			{
				m_iconType = eActionIcon::Pick;
			}
			if (id == eStageID::GuideRail && player->GetStatus(ePlayerStatus::IsHaveRail))
			{
				m_iconType = eActionIcon::Rail;
			}
		}

		if (icon != m_iconType)
		{
			SetTexture(m_texMap.at(m_iconType));
		}

		SetDrawActive(m_iconType != eActionIcon::None);
	}
}