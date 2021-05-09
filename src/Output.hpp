#ifndef __OUTPUT_HPP__
#define __OUTPUT_HPP__

#include<string>
#include "Show.hpp"
#include "Encoder.hpp"
#include "Logging.hpp"
#include<unistd.h>

using namespace std;

enum class OutputType {
	InvalidType = -1,
	LocalFile = 0,
	RTMP
};
enum class OutputStatus {
	ERROR,
	OK
};
struct RTMPInfo{
	string stream;
	string key;
};
struct LocalFileInfo{
	string url;
};


class Output {

	private:
		OutputType outputType;
		Encoder *encoder=nullptr;
		obs_output_t*   output=nullptr;
		RTMPInfo *rtmp;
		LocalFileInfo *localFile;

    public:
		static RTMPInfo default_rtmp;
		static LocalFileInfo default_localFile;

		Output(OutputType _outputType,void *infoptr=nullptr);
		OutputStatus create_rtmp_output();
		OutputStatus create_ffmpeg_output();
		OutputStatus create_av_encoder();
		OutputStatus set_show(Show *show);
		OutputStatus start_output(bool display);

};

#endif //__OUTPUT_HPP__