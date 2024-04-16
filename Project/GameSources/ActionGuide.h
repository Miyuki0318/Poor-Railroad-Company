/*!
@file ActionGuide.h
@brief 行動のガイドアイコン描画
@author 小澤博貴
*/

#pragma once
#include "Billboard.h"
#include "GamePlayer.h"

namespace basecross
{
	// アイコンタイプenum
	enum class eActionIcon : size_t
	{
		None,	// 何も無し
		Pick,	// ツルハシ
		Axe,	// アックス
		Craft,	// クラフト
		Rail,	// レール
	};

	/*!
	@brief 行動ガイドアイコン
	*/
	class ActionGuide : public Billboard
	{
		eActionIcon m_iconType;	// アイコンのタイプ
		map<eStageID, eActionIcon> m_iconMap; // アイコンテーブル
		map<eActionIcon, wstring> m_texMap;	// テクスチャテーブル

		const weak_ptr<GamePlayer> m_player; // プレイヤーのポインタ
		const weak_ptr<SelectIndicator> m_indicator; // インディケーターのポインタ
		const Vec3 m_diffPosition; // 頭上までの座標のずれ

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param プレイヤーポインタ
		@param インディケーターポインタ
		*/
		ActionGuide(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<GamePlayer>& playerPtr,
			const shared_ptr<SelectIndicator>& indicatorPtr
		) :
			Billboard(stagePtr, L"WHITE_TX", Vec2(1.0f), Vec3(0.0f)),
			m_player(playerPtr),
			m_indicator(indicatorPtr),
			m_diffPosition(Vec3(0.0f, 2.0f, 0.5f))
		{
			m_iconType = eActionIcon::None;

			m_iconMap.insert(make_pair(eStageID::Rock, eActionIcon::Pick));
			m_iconMap.insert(make_pair(eStageID::Tree, eActionIcon::Axe));
			m_iconMap.insert(make_pair(eStageID::GuideRail, eActionIcon::Rail));

			m_texMap.insert(make_pair(eActionIcon::Pick, L"ICON_PICK_TX"));
			m_texMap.insert(make_pair(eActionIcon::Axe, L"ICON_AXE_TX"));
			m_texMap.insert(make_pair(eActionIcon::Craft, L"ICON_CRAFT_TX"));
			m_texMap.insert(make_pair(eActionIcon::Rail, L"ICON_RAIL_TX"));
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~ActionGuide() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief 座標の更新処理関数
		*/
		void UpdatePosition();

		/*!
		@brief アイコンタイプの更新処理関数
		*/
		void UpdateIconType();
	};
}