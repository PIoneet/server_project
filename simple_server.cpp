#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // 1. 소켓 생성 (핸드폰 사기)
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        std::cerr << "소켓 생성 실패!" << std::endl;
        return -1;
    }

    // 2. 주소 설정 (전화번호 개통)
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7777); // 포트 번호: 7777 (게임 서버 단골 포트)
    server_addr.sin_addr.s_addr = INADDR_ANY; // 아무나 다 받아라

    // 3. 바인딩 (유심칩 꽂기)
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "바인딩 실패! (이미 켜져 있거나 권한 문제)" << std::endl;
        return -1;
    }

    // 4. 리슨 (벨 소리 기다리기)
    if (listen(server_sock, 5) == -1) { // 5명까지 대기 가능
        std::cerr << "대기 모드 실패!" << std::endl;
        return -1;
    }

    std::cout << "=== 서버가 7777번 포트에서 대기 중입니다... ===" << std::endl;

    // 5. 수락 (전화 받기)
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
    
    if (client_sock == -1) {
        std::cerr << "접속 수락 실패!" << std::endl;
        return -1;
    }

    std::cout << ">>> 클라이언트(Windows)가 접속했습니다! <<<" << std::endl;

    // 6. 통신 (대화하기)
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // 버퍼 비우기
        int str_len = recv(client_sock, buffer, sizeof(buffer), 0);
        
        if (str_len == 0) { // 연결 끊김
            std::cout << "클라이언트가 접속을 종료했습니다." << std::endl;
            break;
        }

        std::cout << "[받은 메시지]: " << buffer << std::endl;
        
        // 받은 말 그대로 돌려주기 (Echo)
        send(client_sock, buffer, str_len, 0);
    }

    // 7. 종료 (전화 끊기)
    close(client_sock);
    close(server_sock);
    return 0;
}