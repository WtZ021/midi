/*
 * main.cc
 *
 *  Created on: 27 янв. 2020 г.
 *      Author: Wertiz
 */

	#include <iostream>
	#include <iomanip>
	#include <chrono>
	#include <fstream>
	#ifdef __WIN32__
	#include <winsock.h>
	#else
	#include <netinet/in.h>
	#endif
	#include <vector>
	#include <cmath>

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_main.h>

	#include "AudioDriver.h"
	#include "SoundSource.h"
	#include "FMWaveGenerator.h"

/*int main(int, char**)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	FMInstrument inst
	{ 4, 2, {
			// at,dr,sl,sr,rr,dt,beta, multiplier
		{1., 0.001, 0.9, 0.0001, 0.001, 0., 3 , 2.},
		{10., 0.1, 0.98, 0.000005, 0.0001, 0., 0.5, 1.},
		{1., 0.001, 0.9, 0.0001, 0.001, 0., 3, 2.},
		{1., 0.1, 0.98, 0.000005, 0.0001, 0., 0.5, 0.5} } };



	AudioDriver drv;


	auto src = std::make_shared<SoundSource>();
	auto wg = std::make_shared<SineWaveGenerator>();
	auto eg = std::make_shared<ConstantEnvelopeGenerator>(1.);
	auto fm = std::make_shared<FMWaveGenerator>();

	fm->set_freq(440.);
	fm->setup(inst);

	src->set_envelope_generator(eg);
	src->set_wave_generator(fm);


	drv.set_sound_source(src);
	drv.unpause();
	drv.set_master_volume(4000);
	fm->key_on();

	auto win = std::shared_ptr<SDL_Window>(
			SDL_CreateWindow("", 50, 50, 800, 600, SDL_WINDOW_SHOWN),
			SDL_DestroyWindow);
	SDL_Event ev;
	for (;;) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) goto the_exit;
		}

		SDL_Delay(10);
	}

the_exit:
*/

	/*
	 * midi_parser.cc
	 */



	int main(int, char**)
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		static const double note_gz[] = {
	32.70, 		34.65, 		36.71, 		38.89, 		41.20, 		43.65, 		46.25, 		49., 		51.91, 		55., 		58.27, 		61.74,
	65.41, 		69.30, 		73.42, 		77.78, 		82.41, 		87.31, 		92.50, 		98.00, 		103.83, 	110.00, 	116.54, 	123.47,
	130.81, 	138.59, 	146.83, 	155.56, 	164.81, 	174.61, 	185.00, 	196.00, 	207.65, 	220.00, 	233.08, 	246.94,
	261.63, 	277.18, 	293.66, 	311.13, 	329.63, 	349.23, 	369.99, 	392.00, 	415.30, 	440.00, 	466.16, 	493.88,
	523.25, 	554.37, 	587.33, 	622.25, 	659.26, 	698.46, 	739.99, 	783.99, 	830.61, 	880.00, 	932.33, 	987.77,
	1046.50, 	1108.73, 	1174.66, 	1244.51, 	1318.51, 	1396.91, 	1479.98, 	1567.98, 	1661.22, 	1760.00, 	1864.66, 	1975.53,
	2093.00, 	2217.46, 	2349.32, 	2489.02, 	2637.02, 	2793.83, 	2959.96, 	3135.96, 	3322.44, 	3520.00, 	3729.31, 	3951.07,
	4186.01, 	4434.92, 	4698.64, 	4978.03, 	5274.04, 	5587.65, 	5919.91, 	6271.93, 	6644.88, 	7040.00, 	7458.62, 	7902.13,
	8372.02, 	8869.84, 	9397.27, 	9956.06, 	10548.08, 	11175.30, 	11839.82, 	12543.85, 	13289.75, 	14080.00, 	14917.24, 	15804.27,
	16744.04, 	17739.69, 	18794.55, 	19912.13, 	21096.16, 	22350.61, 	23679.64, 	25087.71, 	26579.50, 	28160.00, 	29834.48, 	31608.53,
	33488.07, 	35479.38, 	37589.09, 	39824.25, 	42192.33, 	44701.21, 	47359.29, 	50175.42, 	53159.00, 	56320.00, 	59668.96, 	63217.06
		};


		FMInstrument inst
				{ 4, 2, {
				//	at, dr, sl, sr, rr, dt, beta, multiplier
					{5., 0.001, 0.5, 0.0001, 0.001, 0., 0.1 , 0.05},
					{10., 0.1, 0.98, 0.000005, 0.0001, 0., 0.5, 1.},
					{5., 0.001, 0.5, 0.0001, 0.001, 0., 0.1 , 0.05},
					{10., 0.1, 1., 0.00009, 0.0001, 0.9, 0.5, 0.5} } };


		AudioDriver drv;
		drv.unpause();
		auto src = std::make_shared<SoundSource>();
		drv.set_sound_source(src);
		drv.set_master_volume(4000);

		std::vector<std::vector<char>> track;

		std::fstream file;

		auto win = std::shared_ptr<SDL_Window>(
					SDL_CreateWindow("", 50, 50, 400, 200, SDL_WINDOW_SHOWN),
					SDL_DestroyWindow);



		file.open("MIDI_sample.mid", std::ios::in | std::ios::binary);

		int chunk_type;
		int MThd_length;
		short MThd_format;
		short MThd_tracks;
		short MThd_division;



		file.read(reinterpret_cast<char*>(&chunk_type), sizeof(chunk_type));
		if (chunk_type == (
				'M'*0x00000001 +
				'T'*0x00000100 +
				'h'*0x00010000 +
				'd'*0x01000000))
			std::cout << "О, это заголовок!" << std::endl;

		file.read(reinterpret_cast<char*>(&MThd_length), sizeof(MThd_length));
		MThd_length = ntohl(MThd_length);
		std::cout << "MThd length: " << MThd_length << std::endl;

		file.read(reinterpret_cast<char*>(&MThd_format), sizeof(MThd_format));
		MThd_format = ntohs(MThd_format);
		std::cout << "MThd format: " << MThd_format << std::endl;

		file.read(reinterpret_cast<char*>(&MThd_tracks), sizeof(MThd_tracks));
		MThd_tracks = ntohs(MThd_tracks);
		std::cout << "MThd tracks: " << MThd_tracks << std::endl;

		file.read(reinterpret_cast<char*>(&MThd_division), sizeof(MThd_division));
		MThd_division = ntohs(MThd_division);
		std::cout << "MThd division: " << MThd_division << std::endl;
		if (MThd_division & 0x8000) {
			std::cout << "Единица измерения - SMPTE" << std::endl;
		} else {
			std::cout << "Единица измерения - TPQN" << std::endl;
		}

		track.resize(MThd_tracks);

		for (int i = 0; i < MThd_tracks; i++) {
			file.read(reinterpret_cast<char*>(&chunk_type), sizeof(chunk_type));
			if (chunk_type == (
					'M'*0x00000001 +
					'T'*0x00000100 +
					'r'*0x00010000 +
					'k'*0x01000000)) {
				std::cout << "О, а это дорожка " << i << std::endl;
			} else {
				std::cout << "Ой, это не дорожка :(" << std::endl;
				break;
			}
			int trk_length;
			file.read(reinterpret_cast<char*>(&trk_length), sizeof(int));
			trk_length = ntohl(trk_length);
			std::cout << "  Длина: " << trk_length << std::endl;
			track[i].resize(trk_length);
			file.read(&track[i][0], trk_length);
		}

		size_t trk = 0;


		std::cout << "Для демонстрации будем работать только с дорожкой " << trk << std::endl;



		size_t p = 0;
		unsigned note_now;
		while (p < track[trk].size()) {
			unsigned delta = 0;
			unsigned delta_v;
			do {
			  delta_v = unsigned(track[trk][p++]) & 0xffU;
			  delta *= 128U;
			  delta += delta_v & 0x7fU;
			} while (delta_v & 0x80U);
			std::cout << std::dec << std::setw(10) << delta << ": ";

			SDL_Delay(delta);

			unsigned event_type = unsigned (track[trk][p++]) & 0xffU;
			char old_event_type;

			std::cout << std::hex << event_type;
			if ((event_type & 0x80) == 0) {
			  --p;
			  event_type = old_event_type & 0xffU;
			} else {
			  old_event_type = event_type & 0xffU;
			}

			switch(event_type) {
				case 0xff: {
					std::cout << std::endl << " Это метасобытие ";
					unsigned ev = unsigned(track[trk][p++]) & 0xffU;
					std::cout << std::hex << ev ;
					switch(ev) {
						case 0x00: {
							unsigned next = track[trk][p++] & 0xffU;
							std::cout << " - Sequence Number " << next << std::endl;
						} break;
						case 0x01:{
							unsigned len = track[trk][p++];
							std::string str;
							while (len--) str += track[trk][p++];
							std::cout << " - Text Event: " << str << std::endl;
						} break;
						case 0x02:{
							unsigned len = track[trk][p++];
							std::string str;
							while (len--) str += track[trk][p++];
							std::cout << " - Copyright Notice: " << str << std::endl;
						} break;
						case 0x03: {
							unsigned len = track[trk][p++];
							std::string str;
							while (len--) str += track[trk][p++];
							std::cout << " - Track Name: " << str << std::endl;
						} break;
						case 0x04:{
							unsigned len = track[trk][p++];
							std::string str;
							while (len--) str += track[trk][p++];
							std::cout << " - Instrument Name: " << str << std::endl;
						} break;
						case 0x05:{
							unsigned len = track[trk][p++];
							std::string str;
							while (len--) str += track[trk][p++];
							std::cout << " - Lyric: " << str << std::endl;
						} break;
						case 0x06:{
							unsigned len = track[trk][p++];
							std::string str;
							while (len--) str += track[trk][p++];
							std::cout << " - Marker: " << str << std::endl;
						} break;
						case 0x07:{
							unsigned len = track[trk][p++];
							std::string str;
							while (len--) str += track[trk][p++];
							std::cout << " - Cue Point: " << str << std::endl;
						} break;
						case 0x20: {
							unsigned len_ch = track[trk][p++];
							unsigned next_ch = track[trk][p++];
							std::cout << " - MIDI Channel Prefix " << len_ch  << "_" << next_ch << std::endl;
						} break;
						case 0x2f:
							std::cout << " - End of Track" << std::endl;
							break;
						case 0x51: {
							unsigned t_len = track[trk][p++] - 1;
							unsigned temp = track[trk][p++] & 0xffU;
							while (t_len--) temp = (temp << 8) | (track[trk][p++] & 0xffU);
							std::cout << " - Set Tempo " <<  std::dec << temp << " микросекунд" << std::endl;
						} break;
						case 0x54:{
							unsigned len_offset = track[trk][p++];
							unsigned hr = track[trk][p++] & 0xffU;
							unsigned mn = track[trk][p++] & 0xffU;
							unsigned se = track[trk][p++] & 0xffU;
							unsigned fr = track[trk][p++] & 0xffU;
							unsigned ff = track[trk][p++] & 0xffU;
							std::cout << " - SMPTE Offset: "<< len_offset << "_" << hr << "_" << mn << "_" << se << "_" << fr << "_" << ff << std::endl;
						} break;
						case 0x58: {
							unsigned len_time_s = track[trk][p++];
							unsigned razm_v = track[trk][p++] & 0xffU;
							unsigned razm_n = pow(2,track[trk][p++] & 0xffU);
							unsigned metronom = track[trk][p++] & 0xffU;
							unsigned doli = track[trk][p++] & 0xffU;
							std::cout << " - Time Signature:" <<  len_time_s   <<" Размер " << razm_v << "/"  << razm_n
									<< ", количество тиков в ударе метронома " <<  std::dec << metronom
									<< ", количество 32-х долей в 24 тиках " <<  std::dec << doli << std::endl;
						} break;
						case 0x59:{
							unsigned len_key_s = track[trk][p++];
							int sf = track[trk][p++];
							unsigned mi = track[trk][p++];
							std::cout << " - Key Signature: " << len_key_s <<" ";
							if (sf < 0) std::cout << sf*(-1) << " бемолей, "; else std::cout << sf << " диезов, ";
							if (mi==0) std::cout << "мажор" << std::endl; else std::cout << "минор" << std::endl;
						} break;
						case 0x7f:
							std::cout << "- Sequencer Specific Meta-Event" << std::endl;
							break;
						default: {
							std::cout << "хз, чини свой код" << std::endl;
							return 1;
						}
					}
					break;
				}

				case 0x80 ... 0x8f: {
					//unsigned chan = event_type & 0x0fU;
				    unsigned note = unsigned(track[trk][p++]) & 0x7fU;
				    unsigned velocity = unsigned(track[trk][p++]) & 0x7fU;
				    std::cout << " - Note Off " <<  std::hex << ", выключаем ноту " << note << " со скоростью "  << velocity << std::endl;

				    if(note_now == note) {
						auto fm = std::make_shared<FMWaveGenerator>();
						auto eg = std::make_shared<ConstantEnvelopeGenerator>(velocity);

						fm->set_freq(note_gz[note]);
						fm->setup(inst);
						src->set_envelope_generator(eg);
						src->set_wave_generator(fm);

						fm->key_off();
				    }

				} break;
				case 0x90 ... 0x9f: {

					//unsigned chan = event_type & 0x0fU;
					unsigned note = unsigned(track[trk][p++]) & 0x7fU;
					unsigned velocity = unsigned(track[trk][p++]) & 0x7fU;
					std::cout << " - Note On " <<  std::hex << ", включаем ноту " << note << " со скоростью " << velocity << std::endl;


					auto fm = std::make_shared<FMWaveGenerator>();
					auto eg = std::make_shared<ConstantEnvelopeGenerator>(velocity);

					fm->set_freq(note_gz[note]);
					fm->setup(inst);
					src->set_envelope_generator(eg);
					src->set_wave_generator(fm);

					fm->key_on();

					note_now = note;

				} break;

				case 0xa0 ... 0xaf:
					std::cout << " - полифонический Aftertouch" << std::endl;
					break;
				case 0xb0 ... 0xbf: {
					unsigned par1 = unsigned(track[trk][p++]) & 0xffU;
					unsigned par2 = unsigned(track[trk][p++]) & 0xffU;
					std::cout << " : " << par1 << " : " << par2 << " - Control Change"  << std::endl;
				} break;

				case 0xc0 ... 0xcf: {
					unsigned par1 = unsigned(track[trk][p++]) & 0xffU;
					std::cout << " : " << par1 << " - Program Change"  << std::endl;
				} break;

				case 0xd0 ... 0xdf: {
					std::cout << " - канальный Aftertouch" << std::endl;
				} break;
				case 0xe0 ... 0xef:{
					unsigned par1 = unsigned(track[trk][p++]) & 0xffU;
					unsigned par2 = unsigned(track[trk][p++]) & 0xffU;
					std::cout << " - Pitch Wheel Change: " << par1 << ", " << par2 << std::endl;
				} break;
				case 0xf0:
					std::cout << " - SysEx" << std::endl;
					break;
				case 0xf2: {
					unsigned par1 = unsigned(track[trk][p++]) & 0xffU;
					unsigned par2 = unsigned(track[trk][p++]) & 0xffU;
					std::cout << " - Song Position Pointer: " << par1 << ", " << par2 << std::endl;
				} break;
				case 0xf3:{
					unsigned par1 = unsigned(track[trk][p++]) & 0xffU;
					std::cout << " - Song Select: " << par1 << std::endl;
				} break;
				case 0xf6:
					std::cout << " - Tune Request" << std::endl;
					break;
				case 0xf8 or 0xfa or 0xfb or 0xfc or 0xfe :
					;
					break;
				default:{
					std::cout << std::endl;
				}
			}

		}


		file.close();


		return 0;
	}



