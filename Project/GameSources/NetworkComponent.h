#pragma once
#include "stdafx.h"
#include "PPDataConnecter.h"

namespace basecross
{
    class NetworkComponent : public Component 
    {
    public:

        explicit NetworkComponent(const shared_ptr<GameObject>& GameObjectPtr);
        virtual ~NetworkComponent();

        const PPDataConnecter* NetworkComponent::GetNetwork() const;

        // 送信バッファにデータを追加
        void AddDataToSendBuffer(const string& header, const string& data) const;

        // 受信バッファからデータを取得
        bool GetDataFromReceiveBuffer(const string& header, string& outData) const;

        virtual void OnUpdate()override {}
        virtual void OnDraw()override {}
    };
}