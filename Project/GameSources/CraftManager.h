/*!
@file CraftManager.h
@brief クラフト管理用マネージャ
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"
#include "CraftUI.h"
#include "CraftWindow.h"
#include "CraftItemIcon.h"
#include "CraftingQTE.h"
#include "CSVLoader.h"

namespace basecross
{
	// クラフトアイテムenum
	enum class eCraftItem : size_t
	{
		Rail,		// レール
		WoodBridge,	// 木の橋
		Crossing,	// 踏切
	};

	// クラフトCSVパラメータ
	enum class eCraftParam : size_t
	{
		ItemType,	 // 作るアイテムタイプ
		WoodValue,	 // 木の要求数
		StoneValue,	 // 石の要求数
		GearValue,	 // 歯車の要求数
		SuccesValue, // QTE成功時の作成量
		FailedValue, // QTE失敗時の作成量
	};

	// アイテムカウンタ用enum
	enum class eItemType : size_t
	{
		None,		// 何もなし
		Wood,		// 木
		Stone,		// 石
		Gear,		// 歯車
		Rail,		// レール
		WoodBridge,	// 木の橋
		Crossing,	// 踏切
		GoldBar,	// 金塊
		Money,		// お金
	};

	/*!
	@brief クラフト管理クラス(プレイヤーとクラフト関連クラスの仲介クラス)
	*/
	class CraftManager
	{
		const weak_ptr<TemplateObject> m_player;// プレイヤーのポインタ
		const weak_ptr<CraftingQTE> m_craftQTE; // クラフトQTE
		const weak_ptr<CraftWindow> m_window;	// クラフトウィンドウ

		// クラフトアイテムアイコン
		map<eCraftItem, weak_ptr<CraftItemIcon>> m_iconMap;
		map<eCraftItem, eInputButton> m_inputButton;

		const vector<vector<string>> m_racipe; // クラフトレシピ
		eCraftItem m_craftItem;	// 作成中のアイテム
		eItemType m_craftType;	// 作成中のアイテム

	public:

		map<eItemType, unsigned char> m_itemCount; // アイテム数

		/*!
		@brief コンストラクタ
		@param ウィンドウオブジェクトのポインタ
		@param クラフトQTEオブジェクトのポインタ
		*/
		CraftManager(const shared_ptr<TemplateObject>& playerPtr,
			const shared_ptr<CraftWindow>& windowPtr,
			const shared_ptr<CraftingQTE>& qtePtr) :
			m_player(playerPtr),
			m_window(windowPtr),
			m_craftQTE(qtePtr),
			m_racipe(CSVLoader::LoadFile("CraftRacipe"))
		{
			m_craftItem = eCraftItem::Rail;
			m_craftType = eItemType::Rail;

			// アイテムカウンタ
			m_itemCount.emplace(eItemType::Wood, 0);
			m_itemCount.emplace(eItemType::Stone, 0);
			m_itemCount.emplace(eItemType::Gear, 0);
			m_itemCount.emplace(eItemType::Rail, 0);
			m_itemCount.emplace(eItemType::WoodBridge, 0);
			m_itemCount.emplace(eItemType::Crossing, 0);
			m_itemCount.emplace(eItemType::GoldBar, 0);

			m_inputButton.emplace(eCraftItem::Rail, eInputButton::ButtonB);
			m_inputButton.emplace(eCraftItem::WoodBridge, eInputButton::ButtonA);
			m_inputButton.emplace(eCraftItem::Crossing, eInputButton::ButtonY);
		}

		/*!
		@brief デストラクタ
		*/
		~CraftManager() {}

		/*!
		@brief 生成時の処理関数
		*/
		void OnCreate();

		/*!
		@brief リセット処理関数
		*/
		void ResetCraftManager();

		/*!
		@brief アイテムクラフト関数
		@param クラフトするアイテム
		@return クラフト可能かの真偽
		*/
		bool CraftOrder(eCraftItem item) const;

		/*!
		@brief クラフト有効化関数
		@param 有効にするかの真偽
		*/
		void CraftingEnabled(bool enable);

		/*!
		@brief プレイヤーの座標からウィンドウ座標を取得する関数
		@param プレイヤーのシェアドポインタ
		@return ウィンドウ座標
		*/
		Vec3 GetPlayerWindowPosition(const shared_ptr<TemplateObject>& player);

		/*!
		@brief ウィンドウ座標から画像のRectを取得する関数
		@param ウィンドウ座標
		@return eRectType
		*/
		eRectType GetWindowRectType(const Vec3& windowPos);

		/*!
		@brief 接続デバイスに応じたテクスチャを設定する関数
		*/
		void SetIconDeviceTexture();

		/*!
		@brief QTE開始関数
		*/
		void StartQTE(eCraftItem item, eItemType type);

		/*!
		@brief QTE停止とQTE結果取得関数
		@param クラフトアイテム
		*/
		bool StopQTE();

		/*!
		@brief QTE強制停止関数
		*/
		void DestroyCraftQTE();

		/*!
		@brief ウィンドウ描画が完了したかの真偽取得関数
		@return 描画完了してたらtrue、それ以外はfalse
		*/
		bool GetShowCraftWindow() const
		{
			return m_window.lock()->GetShowWindow();
		}

		/*!
		@brief QTEが終了したかの真偽取得関数
		@return 終了してたらtrue、それ以外はfalse
		*/
		bool GetEndedQTE() const
		{
			return !m_craftQTE.lock()->GetEnableQTE();
		}

		/*!
		@brief アイテム数取得関数
		@param アイテムタイプenum
		@return アイテム数
		*/
		int GetItemCount(eItemType type) const
		{
			if (m_itemCount.find(type) == m_itemCount.end()) return 0;

			return m_itemCount.at(type);
		}

		/*!
		@brief アイテム数追加関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		void AddItemCount(eItemType type, int addNum = 1)
		{
			if (m_itemCount.find(type) == m_itemCount.end()) return;

			m_itemCount.at(type) += addNum;
		}

		/*!
		@brief アイテム数追加関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		void UseItem(eItemType type, int useNum = 1)
		{
			if (m_itemCount.find(type) == m_itemCount.end()) return;

			m_itemCount.at(type) -= useNum;
		}

		/*!
		@brief クラフト要求素材数取得関数
		@param クラフトアイテム
		@param 素材タイプ
		@return 素材要求数
		*/
		int GetRacipeValue(eCraftItem item, eCraftParam param) const
		{
			return stoi(m_racipe.at(static_cast<size_t>(item)).at(static_cast<size_t>(param)));
		}

		/*!
		@brief クラフト可能かの真偽取得関数
		@param 木の要求数
		@param 石の要求数
		@param 歯車の要求数
		@return 可能かの真偽
		*/
		bool GetCraftPossible(int woodValue, int stoneValue, int gearValue) const
		{
			return woodValue <= GetItemCount(eItemType::Wood)	// 木を要求数以上所持しているか
				&& stoneValue <= GetItemCount(eItemType::Stone) // 石を要求数以上所持しているか
				&& gearValue <= GetItemCount(eItemType::Gear);	// 歯車を要求数以上所持しているか
		}

		/*!
		@brief クラフトしているアイテムタイプを返す関数
		@return m_craftType
		*/
		eItemType GetCraftingItemType() const
		{
			return m_craftType;
		}
	};
}