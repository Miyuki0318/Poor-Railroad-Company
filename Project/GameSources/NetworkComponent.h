// NetworkComponent.h
#pragma once
#include "stdafx.h"

namespace basecross
{
    class NetworkComponent : public Component 
    {
    public:

        explicit NetworkComponent(const shared_ptr<GameObject>& GameObjectPtr);
        virtual ~NetworkComponent();

        // 送信バッファにデータを追加
        void AddDataToSendBuffer(const string& header, const string& data) const;
        void AddDataToSendBuffer(const string& header, const wstring& data) const;

        // 受信バッファからデータを取得
        bool GetDataFromReceiveBuffer(const string& header, string& outData) const;
        bool GetDataFromReceiveBuffer(const string& header, wstring& outData) const;

        virtual void OnUpdate()override {}
        virtual void OnDraw()override {}

    private:

        class Impl;
        unique_ptr<Impl> pImpl;
    };
}