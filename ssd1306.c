#include "ssd1306.h"
#include <string.h> // memset のために必要
#include <stdlib.h> // abs のために必要
#include <xc.h>     // __delay_ms のために必要 (XC8コンパイラの場合)
#include "i2c_wrapper.h" // 作成したI2Cラッパーのヘッダー
#include "ssd1306_font.h" // フォントデータヘッダー

// --- 定数 ---
#define SSD1306_I2C_ADDR_7BIT (SSD1306_I2C_ADDR >> 1) // I2C通信で使用する7ビットアドレス

// --- XC8 遅延関数用設定 ---
#ifndef _XTAL_FREQ
// プロジェクト設定で _XTAL_FREQ が定義されていない場合、デフォルト値 (32MHz) を使用
#define _XTAL_FREQ 32000000
#endif

// --- 描画バッファ本体 ---
uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];

// --- I2C通信ヘルパー関数 ---
// I2Cラッパー関数を呼び出すインライン関数 (エラーチェックは省略)
static inline void SSD1306_SendCommand(uint8_t command) {
    // TODO: I2C_Wrapper_WriteCommand の戻り値を確認するエラー処理を追加推奨
    I2C_Wrapper_WriteCommand(SSD1306_I2C_ADDR_7BIT, command);
}

static inline void SSD1306_SendData(uint8_t data) {
    // TODO: I2C_Wrapper_WriteData の戻り値を確認するエラー処理を追加推奨
    I2C_Wrapper_WriteData(SSD1306_I2C_ADDR_7BIT, data);
}

// フォントデータは ssd1306_font.c で定義され、ssd1306_font.h で extern 宣言されています。

// --- 公開関数 ---

/**
 * @brief SSD1306ディスプレイを初期化します。
 */
void SSD1306_Init(void) {
    // I2C通信自体はMCCのSYSTEM_Initialize()で初期化される想定

    // SSD1306 初期化シーケンス (データシート参照)
    __delay_ms(100); // 電源投入後の安定待ち

    SSD1306_SendCommand(0xAE); // Display OFF (スリープモード)
    SSD1306_SendCommand(0xD5); // 表示クロック分周比/発振周波数設定
    SSD1306_SendCommand(0x80); // デフォルト値
    SSD1306_SendCommand(0xA8); // マルチプレクス比設定
    SSD1306_SendCommand(SSD1306_HEIGHT - 1); // 64 MUX (63)
    SSD1306_SendCommand(0xD3); // 表示オフセット設定
    SSD1306_SendCommand(0x00); // オフセットなし
    SSD1306_SendCommand(0x40 | 0x0); // 表示開始ライン設定 (0行目)
    SSD1306_SendCommand(0x8D); // チャージポンプ設定
    SSD1306_SendCommand(0x14); // チャージポンプ有効 (内部昇圧)
    SSD1306_SendCommand(0x20); // メモリアドレッシングモード設定
    SSD1306_SendCommand(0x00); // 水平アドレッシングモード
    SSD1306_SendCommand(0xA1); // セグメント再マップ設定 (左右反転)
    SSD1306_SendCommand(0xC8); // COM出力スキャン方向設定 (上下反転)
    SSD1306_SendCommand(0xDA); // COMピンハードウェア構成設定
    SSD1306_SendCommand(0x12); // 128x64用設定
    SSD1306_SendCommand(0x81); // コントラスト制御設定
    SSD1306_SendCommand(0xCF); // デフォルトコントラスト値
    SSD1306_SendCommand(0xD9); // プリチャージ期間設定
    SSD1306_SendCommand(0xF1); // 推奨値
    SSD1306_SendCommand(0xDB); // VCOMH電圧レベル設定
    SSD1306_SendCommand(0x40); // 推奨値
    SSD1306_SendCommand(0xA4); // 全ピクセル点灯解除 (RAMの内容を表示)
    SSD1306_SendCommand(0xA6); // 通常表示 (非反転)
    SSD1306_SendCommand(0xAF); // Display ON (スリープ解除)

    SSD1306_ClearBuffer();    // 描画バッファを初期化 (制御バイト含む)
    SSD1306_UpdateScreen(); // 初期状態 (クリア画面) をディスプレイに反映
}

// --- バッファ操作 ---
void SSD1306_ClearBuffer(void) {
    // ページ毎にループし、制御バイト(0x40)とピクセルデータ(0x00)で初期化
    for (uint8_t page = 0; page < SSD1306_NUM_PAGES; page++) {
        uint8_t* page_start = ssd1306_buffer + (page * SSD1306_PAGE_TRANSFER_SIZE);
        page_start[0] = 0x40; // データ送信制御バイト
        memset(&page_start[1], 0, SSD1306_PAGE_SIZE); // ピクセルデータを0でクリア
    }
}

void SSD1306_UpdateScreen(void) {
    // ページ単位でデータを送信する (0 から 7 ページ)
    for (uint8_t page = 0; page < (SSD1306_HEIGHT / 8); page++) {
        // 1. ページアドレスを設定 (B0h-B7h)
        SSD1306_SendCommand(0xB0 | page);

        // 2. カラムアドレスを0に設定
        // Lower nibble of column start address (00h-0Fh) -> 0
        SSD1306_SendCommand(0x00);
        // Upper nibble of column start address (10h-1Fh) -> 0
        SSD1306_SendCommand(0x10);

        // 3. 該当ページのバッファ領域 (制御バイト + ピクセルデータ) のポインタを取得し、送信
        uint8_t* page_transfer_start = ssd1306_buffer + (page * SSD1306_PAGE_TRANSFER_SIZE);
        //    その領域 (129バイト) をそのまま送信
        I2C_Wrapper_WriteBuffer(SSD1306_I2C_ADDR_7BIT, page_transfer_start, SSD1306_PAGE_TRANSFER_SIZE);
    }
}


// --- 描画関数 ---

/**
 * @brief 指定した座標に点を描画します（バッファに書き込みます）。
 *        座標範囲外の場合は何も行いません。
 * @param x X座標 (0 ~ SSD1306_WIDTH - 1)
 * @param y Y座標 (0 ~ SSD1306_HEIGHT - 1)
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawPixel(int16_t x, int16_t y, bool color) {
    // 座標範囲チェック
    if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) {
        return;
    }

    // バッファ内の対応するビット位置を計算 (新しい構造に合わせて)
    // y は 0-63 の範囲なので、y / 8 は 0-7 となり uint8_t に収まる
    uint8_t page = (uint8_t)(y / 8);
    // x は 0-127 の範囲なので uint16_t にキャストしても問題ない
    // byte_index は最大 7*129 + 1 + 127 = 903 + 1 + 127 = 1031 となり uint16_t に収まる
    uint16_t byte_index = (page * SSD1306_PAGE_TRANSFER_SIZE) + 1 + (uint16_t)x;
    // y % 8 は 0-7 なので、シフト結果は uint8_t に収まる
    uint8_t bit_mask = (uint8_t)(1 << (y % 8));

    // 指定された色に応じてビットを操作
    if (color) {
        ssd1306_buffer[byte_index] |= bit_mask;  // ビットを1に設定 (点灯)
    } else {
        ssd1306_buffer[byte_index] &= ~bit_mask; // ビットを0に設定 (消灯)
    }
}

/**
 * @brief 指定した座標にASCII文字を描画します（バッファに書き込みます）。
 *        フォントは 5x8 ピクセルです。
 * @param x 描画開始位置のX座標 (文字の左上)
 * @param y 描画開始位置のY座標 (文字の左上)
 * @param c 描画するASCII文字
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawChar(int16_t x, int16_t y, char c, bool color) {
    // ASCIIコードがフォントデータの範囲外なら何もしない
    if (c < ' ' || c > '~') {
        return;
    }

    // フォントデータ配列内での文字データの開始インデックスを計算
    // (ASCIIコード - 32) * 5バイト/文字
    const uint8_t* font_ptr = ssd1306_font5x8 + (c - ' ') * 5;

    // 5列 (5バイト) 分ループ
    for (uint8_t col = 0; col < 5; col++) {
        uint8_t col_data = font_ptr[col]; // 現在の列のデータを取得
        // 8行 (8ビット) 分ループ
        for (uint8_t row = 0; row < 8; row++) {
            // 現在の行のビットが立っているか確認
            if ((col_data >> row) & 0x01) {
                // 対応するピクセルを描画
                SSD1306_DrawPixel(x + col, y + row, color);
            }
            // 背景色で描画する場合 (オプション)
            // else {
            //     SSD1306_DrawPixel(x + col, y + row, !color); // 背景色
            // }
        }
    }
}

/**
 * @brief 指定した座標から文字列を描画します（バッファに書き込みます）。
 *        フォントは 5x8 ピクセル、文字間隔は1ピクセルです (計6ピクセル幅)。
 * @param x 描画開始位置のX座標 (最初の文字の左上)
 * @param y 描画開始位置のY座標 (最初の文字の左上)
 * @param str 描画する文字列 (NULL終端)
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawString(int16_t x, int16_t y, const char* str, bool color) {
    int16_t current_x = x;
    while (*str) {
        SSD1306_DrawChar(current_x, y, *str, color);
        current_x += 6; // 文字幅(5) + 間隔(1)
        str++;
        // 簡単な折り返し処理 (オプション)
        if (current_x + 5 >= SSD1306_WIDTH) { // 次の文字がはみ出す場合
             // y += 8; // 次の行へ (必要なら)
             // current_x = x; // X座標をリセット (必要なら)
             break; // とりあえず行末で描画終了
        }
    }
}


/**
 * @brief 指定した始点と終点を結ぶ線を描画します（バッファに書き込みます）。
 *        ブレゼンハムのアルゴリズムを使用します。
 * @param x1 始点のX座標
 * @param y1 始点のY座標
 * @param x2 終点のX座標
 * @param y2 終点のY座標
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color) {
    int16_t dx = abs(x2 - x1);
    int16_t dy = -abs(y2 - y1);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx + dy;
    int16_t e2;

    while (1) {
        SSD1306_DrawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { // e_xy+e_x > 0
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) { // e_xy+e_y < 0
            err += dx;
            y1 += sy;
        }
    }
}

/**
 * @brief 指定した対角の2点で定義される矩形領域を描画し、内部を塗りつぶします（バッファに書き込みます）。
 * @param x1 始点のX座標
 * @param y1 始点のY座標
 * @param x2 終点のX座標
 * @param y2 終点のY座標
 * @param color 描画色 (true: 点灯, false: 消灯)
 */
void SSD1306_DrawFilledRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color) {
    int16_t start_x = (x1 < x2) ? x1 : x2;
    int16_t start_y = (y1 < y2) ? y1 : y2;
    int16_t end_x = (x1 > x2) ? x1 : x2;
    int16_t end_y = (y1 > y2) ? y1 : y2;

    // 座標範囲チェックとクリッピング
    if (start_x >= SSD1306_WIDTH || start_y >= SSD1306_HEIGHT || end_x < 0 || end_y < 0) {
        return; // 完全に画面外
    }
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (end_x >= SSD1306_WIDTH) end_x = SSD1306_WIDTH - 1;
    if (end_y >= SSD1306_HEIGHT) end_y = SSD1306_HEIGHT - 1;

    for (int16_t y = start_y; y <= end_y; y++) {
        for (int16_t x = start_x; x <= end_x; x++) {
            SSD1306_DrawPixel(x, y, color);
        }
    }
}
