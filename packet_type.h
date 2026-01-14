#pragma once
#include <cstdint>

// 1바이트 단위로 꽉 채우기 (빈 공간 없이)
#pragma pack(push, 1)

// 패킷 종류를 나타내는 ID
enum PacketType : uint16_t {
    PKT_NONE = 0,
    PKT_C_LOGIN = 1000, // Client -> Login 요청
    PKT_S_LOGIN = 1001, // Server -> Login 응답
    PKT_C_MOVE  = 1002, // Client -> 이동 요청
    PKT_C_CHAT  = 1003, // Client -> 채팅 메시지
};

// 1. 공통 헤더 (모든 패킷의 앞부분)
struct PacketHeader {
    uint16_t len; // 패킷 전체 길이
    uint16_t id;  // 패킷 ID (이게 로그인인지, 이동인지)
};

// 2. 로그인 패킷 (내용물)
struct PlayerInfoPacket : public PacketHeader {
    int32_t hp;
    int32_t attack;
    // 패킷 본문에는 가변 길이 문자열(string)이나 포인터를 넣으면 안 됨! 
    // (일단은 고정 길이 배열로 연습합시다)
};

// 3. 이동 패킷 (내용물)
struct MovePacket : public PacketHeader {
    float x;
    float y;
    float z;
};

#pragma pack(pop)

