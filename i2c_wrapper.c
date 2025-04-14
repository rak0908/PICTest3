#include "i2c_wrapper.h"
#include "mcc_generated_files/i2c_host/mssp1.h" // MCC I2C(MSSP1) ヘッダー
#include <string.h> // memcpy のために必要 (現在は未使用)

// SSD1306 用のI2C制御バイト定義 (このファイル内でのみ使用)
#define SSD1306_COMMAND_CONTROL_BYTE 0x00 // コマンド送信を示す制御バイト
#define SSD1306_DATA_CONTROL_BYTE    0x40 // データ送信を示す制御バイト

/**
 * @brief I2C通信を初期化します (プレースホルダー)。
 *        実際の初期化はMCCのSYSTEM_Initialize()で行われる想定です。
 */
void I2C_Wrapper_Initialize(void) {
    // 通常、MCC生成の I2C1_Initialize() は SYSTEM_Initialize() 内で呼び出されます。
    // 個別の呼び出しが必要な場合は、ここに追記してください。
}

/**
 * @brief 指定された7ビットアドレスのデバイスにコマンドを1バイト送信します。
 *        内部でSSD1306用の制御バイト(0x00)を付加します。
 */
bool I2C_Wrapper_WriteCommand(uint8_t address, uint8_t command) {
    uint8_t i2c_data[2];
    i2c_data[0] = SSD1306_COMMAND_CONTROL_BYTE;
    i2c_data[1] = command;

    // MCCのI2C書き込み関数を呼び出す
    if (!I2C1_Write(address, i2c_data, 2)) {
        return false; // I2C書き込み要求に失敗
    }
    // 転送完了を待つ (ブロッキング)
    while(I2C1_IsBusy());

    // I2Cエラーチェック
    i2c_host_error_t error = I2C1_ErrorGet();
    if (error != I2C_ERROR_NONE) {
        // TODO: I2Cエラー処理 (例: エラーログ、リトライなど)
        return false; // I2Cエラー発生
    }
    return true;
}

/**
 * @brief 指定された7ビットアドレスのデバイスにデータを1バイト送信します。
 *        内部でSSD1306用の制御バイト(0x40)を付加します。
 */
bool I2C_Wrapper_WriteData(uint8_t address, uint8_t data) {
    uint8_t i2c_data[2];
    i2c_data[0] = SSD1306_DATA_CONTROL_BYTE;
    i2c_data[1] = data;

    if (!I2C1_Write(address, i2c_data, 2)) {
        return false; // I2C書き込み要求に失敗
    }
    // 転送完了を待つ (ブロッキング)
    while(I2C1_IsBusy());

    // I2Cエラーチェック
    i2c_host_error_t error = I2C1_ErrorGet();
    if (error != I2C_ERROR_NONE) {
        return false; // I2Cエラー発生
    }
    return true;
}

/**
 * @brief 指定された7ビットアドレスのデバイスにバッファの内容をそのまま送信します。
 *        制御バイト等は呼び出し側でバッファに含める必要があります。
 */
bool I2C_Wrapper_WriteBuffer(uint8_t address, uint8_t* buffer, size_t count) {
    if (count == 0) {
        return true; // 送信データなし
    }
    if (!I2C1_Write(address, buffer, count)) {
        return false; // I2C書き込み要求に失敗
    }
    // 転送完了を待つ (ブロッキング)
    while(I2C1_IsBusy());

    // I2Cエラーチェック
    i2c_host_error_t error = I2C1_ErrorGet();
    if (error != I2C_ERROR_NONE) {
        return false; // I2Cエラー発生
    }
    return true;
}


/**
 * @brief 指定された7ビットアドレスのデバイスからデータブロックを読み込みます。
 */
bool I2C_Wrapper_ReadDataBlock(uint8_t address, uint8_t* data, size_t count) {
    if (!I2C1_Read(address, data, count)) {
        return false; // I2C読み込み要求に失敗
    }
    // 転送完了を待つ (ブロッキング)
    while(I2C1_IsBusy());

    // I2Cエラーチェック
    i2c_host_error_t error = I2C1_ErrorGet();
    if (error != I2C_ERROR_NONE) {
        return false; // I2Cエラー発生
    }
    // Read関数はエラー時に false を返すため、ここでは常に true を返しても良いが、
    // エラーチェックを残すことで将来的なデバッグに役立つ可能性がある
    return true;
}

/**
 * @brief 指定された7ビットアドレスのデバイスにデータを書き込んだ後、続けてデータを読み込みます。
 */
bool I2C_Wrapper_WriteReadDataBlock(uint8_t address, uint8_t* writeData, size_t writeCount, uint8_t* readData, size_t readCount) {
     if (!I2C1_WriteRead(address, writeData, writeCount, readData, readCount)) {
         return false; // I2C書き込み/読み込み要求に失敗
     }
     // 転送完了を待つ (ブロッキング)
     while(I2C1_IsBusy());

     // I2Cエラーチェック
     i2c_host_error_t error = I2C1_ErrorGet();
     if (error != I2C_ERROR_NONE) {
         return false; // I2Cエラー発生
     }
     return true;
}
