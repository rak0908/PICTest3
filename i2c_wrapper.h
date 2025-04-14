#ifndef I2C_WRAPPER_H
#define I2C_WRAPPER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> // size_t のために必要

/**
 * @brief I2C通信を初期化します (必要に応じてMCCの初期化関数を呼び出します)。
 *        通常、MCCのSYSTEM_Initialize()内でI2C初期化が行われるため、
 *        この関数が空になることもあります。
 */
void I2C_Wrapper_Initialize(void);

/**
 * @brief 指定された7ビットアドレスのデバイスにコマンドを送信します。
 * @param address ターゲットデバイスの7ビットI2Cアドレス
 * @param command 送信するコマンドバイト
 * @return 通信が成功した場合は true、失敗した場合は false
 */
bool I2C_Wrapper_WriteCommand(uint8_t address, uint8_t command);

/**
 * @brief 指定された7ビットアドレスのデバイスにデータを1バイト送信します。
 * @param address ターゲットデバイスの7ビットI2Cアドレス
 * @param data 送信するデータバイト
 * @return 通信が成功した場合は true、失敗した場合は false
 */
bool I2C_Wrapper_WriteData(uint8_t address, uint8_t data);

/**
 * @brief 指定された7ビットアドレスのデバイスにデータブロックを送信します。
 * @param address ターゲットデバイスの7ビットI2Cアドレス
 * @param data 送信するデータのポインタ
 * @param count 送信するバイト数
 */
// bool I2C_Wrapper_WriteDataBlock(uint8_t address, uint8_t* data, size_t count); // 削除

/**
 * @brief 指定された7ビットアドレスのデバイスにバッファの内容をそのまま送信します。
 *        制御バイトなどは呼び出し側でバッファに含めておく必要があります。
 * @param address ターゲットデバイスの7ビットI2Cアドレス
 * @param buffer 送信するデータが格納されたバッファのポインタ
 * @param count 送信するバイト数
 * @return 通信が成功した場合は true、失敗した場合は false
 */
bool I2C_Wrapper_WriteBuffer(uint8_t address, uint8_t* buffer, size_t count);

/**
 * @brief 指定された7ビットアドレスのデバイスからデータブロックを読み込みます。
 *        (今回はSSD1306では使用しませんが、汎用ラッパーとして用意)
 * @param address ターゲットデバイスの7ビットI2Cアドレス
 * @param data 読み込んだデータを格納するバッファのポインタ
 * @param count 読み込むバイト数
 * @return 通信が成功した場合は true、失敗した場合は false
 */
bool I2C_Wrapper_ReadDataBlock(uint8_t address, uint8_t* data, size_t count);

/**
 * @brief 指定された7ビットアドレスのデバイスにデータを書き込んだ後、続けてデータを読み込みます。
 *        (今回はSSD1306では使用しませんが、汎用ラッパーとして用意)
 * @param address ターゲットデバイスの7ビットI2Cアドレス
 * @param writeData 書き込むデータのポインタ
 * @param writeCount 書き込むバイト数
 * @param readData 読み込んだデータを格納するバッファのポインタ
 * @param readCount 読み込むバイト数
 * @return 通信が成功した場合は true、失敗した場合は false
 */
bool I2C_Wrapper_WriteReadDataBlock(uint8_t address, uint8_t* writeData, size_t writeCount, uint8_t* readData, size_t readCount);

#endif // I2C_WRAPPER_H
