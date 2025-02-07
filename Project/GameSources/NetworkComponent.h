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
        
        // 送信バッファにデータを追加
        void AddSendData(const string& header, const string& data) const;

        // 受信バッファからデータを取得
        bool GetRecvData(const string& header, string& outData) const;

        bool IsConnected() const;
        bool IsWaiting() const;

        string GetNetworkID() const;

        virtual void OnUpdate()override {}
        virtual void OnDraw()override {}

    private:

        PPDataConnecter* NetworkComponent::GetNetwork() const;
    };
}