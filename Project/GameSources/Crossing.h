/*!
@file Crossing.h
@brief 踏切と踏切管理クラス
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief 線路に設置する踏切
	*/
	class Crossing : public TemplateObject
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // 描画コンポーネント
		shared_ptr<Shadowmap> m_ptrShadow; // 描画コンポーネント
		string m_csvLine;	// csvのrowとcolのLINE

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Crossing(const shared_ptr<Stage>& stagePtr,
			const string& csvLine
		) :
			TemplateObject(stagePtr),
			m_csvLine(csvLine)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Crossing() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief モデルとトランスフォームの差分行列設定関数
		@param 設定する行列
		*/
		void SetModelMatrix(const Mat4x4& matrix)
		{
			if (m_ptrDraw) m_ptrDraw->SetMeshToTransformMatrix(matrix);
			if (m_ptrShadow) m_ptrShadow->SetMeshToTransformMatrix(matrix);
		}

		/*!
		@brief 踏切が開いた状態のアニメーションにする関数
		*/
		void SetOpen()
		{
			if (!m_ptrDraw) return;
			if (m_ptrDraw->GetCurrentAnimation() == L"OPEN") return;

			m_ptrDraw->ChangeCurrentAnimation(L"OPEN");
		}

		/*!
		@brief 踏切が閉じた状態のアニメーションにする関数
		*/
		void SetCross()
		{
			if (!m_ptrDraw) return;
			if (m_ptrDraw->GetCurrentAnimation() == L"CROSS") return;

			m_ptrDraw->ChangeCurrentAnimation(L"CROSS");
		}

		/*!
		@brief コンティニュー時に停止状態にする関数
		*/
		void ContinueSleap()
		{
			SetOpen();
			SetUpdateActive(false);
			SetDrawActive(false);
		}

		/*!
		@brief 停止状態を解除し、座標を更新する関数
		*/
		void SleepedWakeUp(string addLine)
		{
			size_t row, col;
			m_csvLine = addLine;
			GetLineStringToRowCol(row, col, m_csvLine);
			SetPosition(ROWCOL2POS(row, col));
			SetUpdateActive(true);
			SetDrawActive(true);
		}
	};

	/*!
	@brief 踏切管理オブジェクト
	*/
	class CrossingManager : public GameObject
	{
		// レールデータマップのポインタ
		const map<string, RailData>* m_railDataMap;

		// 踏切オブジェクト配列
		map<string, weak_ptr<Crossing>> m_crossingMap;

		// 列車のポインタ
		weak_ptr<TemplateObject> m_trainPtr;

		const Mat4x4 m_xLineModelMat;	// X軸直線レール用モデルの差分行列
		const Mat4x4 m_zLineModelMat;	// Z軸直線レール用モデルの差分行列
		const float m_updateRange;	// 更新範囲(列車からの相対距離)
		const float m_openRange;	// 開閉範囲

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		CrossingManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_railDataMap(nullptr),
			m_updateRange(15.0f),
			m_openRange(2.0f),
			m_xLineModelMat((Mat4x4)XMMatrixAffineTransformation(
				Vec3(0.25f, 0.25f, 0.35f),
				Vec3(0.0f),
				(Quat)XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f, 0.0f, 0.0f)),
				Vec3(0.0f, 0.05f, 0.0f)
			)),
			m_zLineModelMat((Mat4x4)XMMatrixAffineTransformation(
				Vec3(0.25f, 0.25f, 0.35f),
				Vec3(0.0f),
				(Quat)XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f)),
				Vec3(0.0f, 0.05f, 0.0f)
			))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CrossingManager() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 初期化関数
		*/
		void ResetCrossing();

		/*!
		@brief 踏切追加関数
		@param 選択ポイント
		*/
		void AddCrossing(const Point2D<size_t>& point);

		/*!
		@brief 現在のポイントの位置がレールになっているか取得関数
		@param 選択ポイント
		@return 一致してたらtrue
		*/
		bool GetIsRailPoint(const Point2D<size_t>& point) const;
	};
}