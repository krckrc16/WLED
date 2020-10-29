#pragma once

#include "wled.h"
#include "driver/i2s.h"
#include "src/dependencies/cqt/CQT.h"

#define SamplingFrequency   11025 // Samplerate 
#define BlockSize           512  // Number of samples (512 is maximum of esp-idf I2S buffer)
#define SampleBufferSize    BlockSize * 2

#define FrequencyBands      16 // Number of frequency bands

// I2S settings
#define I2S_NUM             I2S_NUM_0           // INMP441 L/R -> GND
#define I2S_BCK_IO_NUM      14                  // INMP441 SCK  BCKL
#define I2S_WS_IO_NUM       15                  // INMP441 WS   LRCL
#define I2S_DATA_OUT_NUM    I2S_PIN_NO_CHANGE
#define I2S_IN_NUM          34                  // INMP441 SD





class SoundReactiveUsermod : public Usermod {
  private:
    unsigned long lastTime = 0;

    // Constant Q Transform object
    CQT *cqt;

    uint16_t sampleIndex = 0;
    uint16_t fftIndex = 0;
    
    int sampleBuffer[SampleBufferSize];

    int32_t i2sBuffer[BlockSize];
    uint32_t maxAmp = 1 ;
    const i2s_port_t I2S_PORT = I2S_NUM; 
    const i2s_config_t i2s_config = {
            .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
            .sample_rate = SamplingFrequency,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, 
            .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, 
            .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, 
            .dma_buf_count = 2,
            .dma_buf_len = BlockSize,
            .use_apll = false,
            .tx_desc_auto_clear = false,
            .fixed_mclk = 0,
    };

    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCK_IO_NUM,   
        .ws_io_num = I2S_WS_IO_NUM,    
        .data_out_num = I2S_DATA_OUT_NUM, 
        .data_in_num = I2S_IN_NUM   
        
    };

    int32_t Sample_I2S();
    void handleSound();
    void initSound() ;
    void initCQT() ;
    void initTimers();
  public:
    // Public as to re-enter the object after callback  
    void sampleTimer_callback(void *arg);
    void fftTimer_callback(void *arg);

    void setup(); 
    void loop();
    uint16_t getId();    
};