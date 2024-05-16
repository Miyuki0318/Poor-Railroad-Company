/*!
@file ActionGuide.h
@brief 行動のガイドアイコン描画
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"
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

	// アイコン位置タイプenum
	enum class eIconPivot : uint8_t
	{
		Center,	// 中央
		Left,	// 左
		Right,	// 右
	};

	// スプライトタイプenum
	enum class eSpriteType
	{
		Icon,	 // アイコン
		Balloon, // 吹き出し
	};

	// テクスチャタイプ構造体
	struct TextureType
	{
		wstring textureStr; // テクスチャキー 
		uint8_t layerIndex; // 優先度レイヤー数

		TextureType(wstring texStr, uint8_t index) :
			textureStr(texStr),
			layerIndex(index)
		{
		}
	};

	// スプライトタイプ構造体
	struct SpriteType
	{
		eIconPivot pivot;		 // アイコンの位置
		eSpriteType type;		 // テクスチャタイプenum
		weak_ptr<Sprite> sprite; // スプライトのポインタ
		bool active;			 // アクティブかの真偽

		SpriteType(eIconPivot piv, eSpriteType eType, const shared_ptr<Sprite>& ptr) :
			pivot(piv),
			type(eType),
			sprite(ptr)
		{
			active = false;
		}
	};

	/*!
	@brief 行動ガイドアイコン
	*/
	class ActionGuide : public GameObject
	{
		map<eStageID, eActionIcon> m_iconMap;		// アイコンテーブル
		map<eActionIcon, TextureType> m_texMap;		// テクスチャテーブル
		map<eSpriteType, Vec3> m_typePos;			// タイプに応じた座標
		vector<SpriteType> m_spriteMap;				// スプライトテーブル
		map<int, eActionIcon> m_iconBuffer;			// アイコンタイプバッファ
		Vec3 m_position;							// 座標

		const weak_ptr<GamePlayer> m_player;		// プレイヤーのポインタ
		const weak_ptr<SelectIndicator> m_indicator;// インディケーターのポインタ
		const Vec2 m_scale;							// スプライトのスケール
		const Vec3 m_diffPosition;					// 頭上までの座標のずれ
		const float m_distanceX;					// 複数表示時のX軸の余白

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
			GameObject(stagePtr),
			m_player(playerPtr),
			m_indicator(indicatorPtr),
			m_scale(Vec2(50.0f)),
			m_diffPosition(Vec3(0.0f, 2.0f, 0.5f)),
			m_distanceX(30.0f)
		{
			// アイコンタイプに応じた座標テーブル
			m_typePos.insert(make_pair(eSpriteType::Icon, Vec3(0.0f)));
			m_typePos.insert(make_pair(eSpriteType::Balloon, Vec3(0.0f, 0.0f, 0.1f)));

			// ステージIDに応じたアイコンテーブル
			m_iconMap.insert(make_pair(eStageID::Stone, eActionIcon::Pick));
			m_iconMap.insert(make_pair(eStageID::Tree, eActionIcon::Axe));
			m_iconMap.insert(make_pair(eStageID::GuideRail, eActionIcon::Rail));

			// アイコンに応じたテクスチャと優先度テーブル
			m_texMap.insert(make_pair(eActionIcon::Rail, TextureType(L"I_RAIL_TX", 1)));
			m_texMap.insert(make_pair(eActionIcon::Axe, TextureType(L"I_AXE_TX", 2)));
			m_texMap.insert(make_pair(eActionIcon::Pick, TextureType(L"I_PICK_TX", 2)));
			m_texMap.insert(make_pair(eActionIcon::Craft, TextureType(L"I_CRAFT_TX", 3)));
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

		/*!
		@brief 描画するかの設定関数
		@param 描画するかの真偽
		*/
		void SetDrawActive(bool b) override
		{
			// 継承元の描画設定を送る
			GameObject::SetDrawActive(b);

			// 各スプライトに描画設定を送る
			for (const auto& sprite : m_spriteMap)
			{
				sprite.sprite.lock()->SetDrawActive(b);
			}
		}

		/*!
		@brief アクティブにするかの設定関数
		@param アクティブにするかの真偽
		*/
		void SetSpriteActive(bool b)
		{
			// 各スプライトに描画設定を送る
			for (auto& sprite : m_spriteMap)
			{
				sprite.active = b;
			}
		}

	private:

		/*!
		@brief 座標の更新処理関数
		*/
		void UpdatePosition();

		/*!
		@brief アイコンタイプの設定更新処理関数
		*/
		void UpdateIconType();

		/*!
		@brief アイコンセットの更新処理関数
		*/
		void UpdateIconSet();

		/*!
		@brief アイコン描画の更新処理関数
		*/
		void UpdateIconDraw();
	};
}