//TODO[X] 
//add 100ohm on MCLK https://forum.pjrc.com/threads/41157-Just-noise-with-Teensy-3-6-and-Audio-Adaptor-Board?highlight=rev+mclk+100
// https://github.com/WMXZ-EU/microSoundRecorder
//Effects:
//* Reverb (Freeverb)
//* Delay with feedback and lowpass
//* Chorus
//* Flanger
//* Lowpass - 12db/oct
//* Highpass - 12db/oct

// undefined midi cc list
//    MIDI CC 3
//    MIDI CC 9
//    MIDI CC 14-15
//    MIDI CC 20-31
//    MIDI CC 85-90
//    MIDI CC 102-119

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MIDI.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthNoisePink      pink1;          //xy=84.6153793334961,348.99996852874756
AudioInputUSB            usbIN;          //xy=86.15384292602539,200.69229125976562
AudioInputI2S            i2sIN;          //xy=89,99
AudioSynthWaveformSineHires sine_hires1;    //xy=93.6153793334961,269.99996852874756
AudioAmplifier           amp2L;           //xy=218.6153564453125,80.38461303710938
AudioAmplifier           amp2R;           //xy=219.6154022216797,118.84613990783691
AudioAmplifier           ampUL;           //xy=219.38458251953125,179.3845977783203
AudioAmplifier           ampUR;           //xy=219.61537170410156,218.07688903808594
AudioMixer4              mixerR;         //xy=453.07688903808594,342.6153926849365
AudioMixer4              mixerL;         //xy=455.07688903808594,254.61539268493652
AudioOutputUSB           usbOUT;         //xy=637.1537170410156,261.15380096435547
AudioOutputI2S           i2sOUT;         //xy=639.2306938171387,336.1538333892822
AudioConnection          patchCord1(pink1, 0, mixerR, 2);
AudioConnection          patchCord2(usbIN, 0, ampUL, 0);
AudioConnection          patchCord3(usbIN, 1, ampUR, 0);
AudioConnection          patchCord4(i2sIN, 0, amp2L, 0);
AudioConnection          patchCord5(i2sIN, 1, amp2R, 0);
AudioConnection          patchCord6(sine_hires1, 0, mixerL, 2);
AudioConnection          patchCord7(amp2L, 0, mixerL, 0);
AudioConnection          patchCord8(amp2R, 0, mixerR, 0);
AudioConnection          patchCord9(ampUL, 0, mixerL, 1);
AudioConnection          patchCord10(ampUR, 0, mixerR, 1);
AudioConnection          patchCord11(mixerR, 0, usbOUT, 1);
AudioConnection          patchCord12(mixerR, 0, i2sOUT, 1);
AudioConnection          patchCord13(mixerL, 0, usbOUT, 0);
AudioConnection          patchCord14(mixerL, 0, i2sOUT, 0);
AudioControlSGTL5000     sgtl5000;       //xy=87,43
// GUItool: end automatically generated code

//MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial4, MIDI); // pin 31 on T3.6

void setup() { 
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(20); // wie viel soll?

  // Enable the audio shield, select input, and enable output
  sgtl5000.enable();
  sgtl5000.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000.volume(0.5); // hp volume
//  sgtl5000.adcHighPassFilterDisable(); // https://forum.pjrc.com/threads/58599-adcHighPassFilterEnable()?highlight=highpass+filter+disable https://openaudio.blogspot.com/2017/03/teensy-audio-board-self-noise.html

  sgtl5000.micGain(36); // 0-63 war 36
 sgtl5000.lineInLevel(2,2);  //better audio quality? 2.22Vpp
 //sgtl5000.lineOutLevel(16);
// sgtl5000.audioPostProcessorEnable();
// sgtl5000.autoVolumeControl(0, 3, 1, -18, 0.1, 1);
// sgtl5000.autoVolumeEnable();  // https://forum.pjrc.com/threads/69574-Need-help-ideas-and-less-noise-for-Teensy-3-6-in-my-electric-bass?highlight=lineInLevel
/*autoVolumeControl(maxGain, response, hardLimit, threshold, attack, decay);

 Configures the auto volume control, which is implemented as a compressor/expander or hard limiter. 
 maxGain is the maximum gain that can be applied for expanding, and can take one of three values: 0 (0dB), 1 (6.0dB) and 2 (12dB). 
 Values greater than 2 are treated as 2.
 response controls the integration time for the compressor and can take four values: 0 (0ms), 1 (25ms), 2 (50ms) or 3 (100ms). 
 Larger values average the volume over a longer time, allowing short-term peaks through.

 If hardLimit is 0, a 'soft knee' compressor is used to progressively compress louder values which are near to or above the threashold (the louder they are, the greater the compression). 
 If it is 1, a hard compressor is used (all values above the threashold are the same loudness). 
 The threashold is specified as a float in the range 0dBFS to -96dBFS, where -18dBFS is a typical value. 
 attack is a float controlling the rate of decrease in gain when the signal is over threashold, in dB/s. 
 decay controls how fast gain is restored once the level drops below threashold, again in dB/s. It is typically set to a longer value than attack. 
 */

  // mixer L
  mixerL.gain(0,1); // L i2s
  mixerL.gain(1,1); // L usb
  mixerL.gain(2,0.0); // L sine

  // mixer R
  mixerR.gain(0,1); // R i2s
  mixerR.gain(1,1); // R usb
  mixerR.gain(2,0.0); // R pink
  
  sine_hires1.amplitude(0.0);
  sine_hires1.frequency(110);
   
  pink1.amplitude(0.0); 

//MIDI  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);
  Serial.println("MIDI Receving...");
  MIDI.setHandleControlChange(OnCC); // set handle for MIDI continuous controller messages

  delay(1000);
}

//elapsedMillis volmsec=0;

void loop() {
  
    static int vol = 0;   
    int knob2 = analogRead(15);
    if (knob2 != vol) {
      vol = knob2;
      sgtl5000.volume(knob2 / 1023.);
    } // https://forum.pjrc.com/threads/59943-Teensy-4-0-Audio-Shield-344Hz-buzz-when-using-line-in?highlight=lineInLevel

//  // every 50 ms, adjust the volume
//  if (volmsec > 50) {
//    float vol = analogRead(15);
//    vol = vol / 1023.0;
//    sgtl5000.volume(vol); // <-- uncomment if you have the optional
//    volmsec = 0;               //     volume pot on your audio shield
//  }

//MIDI
  MIDI.read(); // read the MIDI bus every loop
}

void OnCC(byte channel, byte controller, byte value) {
//    Serial.println(String("CH=") + channel + ", CC=" + controller + ", VALUE=" + value); 
      if (channel == 2){
      
      switch(controller) {
        case 51: // line-in L mute/unmute
         switch (value){
          case 64:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", line-in-L-unmute=" + value); 
          amp2L.gain(1.0); // unmute
          break;
          case 0:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", line-in-L-mute=" + value);
          amp2L.gain(value); //mute
          break;
        }
        break;

        case 52: // line-in R mute/unmute
         switch (value){
          case 64:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", line-in-R-unmute=" + value); 
          amp2R.gain(1.0); // unmute
          break;
          case 0:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", line-in-R-mute=" + value);
          amp2R.gain(value); //mute
          break;
        }
        break;
                
  case 61: // usb-in L mute/unmute
         switch (value){
          case 64:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", USB-L-unmute=" + value); 
          ampUL.gain(1.0); // unmute
          break;
          case 0:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", USBL-mute=" + value);
          ampUL.gain(value); //mute
          break;
        }
        break;

        case 62: // usb-in R mute/unmute
         switch (value){
          case 64:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", USB-R-unmute=" + value); 
          ampUR.gain(1.0); // unmute
          break;
          case 0:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", USB-R-mute=" + value);
          ampUR.gain(value); //mute
          break;
        }
        break;

        case 13: // line-in L volume
        Serial.println(String("CH=") + channel + ", CC=" + controller + ", L-gain=" + value/127.0);
        mixerL.gain(0, value/127.0);
        break;

        case 107: // line-in R volume
        Serial.println(String("CH=") + channel + ", CC=" + controller + ", R-gain=" + value/127.0);
        mixerR.gain(0, value/127.0);
        break;

        case 58: // inputSelect
         switch (value){
          case 64:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", VALUE=" + value + " -> INPUT_MIC"); 
          sgtl5000.micGain(50);
          sgtl5000.inputSelect(AUDIO_INPUT_MIC); 
          break;
          case 0:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", VALUE=" + value + " -> INPUT_LINEIN");
          sgtl5000.inputSelect(AUDIO_INPUT_LINEIN); 
          break;
        }
        break;
        
        case 3: // hires sine
         switch (value){
          case 64:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", VALUE=" + value + " -> hires_sine"); 
          mixerL.gain(2,1.0); // L sine
          sine_hires1.amplitude(1.0);
          sine_hires1.frequency(110);
          break;
          case 0:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", VALUE=" + value + " -> hires_sine");
          mixerL.gain(2,0.0); // L sine
          sine_hires1.amplitude(0.0);
          break;
        }
        break;

//        case 13: // line-in L volume
//          Serial.println(String("CH=") + channel + ", CC=" + controller + ", L-gain=" + value/127.0);
//          mixerL.gain(0, value/127.0);
//          break;

//        case 85: // mic gain   sgtl5000.micGain(50); // 0-63 war 36
//          Serial.println(String("CH=") + channel + ", CC=" + controller + ", mic-gain=" + value * 63.0 /127);
//          sgtl5000.micGain(value * 63.0  /127);
//          break;

        case 9: // pink
         switch (value){
          case 64:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", VALUE=" + value + " -> pink"); 
          mixerR.gain(2,1.0); // R pink
          pink1.amplitude(1.0); 
          break;
          case 0:
          Serial.println(String("CH=") + channel + ", CC=" + controller + ", VALUE=" + value + " -> pink");
          mixerR.gain(2,0.0); // R pink
          pink1.amplitude(0.0);  
          break;
        }
        break;
  
  }  
      }
      
}
