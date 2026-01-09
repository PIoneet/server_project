#include <iostream>
#include <unistd.h> // 리눅스에서 sleep()을 쓰기 위한 헤더 (중요!)

int main() {
    int count = 0;
    
    std::cout << "=== 게임 서버 가동 시작 ===" << std::endl;

    // 서버의 심장 (Main Loop)
    while (true) {
        std::cout << "[Server] 작동 중... 접속 대기 (" << count << "초)" << std::endl;
        count++;
        
        // 1초 쉬기 (CPU 과부하 방지)
        // 윈도우 Sleep(1000)과 다르게, 리눅스는 소문자 sleep(1)이고 '초' 단위입니다.
        sleep(1); 
    }

    return 0;
}
