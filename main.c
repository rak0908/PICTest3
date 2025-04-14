/*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
 */

/*
? [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"
#include "ssd1306.h" // 作成したSSD1306ライブラリのヘッダー

/*
    Main application
 */

int main(void) {
    SYSTEM_Initialize();
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable(); 

    // 1. SSD1306 ディスプレイの初期化
    //    内部で画面クリアと初回更新も行われます。
    SSD1306_Init();
    __delay_ms(500); // 初期化後の待機

    // 2. 対角線を描画
    SSD1306_ClearBuffer(); // 描画前にバッファをクリア
    SSD1306_DrawLine(0, 0, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1, true); // 左上 -> 右下
    SSD1306_DrawLine(0, SSD1306_HEIGHT - 1, SSD1306_WIDTH - 1, 0, true); // 左下 -> 右上
    SSD1306_UpdateScreen(); // バッファの内容を画面に反映
    __delay_ms(1500); // 表示確認用ウェイト

    // 3. 塗りつぶし矩形を描画
    SSD1306_ClearBuffer();
    SSD1306_DrawFilledRectangle(10, 10, 50, 30, true); // 小さい矩形
    SSD1306_DrawFilledRectangle(70, 20, 110, 50, true); // 大きい矩形
    SSD1306_UpdateScreen();
    __delay_ms(1500);

    // 4. 枠線のみの矩形を描画 (線描画の組み合わせ)
    SSD1306_ClearBuffer();
    int16_t x1 = 5, y1 = 5, x2 = SSD1306_WIDTH - 6, y2 = SSD1306_HEIGHT - 6;
    SSD1306_DrawLine(x1, y1, x2, y1, true); // 上辺
    SSD1306_DrawLine(x1, y2, x2, y2, true); // 下辺
    SSD1306_DrawLine(x1, y1, x1, y2, true); // 左辺
    SSD1306_DrawLine(x2, y1, x2, y2, true); // 右辺
    SSD1306_UpdateScreen();
    __delay_ms(1500);

    // 5. ピクセル単位で点を描画 (画面中央に破線)
    SSD1306_ClearBuffer();
    for (int16_t i = 0; i < SSD1306_WIDTH; i += 4) {
        SSD1306_DrawPixel(i, SSD1306_HEIGHT / 2, true);
    }
    SSD1306_UpdateScreen();
    __delay_ms(1500);

    // 6. 文字と文字列を描画
    SSD1306_ClearBuffer();
    SSD1306_DrawString(10, 10, "Hello, World!", true); // "Hello," を (10, 10) に描画
    SSD1306_DrawString(10, 20, "PIC16F18326!", true); // 次の行に描画
    SSD1306_DrawString(10, 30, "Test Test", true); // 次の行に描画
    SSD1306_DrawString(0, 50, "0123456789!@#$%^&*()", true); // 記号含む
    SSD1306_UpdateScreen();
    __delay_ms(2000);
    
    while (1) {
        __delay_ms(2000); // 2秒待機
    }
}