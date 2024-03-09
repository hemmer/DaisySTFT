// fourier.h
#ifndef FOURIER

#include "wave.h"
#include "arm_math.h"

namespace soundmath
{
	template <typename T, size_t N> class Fourier
	{
	public:
		void (*processor)(const T* in, T* out);

		// in, middle, out need to be arrays of size (N * laps * 2)
		Fourier(void (*processor)(const T*, T*), Wave<T>* window, size_t laps, T* in, T* middle, T* out) 
			: processor(processor), in(in), middle(middle), out(out), window(window), laps(laps), stride(N / laps)
		{
			writepoints = new int[laps * 2];
			readpoints = new int[laps * 2];

			memset(writepoints, 0, sizeof(int) * laps * 2);
			memset(readpoints, 0, sizeof(int) * laps * 2);

			for (int i = 0; i < 2 * (int)laps; i++) // initialize half of writepoints
				writepoints[i] = -i * (int)stride;

			reading = new bool[laps * 2];
			writing = new bool[laps * 2];

			memset(reading, false, sizeof(bool) * laps * 2);
			memset(writing, true, sizeof(bool) * laps * 2);

		}

		~Fourier()
		{
			delete [] writepoints;
			delete [] readpoints;
			delete [] reading;
			delete [] writing;
		}

		// writes a single sample (with windowing) into the in array
		void write(T x, arm_rfft_fast_instance_f32 * fft)
		{
			for (size_t i = 0; i < laps * 2; i++)
			{
				if (writing[i])
				{
					if (writepoints[i] >= 0)
					{
						T amp = (*window)((T)writepoints[i] / N);
						in[writepoints[i] + N * i] = amp * x;
					}
					writepoints[i]++;

					if (writepoints[i] == N)
					{
						writing[i] = false;
						reading[i] = true;
						readpoints[i] = 0;

						forward(i, fft); // FTs ith in to ith middle buffer
						process(i, fft); // user-defined; ought to move info from ith middle to out buffer
						backward(i, fft); // IFTs ith out to ith in buffer

						current = i;
					}
				}
			}
		}

		inline void forward(const size_t i, arm_rfft_fast_instance_f32 * fft)
		{
			// fft->Direct((in + i * N), (middle + i * N)); // analysis
			arm_rfft_fast_f32(fft, in + i * N, middle + i * N, 0);
		}

		inline void backward(const size_t i, arm_rfft_fast_instance_f32 * fft)
		{
			// fft->Inverse((out + i * N), (in + i * N)); // synthesis
			arm_rfft_fast_f32(fft, out + i * N, in + i * N, 1);
		}

		// executes user-defined callback
		inline void process(const size_t i, arm_rfft_fast_instance_f32 * fft)
		{
			processor((middle + i * N), (out + i * N));
		}

		// read a single reconstructed sample
		T read()
		{
			T accum = 0;

			for (size_t i = 0; i < laps * 2; i++)
			{
				if (reading[i])
				{
					T amp = (*window)((T)readpoints[i] / N);
					accum += amp * in[readpoints[i] + N * i];

					readpoints[i]++;

					if (readpoints[i] == N)
					{
						writing[i] = true;
						reading[i] = false;
						writepoints[i] = 0;
					}
				}
			}

			accum /= N * laps / 2.0;
			return accum;
		}



	private:
		T *in, *middle, *out;

	public:
		// ShyFFT<T, N, RotationPhasor>* fft;
		// arm_rfft_fast_instance_f32* fft;

		Wave<T>* window;

		size_t laps;
		size_t stride;

		int* writepoints;
		int* readpoints;
		bool* reading;
		bool* writing;

		int current = 0;
	};
}

#define FOURIER
#endif