#ifndef PPDATACONNECTER_H
#define PPDATACONNECTER_H

#include "stdafx.h"
#include <string>
#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <unordered_map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <condition_variable>

using namespace std; // 標準ライブラリを省略形で使用可能にする

// 文字列変換ユーティリティ関数
string WStringToUTF8(const wstring& wstr); // wstring を UTF-8 形式の string に変換
wstring UTF8ToWString(const string& utf8Str); // UTF-8 形式の string を wstring に変換

// 64進数変換ユーティリティ関数
string ConvertToBase64(unsigned long number); // 10進数から64進数に変換
unsigned long ConvertFromBase64(const string& base64Str); // 64進数から10進数に変換

// データ送受信用のバッファ構造体
struct BufferedData
{
    string header;  // データの識別用ヘッダー
    string data;    // 実際のデータ本体
};

// データ通信およびソケット管理クラス
class PPDataConnecter
{
private:

    // デリーター
    struct PPDataDeletar
    {
        void operator()(PPDataConnecter* p) { delete p; }
    };

    static const int BUFFER_SIZE = 4096; // 送受信バッファのサイズ

    // シングルトンインスタンス管理
    static unique_ptr<PPDataConnecter, PPDataDeletar> ppdataConnecter;

    // 非同期通信用のメンバ変数
    condition_variable waitCondition;  // 通信待機時の同期処理用
    thread sendThread;   // 送信スレッド
    thread receiveThread; // 受信スレッド

    mutex socketMutex;  // ソケット保護用のミューテックス
    deque<BufferedData> sendBuffer; // 送信バッファ
    deque<BufferedData> recvBuffer; // 受信バッファ
    mutex sendMutex; // 送信バッファの排他制御
    mutex recvMutex; // 受信バッファの排他制御

    SOCKET currentSocket; // 現在の接続ソケット
    USHORT currentPort; // 現在のポート番号
    
    // コンストラクタ・デストラクタ
    PPDataConnecter();  // 初期化処理
    virtual ~PPDataConnecter() {} // 後始末処理

public:

    // シングルトン構築とアクセサ
    static unique_ptr<PPDataConnecter, PPDataDeletar>& CreateNetwork(bool isPreStart = false);
    static unique_ptr<PPDataConnecter, PPDataDeletar>& GetNetwork();

    static bool NetworkCheck();
    static void DeleteNetwork();

    atomic<bool> isWaiting;  // 通信待機フラグ
    atomic<bool> isCanceled; // 通信キャンセルフラグ
    atomic<bool> isConnected; // 接続状態フラグ
    atomic<bool> isDelete; // 削除フラグ
    unique_ptr<thread> connectThread; // 接続スレッド

    // 初期化と終了処理
    void Initialize();  // Winsock 初期化
    void Finalize();    // Winsock 終了処理

    // コンソールのエンコーディング設定
    void SetConsoleToUnicode();

    // ソケット関連処理
    SOCKET CreateSocket(); // ソケット作成
    sockaddr_in BindAndListen(SOCKET& serverSocket); // ソケットをバインドして待機
    void AcceptConnection(SOCKET serverSocket, SOCKET& clientSocket); // 接続を受け入れる

    // 通信開始
    bool StartServer(SOCKET& socket, const wstring& username); // サーバー開始
    bool ConnectToServer(SOCKET& socket, const wstring& username); // クライアント接続

    // 非同期通信
    void StartServerAsync(); // 非同期サーバー開始
    void ConnectToServerAsync(); // 非同期クライアント接続

    // 通信スレッド制御
    void StartCommunication(SOCKET& socket);
    void CancelCommunication();
    void StopCommunication();

    // データの送受信
    void SendPPMessage(SOCKET sock, const wstring& username, const wstring& message); // メッセージ送信
    void ReceivePPMessages(SOCKET socket); // メッセージ受信
    void AddToSendBuffer(const std::string& header, const std::string& data); // 送信バッファにデータ追加
    bool GetFromRecvBuffer(BufferedData& outData); // 受信バッファからデータ取得
    bool GetFromRecvBufferByHeader(const string& header, BufferedData& outData); // 特定のヘッダーを検索

    // 送受信処理
    void SendData(); // バッファから送信
    void ReceiveData(const std::string& rawData); // 受信データを処理

    // ネットワークユーティリティ
    static string GetLocalIPAddress(); // ローカルIP取得
    static wstring GetLocalIPAddressW(); // ワイド文字列版のローカルIP取得
    USHORT GetPortNumber() const;

    // サーバーIDエンコード/デコード
    static string EncodeAndReverseIPPort(const string& ipAddress, unsigned short port); // IPとポートをエンコード
    static pair<string, unsigned short> DecodeAndReverseIPPort(const string& encodedReversed); // エンコードされたIPとポートをデコード

private:

    // 内部処理
    static string Serialize(const BufferedData& data); // データをシリアライズ
    static BufferedData Deserialize(const std::string& rawData); // デシリアライズ
};

#endif
