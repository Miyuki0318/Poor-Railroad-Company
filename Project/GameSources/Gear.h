/*!
@file Gear.h
@brief 踏切に使用する歯車
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	// フレンド化するため名前だけ宣言
	class GearManager;

	/*!
	@brief 歯車オブジェクト
	*/
	class Gear : public TemplateObject
	{
		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; // 描画コンポーネント
		shared_ptr<Shadowmap> m_ptrShadow; // 影描画コンポーネント

		// フレンド化
		friend GearManager;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		*/
		Gear(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr, position, Vec3(0.0f), Vec3(1.0f))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Gear() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief アニメーションの更新処理関数
		*/
		void UpdateAnimation();

		/*!
		@brief 取得後の非アクティブ化関数
		*/
		void GetGearSleap();

		/*!
		@brief リセット処理関数
		*/
		void ResetGear();
	};

	/*!
	@brief 歯車管理オブジェクト
	*/
	class GearManager : public GameObject
	{
		weak_ptr<TemplateObject> m_playerPtr;	// プレイヤーのポインタ
		map<string, weak_ptr<Gear>> m_gearMap;	// 歯車の配列
		
		const Mat4x4 m_gearModelMat; // 歯車モデルの差分行列
		const float m_getGearRange;  // 取得できる距離

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GearManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_getGearRange(1.5f),
			m_gearModelMat((Mat4x4)XMMatrixAffineTransformation(
				Vec3(0.5f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 1.0f, 0.0f)
			))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GearManager() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 歯車のリセット関数
		*/
		void ResetGears();

		/*!
		@brief 取得時の処理関数
		@param 歯車データ
		*/
		void GetGearPlayar(const pair<string, weak_ptr<Gear>>& gear);
	};
}