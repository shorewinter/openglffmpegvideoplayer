extern "C" {
	#include "libavcodec/avcodec.h"
	#include "libavformat/avformat.h"
	#include "libswscale/swscale.h"
}

bool loadFrame(const char* filename, int* widthOut, int* heightOut, unsigned char* dataOut[]) {
	AVFormatContext* avFormatContext = avformat_alloc_context();
	if(!avFormatContext) {
		printf("ERROR: avFormat_alloc_context failed\n");
		return false;
	}

	if(avformat_open_input(&avFormatContext, filename, NULL, NULL) != 0) {
		printf("ERROR: avformat_open_input failed\n");
		return false;
	}

	int videoStreamIndex = -1;
	AVCodecParameters* avCodecParameters;
	AVCodec* avCodec;
	for(int i = 0; i < avFormatContext->nb_streams; ++i) {
		avCodecParameters = avFormatContext->streams[i]->codecpar;
		avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
		if(!avCodec) {
			continue;
		}

		if(avCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStreamIndex = i;
			break;
		}
	}

	if(videoStreamIndex == -1) {
		printf("ERROR: no video streams found\n");
		return false;
	}

	AVCodecContext* avCodecContext = avcodec_alloc_context3(avCodec);
	if(!avCodecContext) {
		printf("ERROR: avcodec_alloc_context3 failed\n");
		return false;
	}

	if(avcodec_parameters_to_context(avCodecContext, avCodecParameters) < 0) {
		printf("ERROR: avcodec_parameters_to_context failed\n");
		return false;
	}

	if(avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
		printf("ERROR: avcodec_open2 failer\n");
		return false;
	}

	AVFrame* avFrame = av_frame_alloc();
	if(!avFrame) {
		printf("ERROR: av_frame_alloc failed\n");
		return false;
	}

	AVPacket* avPacket = av_packet_alloc();
	if(!avPacket) {
		printf("ERROR: av_packet_alloc failed\n");
		return false;
	}
	int response;
	while(av_read_frame(avFormatContext, avPacket) >= 0) {
		if(avPacket->stream_index != videoStreamIndex) continue;
		response = avcodec_send_packet(avCodecContext, avPacket);
		if(response < 0) {
			printf("ERROR: avcodec_send_packet failed\n");
			return false;
		}
		response = avcodec_receive_frame(avCodecContext, avFrame);
		if(response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
			continue;
		} else if(response < 0) {
			printf("ERROR: Failed to decode packet\n");
			return false;
		}

		av_packet_unref(avPacket);
		break;
	}

	unsigned char* data = new unsigned char[avFrame->width * avFrame->height * 3];
	for(int x = 0; x < avFrame->width; ++x) {
		for(int y = 0; y < avFrame->height; ++y) {
			data[y * avFrame->width * 3 + x * 3	   ] = avFrame->data[0][y * avFrame->linesize[0] + x];
			data[y * avFrame->width * 3 + x * 3 + 1] = avFrame->data[0][y * avFrame->linesize[0] + x];
			data[y * avFrame->width * 3 + x * 3 + 2] = avFrame->data[0][y * avFrame->linesize[0] + x];
		}
	}

	*widthOut = avFrame->width;
	*heightOut = avFrame->height;
	*dataOut = data;
	
	avformat_close_input(&avFormatContext);
	avformat_free_context(avFormatContext);
	av_frame_free(&avFrame);
	av_packet_free(&avPacket);
	avcodec_free_context(&avCodecContext);

	return true;
}