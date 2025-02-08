#include "stdafx.h"
#include "OnlinePlayer.h"

namespace basecross
{
	void OnlinePlayer::OnCreate()
	{
		Player::OnCreate();
	}

	void OnlinePlayer::OnUpdate()
	{
		UpdateMove();
		UpdateRotation();
		SetAnimation();
	}

	void OnlinePlayer::UpdateMove()
	{
		Vec3 pos = GetPosition();
		string posStr = Utility::Vec3ToString(pos);
		
		if (m_ptrNet->GetRecvData("POS", posStr))
		{
			SetPosition(Utility::StringToVec3(posStr));
		}
	}

	void OnlinePlayer::UpdateRotation()
	{
		string rotY = to_string(GetRotation().y);

		if (m_ptrNet->GetRecvData("ROTY", rotY))
		{
			SetRotation(Vec3(0.0f, stof(rotY), 0.0f));
		}
	}

	void OnlinePlayer::SetAnimation()
	{
		if (!m_ptrDraw) return;
		if (!m_ptrShadow) return;

		string animKey = "";
		if (m_ptrNet->GetRecvData("ANIMEKEY", animKey))
		{
			for (const auto& map : m_animationMap)
			{
				if (map.second.animeKey == UTF8ToWString(animKey))
				{
					// アニメーションがあるなら
					if (m_ptrDraw->GetCurrentAnimation() == map.second.animeKey)
					{
						// アニメーションの更新
						m_ptrDraw->UpdateAnimation(DELTA_TIME * map.second.animeSpeed);
					}

					// メッシュキーが前回と違うなら
					if (m_currentAnimaKey != map.first)
					{
						// アニメーション付きメッシュの変更
						AnimationMap mesh = m_animationMap.at(map.first);
						m_ptrDraw->SetMeshResource(P_MESH + mesh.animeKey);
						m_ptrDraw->AddAnimation(mesh.animeKey, 0, mesh.flameNum, mesh.loopActive);
						m_ptrDraw->ChangeCurrentAnimation(mesh.animeKey);

						// 影の更新
						m_ptrShadow->SetMeshResource(P_MESH + mesh.animeKey);
					}

					// キーの保持
					m_currentAnimaKey = map.first;
					return;
				}
			}
		}
	}
}