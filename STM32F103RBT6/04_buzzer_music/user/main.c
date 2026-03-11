#include "led.h"
#include "buzzer.h"

// 魂斗罗前奏：更精准的音符
unsigned int contra_melody[] = {
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, // 快速连击
    NOTE_G4, NOTE_AS4, NOTE_C5, 
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_G4, NOTE_AS4, NOTE_C5,
    NOTE_E5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4
};

// 对应的快节奏
// 调整节拍时间让音乐更准确
unsigned int contra_beats[] = {
    150, 150, 150, 150, 150, 150,  // 八分音符
    200, 200, 400,                  // 前三个是八分，最后一个是四分
    150, 150, 150, 150, 150, 150,
    200, 200, 400,
    150, 150, 150, 150, 150, 150, 150, 400  // 最后一个是二分音符
};

int main(void) {
    led_init();
    buzzer_init();
    
    while(1) {
        for(int i = 0; i < sizeof(contra_melody)/sizeof(contra_melody[0]); i++) {
            led_on(i % 3);
            
            // 演奏音符
            play_music(contra_melody[i], contra_beats[i]);
            
            led_off(i % 3);
            
            // NES音乐的短促感，停顿20ms
            delay_ms(20); 
        }
        delay_ms(2000); // 结束后停 2 秒
    }
}

