#include "stdafx.h"
#include "Scene.h"
#include "NetworkComponent.h"
#include "PPDataConnecter.h"
#include <stdexcept>

namespace basecross
{
    NetworkComponent::NetworkComponent(const std::shared_ptr<GameObject>& GameObjectPtr)
        : Component(GameObjectPtr)
    {
    }

    NetworkComponent::~NetworkComponent() {}

    // GetNetworkPtrを使用してP2P通信クラスにアクセス
    PPDataConnecter* NetworkComponent::GetNetwork() const
    {
        auto network = PPDataConnecter::GetNetwork().get();
        if (!network)
        {
            throw BaseException(
                L"ネットワークオブジェクトが生成されていません",
                L"WinMainで初期化されているか確認してください",
                L"NetworkComponent::GetNetwork()"
            );
        }
        return network;
    }

    void NetworkComponent::AddSendData(const std::string& header, const std::string& data) const
    {
        try 
        {
            GetNetwork()->AddToSendBuffer(header, data);
        }
        catch (...) 
        {
            throw;
        }
    }

    bool NetworkComponent::GetRecvData(const string& header, string& outData) const
    {
        try 
        {
            if (GetNetwork()->GetFromRecvBufferByHeader(header, outData))
            {
                return true;
            }

            return false;
        }
        catch (...) 
        {
            throw;
        }
    }

    bool NetworkComponent::IsConnected() const
    {
        return GetNetwork()->isConnected;
    }

    bool NetworkComponent::IsWaiting() const
    {
        return GetNetwork()->isWaiting;
    }

    string NetworkComponent::GetNetworkID() const
    {
        auto netPtr = GetNetwork();
        return netPtr->EncodeAndReverseIPPort(netPtr->GetLocalIPAddress(), netPtr->GetPortNumber());
    }
}