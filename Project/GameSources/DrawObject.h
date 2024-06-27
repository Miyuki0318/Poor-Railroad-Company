/*!
@file DrawObject.h
@brief 描画用オブジェクトの継承元
*/

#pragma once
#include "TemplateObject.h"
#include "OzawaUtility.h"

namespace basecross
{
	// 画像描画方向タイプ
	enum class eRectType
	{
		UpRight,	// 右上
		UpLeft,		// 左上
		DownRight,	// 右下
		DownLeft,	// 左下
	};

	/*!
	@brief 描画用オブジェクトの継承元
	*/
	class DrawObject : public TemplateObject
	{
	protected:

		// 開始時の座標
		const Vec3 m_startPosition;

		Vec2 m_velocity;		// UV用ベロシティ
		Col4 m_diffuseColor;	// ディヒューズ色
		Col4 m_verticesColor;	// 頂点色
		Col4 m_emissiveColor;	// エミッシブ色
		wstring m_texWstr;		// テクスチャキー
		VertexData m_vertex;	// 頂点データ

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		DrawObject(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_startPosition(0.0f)
		{
			m_scale.zero();
			m_velocity.zero();
			m_position.zero();
			m_rotation.zero();
			m_texWstr = L"WHITE_TX";
			m_emissiveColor = COL_NONE;
			m_diffuseColor = COL_WHITE;
			m_verticesColor = COL_WHITE;
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param スケール
		@param ポジション
		*/
		DrawObject(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec3& position,
			const Vec3& rotation, const Vec2& scale
		) :
			TemplateObject(stagePtr, position, rotation, Vec3(scale.x, scale.y, 1.0f)),
			m_texWstr(texWstr),
			m_startPosition(position)
		{
			m_velocity.zero();
			m_emissiveColor = COL_NONE;
			m_diffuseColor = COL_WHITE;
			m_verticesColor = COL_WHITE;
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~DrawObject() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override
		{
			// 継承元の生成時の関数とトランスフォームの簡易設定を送る
			TemplateObject::OnCreate();
			SetTransParam();
		}

		/*!
		@brief 色の範囲が0.0f～1.0fの間になってるかの修正関数
		@param 色
		*/
		void CorrectionColor(Col4& color)
		{
			// 色の範囲が0.0f～1.0fの間になってるかの修正
			for (int i = 0; i < 4; i++)
			{
				color.setElem(i, min(color.getElem(i), 1.0f));
				color.setElem(i, max(color.getElem(i), 0.0f));
			}
		}

		/*!
		@brief ベロシティを変更する関数
		@param ベロシティ
		*/
		void SetVelocity(const Vec2& velo)
		{
			m_velocity = velo;
		}

		/*!
		@brief ベロシティを変更する関数
		@param ベロシティ
		*/
		void SetVelocity(float x, float y)
		{
			SetVelocity(Vec2(x, y));
		}

		/*!
		@brief 開始時の座標に変更する関数
		*/
		void SetStartPosition()
		{
			SetPosition(m_startPosition);
		}

		/*!
		@brief 開始時の座標を取得する関数
		*/
		Vec3 GetStartPosition() const
		{
			return m_startPosition;
		}

		/*!
		@brief 頂点座標の中心を変更する関数
		@param 設定タイプ
		*/
		virtual void SetVerticesRect(eRectType rect) = 0;

		/*!
		@brief ディフューズ色を返す関数
		@return 色
		*/
		Col4 GetDiffuseColor() const
		{
			return m_diffuseColor;
		}

		/*!
		@brief 頂点色を返す関数
		@return 色
		*/
		Col4 GetVerticesColor() const
		{
			return m_verticesColor;
		}

		/*!
		@brief エミッシブ色を返す関数
		@return 色
		*/
		Col4 GetEmissiveColor() const
		{
			return m_emissiveColor;
		}

		/*!
		@brief テクスチャキーを返す関数
		@return テクスチャキー
		*/
		wstring GetTextureKey() const
		{
			return m_texWstr;
		}
	};
}