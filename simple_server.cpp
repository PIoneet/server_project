#include "packet_tcp.h"
#include <netinet/in.h>
#include "packet_type.h"

int main() {
    // 1. 소켓 생성 (핸드폰 사기)
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        cerr << "소켓 생성 실패!" << endl;
        return -1;
    }

    // 2. 주소 설정 (전화번호 개통)
    sockaddr_in server_addr; //실제 구조체 객체
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7777); // 포트 번호: 7777 (게임 서버 단골 포트)
    server_addr.sin_addr.s_addr = INADDR_ANY; // 아무나 다 받아라


    int enable = 1;
    // (이미 사용 중인 주소/포트라도 강제로 다시 바인딩해라)
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(enable));
    // 3. 바인딩 (유심칩 꽂기)
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        cerr << "바인딩 실패! (이미 켜져 있거나 권한 문제)" << endl;
        return -1;
    }


    // 4. 리슨 (벨 소리 기다리기)
    if (listen(server_sock, 5) == -1) { // 5명까지 대기 가능
        cerr << "대기 모드 실패!" << endl;
        return -1;
    }

    cout << "=== 서버가 7777번 포트에서 대기 중입니다... ===" << endl;
    
    // 5. 수락 (전화 받기)
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
    //커널이 IP 주소 방식(IPv4, IPv6 등)에 맞게 소켓을 만들어줌
    if (client_sock == -1) {
        cerr << "접속 수락 실패!" << endl;
        return -1;
    }

    
    cout << ">>> 클라이언트(Windows)가 접속했습니다! <<<" << endl;
   
    ReceiveBuffer recvBuffer;
    HandlePacket(client_sock, &recvBuffer);
    close(server_sock);
    return 0;
}