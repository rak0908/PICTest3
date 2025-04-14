#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h> // uint8_t, uint16_t のために必要
#include <stdbool.h> // bool 型のために必要

// --- ディスプレイ設定 ---
#define SSD1306_WIDTH           128 // ディスプレイの幅（ドット）
#define SSD1306_HEIGHT          64  // ディスプレイの高さ（ドット）
#define SSD1306_I2C_ADDR        0x78 // SSD1306 の I2C スレーブアドレス (SA0=GND の場合)

// --- 描画バッファ関連定数 ---
#define SSD1306_PAGE_SIZE       (SSD1306_WIDTH) // 1ページのピクセルデータバイト数 (128)
#define SSD1306_PAGE_TRANSFER_SIZE (1 + SSD1306_PAGE_SIZE) // 1ページ転送時のサイズ (制御バイト + ピクセルデータ) (129)
#define SSD1306_NUM_PAGES       (SSD1306_HEIGHT / 8) // ディスプレイのページ数 (8)
#define SSD1306_BUFFER_SIZE     (SSD1306_NUM_PAGES * SSD1306_PAGE_TRANSFER_SIZE) // 全バッファサイズ (1032)

// --- 描画バッファ本体 ---
// 各ページの先頭1バイトにI2Cデータ制御バイト(0x40)を格納し、
// 後続の128バイトにピクセルデータを格納する構造。
extern uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];

// --- 関数プロトタイプ ---

/**
 * @brief SSD1306ディスプレイを初期化します。
 *        I2C通信が初期化されている必要があります。
 */
void SSD1306_Init(void);

/**
 * @brief 描画バッファの内容を全て0（描画なし）でクリアします。
 */
void SSD1306_ClearBuffer(void);

/**
 * @brief 描画バッファの内容をディスプレイに転送して表示を更新します。
 */
void SSD1306_UpdateScreen(void);

/**
 * @brief 指定した座標に点を描画します（バッファに書き込みます）。
 * @param x X座標 (0 ~ SSD1306_WIDTH - 1)
 * @param y Y座標 (0 ~ SSD1306_HEIGHT - 1)
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawPixel(int16_t x, int16_t y, bool color);

/**
 * @brief 指定した始点と終点を結ぶ線を描画します（バッファに書き込みます）。
 * @param x1 始点のX座標
 * @param y1 始点のY座標
 * @param x2 終点のX座標
 * @param y2 終点のY座標
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color);

/**
 * @brief 指定した対角の2点で定義される矩形領域を描画し、内部を塗りつぶします（バッファに書き込みます）。
 * @param x1 始点のX座標
 * @param y1 始点のY座標
 * @param x2 終点のX座標
 * @param y2 終点のY座標
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawFilledRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color);

/**
 * @brief 指定した座標にASCII文字を描画します（バッファに書き込みます）。
 *        フォントは 5x8 ピクセルです。
 * @param x 描画開始位置のX座標 (文字の左上)
 * @param y 描画開始位置のY座標 (文字の左上)
 * @param c 描画するASCII文字
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawChar(int16_t x, int16_t y, char c, bool color);

/**
 * @brief 指定した座標から文字列を描画します（バッファに書き込みます）。
 *        フォントは 5x8 ピクセル、文字間隔は1ピクセルです (計6ピクセル幅)。
 * @param x 描画開始位置のX座標 (最初の文字の左上)
 * @param y 描画開始位置のY座標 (最初の文字の左上)
 * @param str 描画する文字列 (NULL終端)
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawString(int16_t x, int16_t y, const char* str, bool color);

#endif // SSD1306_H
