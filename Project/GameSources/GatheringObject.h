/*!
@file GatheringObject.h
@brief 採取できるオブジェクト
@author 小宅碧
*/

#pragma once
#include "BaseStage.h"

namespace basecross
{
	/*!
	@brief インスタンス描画する採取オブジェクト
	*/
	class InstanceGathers : public GameObject
	{
		// 描画コンポーネント
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		InstanceGathers(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~InstanceGathers() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief メッシュデータを設定する関数
		@param メッシュのリソースキー
		@param テクスチャのリソースキー
		@param マルチメッシュかの真偽
		*/
		void SetMeshData(const wstring& meshKey, const wstring& texKey);

		/*!
		@brief インスタンス描画の行列配列に行列を追加する関数
		@param 追加する行列
		*/
		void AddMatrix(const Mat4x4& matrix);

		/*!
		@brief インスタンス描画の行列配列をクリアする関数
		*/
		void AllClearMatrix();

		/*!
		@brief インスタンス描画の行列配列を取得する関数
		@return vector<Mat4x4>&
		*/
		vector<Mat4x4>& GetMatrixVec();
	};

	/*!
	@brief インスタンス描画する採取オブジェクトの管理オブジェクト
	*/
	class GatheringManager : public GameObject
	{
		// インスタンス描画オブジェクト配列
		map<eStageID, weak_ptr<InstanceGathers>> m_gathersMap;

		const Vec3 m_fullScale;	// フルサイズのスケール
		const Vec3 m_helfScale;	// ハーフサイズのスケール

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GatheringManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_fullScale(Vec3(1.0f)),
			m_helfScale(Vec3(0.7f))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GatheringManager() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 採取オブジェクトを再生成する関数
		*/
		void ResetGathering();

		/*!
		@brief 採取処理関数
		@param 選択ポイント
		*/
		int Gathering(const Point2D<size_t>& point);

		/*!
		@brief 現在のポイントの位置が採取オブジェクトかどうか取得関数
		@param 選択ポイント
		@return 一致してたらtrue
		*/
		bool GetIsGatheringPoint(const Point2D<size_t>& point) const;

	private:

		/*!
		@brief 全採取オブジェクトのメッシュデータ設定関数
		*/
		void SetGathersMeshData();

		/*!
		@brief ハーフサイズの採取オブジェクト採取時の関数
		@param 選択ポイント
		*/
		void HelfSizeGathering(const Point2D<size_t>& point);

		/*!
		@brief フルサイズの採取オブジェクト採取時の関数
		@param その位置の行列
		@return ハーフサイズで生成した行列
		*/
		Mat4x4 FullSizeGathering(Mat4x4& matrix);
	};
}