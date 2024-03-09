#include <string.h>
#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "arm_math.h"

using namespace daisy;
using namespace daisysp;
using namespace patch_sm;


const size_t blocksize = 256;

DaisyPatchSM hw;

arm_rfft_fast_instance_f32 DSY_SDRAM_BSS fft;
float DSY_SDRAM_BSS work[blocksize];

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size) {


	arm_rfft_fast_f32(&fft, (float32_t*)&in[0], work, 0);
	arm_rfft_fast_f32(&fft, work, (float32_t*)&out[0], 1);
}

int main(void) {
	hw.Init();

	// initialize FFT and STFT objects
	arm_rfft_fast_init_f32(&fft, blocksize);

	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.SetAudioBlockSize(blocksize);
	hw.StartAudio(AudioCallback);

	// throttle control updates
	while (true) {	}

}