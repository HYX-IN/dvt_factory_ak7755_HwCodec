/*
 *
 * idsp_test
 *
 * History:
 *	2015/09/10 - [Jingyang Qiu] Created this file
 *
 * Description :
 *	Load the tuned file which generated by Img tuning tool.
 *
 * Copyright (C) 2015 Ambarella, Inc.
 *
 * This file and its contents ("Software") are protected by intellectual
 * property rights including, without limitation, U.S. and/or foreign
 * copyrights. This Software is also the confidential and proprietary
 * information of Ambarella, Inc. and its licensors. You may not use, reproduce,
 * disclose, distribute, modify, or otherwise prepare derivative works of this
 * Software or any portion thereof except pursuant to a signed license agreement
 * or nondisclosure agreement with Ambarella, Inc. or its authorized affiliates.
 * In the absence of such an agreement, you agree to promptly notify and return
 * this Software to Ambarella, Inc.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#include <getopt.h>
#include <errno.h>
#include <basetypes.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/signal.h>

#include <iav_ioctl.h>


//#define VIN_ARRAY_SIZE	(8)
//static char * VIN_VSYNC = "/proc/ambarella/vin0_vsync";


static const char* short_options = "f:d:w:h:src:ym";
static struct option long_options[] = {
	{"file", 1, 0, 'f'},
	{"stat", 0, 0, 's'},
	{"raw", 0, 0, 'r'},
	{"yuv", 0, 0, 'y'},
	{"me1", 0, 0, 'm'},
	{"decode-width", 1, 0, 'd'},
	{"width", 1, 0, 'w'},
	{"height", 1, 0, 'h'},
	{"count", 1, 0, 'c'},
	};
//static int raw_file_mode;
static int raw_cap_mode;
static int yuv_cap_mode;
static int me1_cap_mode;

static int stat_read_mode;
static char file_name[128];
static int raw_width;
static int raw_height;
static int cap_count;

static u8* dsp_mem;
static int dsp_size;

static int raw_init_param(int argc, char **argv)
{
	int ch;
	int option_index = 0;

	opterr = 0;
	while ((ch = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
		switch(ch) {
		case 'f':
			memcpy(file_name, optarg, 128);
			break;
//		case 'd':
//			decode_width = atoi(optarg);
//			break;
		case 'w':
			raw_width = atoi(optarg);
			break;
		case 'h':
			raw_height = atoi(optarg);
			break;
		case 's':
			stat_read_mode = 1;
			break;
		case 'r':
			raw_cap_mode = 1;
			break;
		case 'y':
			yuv_cap_mode = 1;
			break;
		case 'm':
			me1_cap_mode = 1;
			break;
		case 'c':
			cap_count = atoi(optarg);
			break;
		default:
			printf("unknown option %c\n", ch);
			return -1;
		}
	}

	return 0;
}

static int get_cap_size(int fd_iav, struct iav_querydesc* desc)
{
	if (ioctl(fd_iav, IAV_IOC_QUERY_DESC, desc) < 0) {
		perror("IAV_IOC_QUERY_DESC");
		return -1;
	}

	return 0;
}

#if 0
static int get_raw_size(int fd_iav, iav_raw_info_t* p_raw_info)
{

	if (ioctl(fd_iav, IAV_IOC_READ_RAW_INFO, p_raw_info) < 0) {
		perror("IAV_IOC_IMG_READ_RAW_INFO");
		return -1;
	}

	printf("raw pattern %d width %d height %d\n", p_raw_info->bayer_pattern, p_raw_info->width, p_raw_info->height);
	return 0;
}

static int get_yuv_size(int fd_iav, iav_yuv_buffer_info_ex_t* yuv_info)
{
	yuv_info->source = 0;
	if (ioctl(fd_iav, IAV_IOC_READ_YUV_BUFFER_INFO_EX, yuv_info) < 0) {
		perror("IAV_IOC_READ_YUV_BUFFER_INFO_EX");
		return -1;
	}

	printf("yuv width %d height %d, pitch %d format %d\n", yuv_info->width, yuv_info->height, yuv_info->pitch, yuv_info->format);
	return 0;
}

static int get_me1_size(int fd_iav, iav_me1_buffer_info_ex_t* me1_info)
{
	me1_info->source = 0;
	if (ioctl(fd_iav, IAV_IOC_READ_ME1_BUFFER_INFO_EX, me1_info) < 0) {
		perror("IAV_IOC_READ_ME1_BUFFER_INFO_EX");
		return -1;
	}

	printf("me1 width %d height %d, pitch %d\n", me1_info->width, me1_info->height, me1_info->pitch);
	return 0;
}
#endif

static int save_2file(u8* src, int size, int cnt, char* fn, char* suffix)
{
	int fd_raw;
	char pic_file_name[128];

	sprintf(pic_file_name, "%s_%d.%s", fn, cnt, suffix);
	fd_raw = open(pic_file_name, O_WRONLY | O_CREAT, 0666);
	if (write(fd_raw, src, size) < 0) {
		perror("write(save_raw)");
		return -1;
	}

	printf("%s picture written to %s\n", suffix, pic_file_name);
	close(fd_raw);
	return 0;
}

static int save_raw2mem(int fd_iav, u8* mem)
{
	struct iav_querydesc desc;
	struct iav_rawbufdesc* raw_desc;
	int size;

	memset(&desc, 0, sizeof(desc));
	desc.qid = IAV_DESC_RAW;
	if (ioctl(fd_iav, IAV_IOC_QUERY_DESC, &desc) < 0) {
		perror("IAV_IOC_QUERY_DESC");
		return -1;
	}
	raw_desc = &desc.arg.raw;
	size = raw_desc->pitch*raw_desc->height;
	memcpy(mem, dsp_mem+raw_desc->raw_addr_offset, size);

	return 0;
}

static int save_yuv2mem(int fd_iav, u8* mem)
{
	struct iav_querydesc desc;
	struct iav_yuvbufdesc* yuv_desc;
	u8 *in = NULL, *out = NULL;
	int i;
	int width, height, pitch;

	memset(&desc, 0, sizeof(desc));
	desc.qid = IAV_DESC_YUV;
	desc.arg.yuv.buf_id = IAV_SRCBUF_MN;

	if (ioctl(fd_iav, IAV_IOC_QUERY_DESC, &desc) < 0) {
		perror("IAV_IOC_QUERY_DESC");
		return -1;
	}

	yuv_desc = &desc.arg.yuv;

	if (yuv_desc->pitch == yuv_desc->width) {
		memcpy(mem, dsp_mem+yuv_desc->y_addr_offset, yuv_desc->width * yuv_desc->height);
		out = mem + yuv_desc->width * yuv_desc->height;
	}
	else {
		in = dsp_mem + yuv_desc->y_addr_offset;
		out = mem;
		for (i = 0; i < yuv_desc->height; i++) {		//row
			memcpy(out, in, yuv_desc->width);
			in += yuv_desc->pitch;
			out += yuv_desc->width;
		}
	}

	width = yuv_desc->width / 2;
	height = yuv_desc->height / 2;
	pitch = yuv_desc->pitch / 2;

	for (i = 0; i < height; ++i)
		memcpy(out + i * width * 2, dsp_mem + yuv_desc->uv_addr_offset + i * pitch * 2, width * 2);


	return 0;
}

static int save_me12mem(int fd_iav, u8* mem)
{
	struct iav_querydesc desc;
	struct iav_mebufdesc* me1_desc;
	u8 *in = NULL, *out = NULL;
	int i;
//	int width, height, pitch;

	memset(&desc, 0, sizeof(desc));
	desc.qid = IAV_DESC_ME1;
	desc.arg.me1.buf_id = IAV_SRCBUF_MN;

	if (ioctl(fd_iav, IAV_IOC_QUERY_DESC, &desc) < 0) {
		perror("IAV_IOC_QUERY_DESC");
		return -1;
	}
	me1_desc = &desc.arg.me1;

	if (me1_desc->pitch == me1_desc->width) {
		memcpy(mem, dsp_mem + me1_desc->data_addr_offset, me1_desc->width * me1_desc->height);
	} else {
		in = dsp_mem + me1_desc->data_addr_offset;
		out = mem;
		for (i = 0; i < me1_desc->height; i++) {		//row
			memcpy(out, in, me1_desc->width);
			in += me1_desc->pitch;
			out += me1_desc->width;
		}
	}

	return 0;

}

int map_bsb(int fd_iav)
{
	struct iav_querybuf querybuf;

	querybuf.buf = IAV_BUFFER_DSP;
	if (ioctl(fd_iav, IAV_IOC_QUERY_BUF, &querybuf) < 0) {
		perror("IAV_IOC_QUERY_BUF");
		return -1;
	}

	dsp_size = querybuf.length;
	dsp_mem = mmap(NULL, dsp_size, PROT_READ, MAP_SHARED, fd_iav, querybuf.offset);
	if (dsp_mem == MAP_FAILED) {
		perror("mmap (%d) failed: %s\n");
		return -1;
	}

	return 0;
}



u8* cap_buf;
u32 cap_size;
int main(int argc, char **argv)
{
	int fd_iav;//, vin_tick;
//	char vin_int_array[VIN_ARRAY_SIZE];
	struct iav_querydesc query_desc;
	struct iav_rawbufdesc *raw_desc;
	struct iav_yuvbufdesc *yuv_desc;
	struct iav_mebufdesc *me1_desc;

	char suffix[8];

	memset(&query_desc, 0, sizeof(query_desc));

	if(raw_init_param(argc, argv)<0 || argc<2)
		return -1;

	if ((fd_iav = open("/dev/iav", O_RDWR, 0)) < 0) {
		perror("/dev/iav");
		return -1;
	}
	map_bsb(fd_iav);
//	vin_tick = open(VIN_VSYNC, O_RDONLY);

	if(raw_cap_mode) {
		query_desc.qid = IAV_DESC_RAW;
		if(get_cap_size(fd_iav,&query_desc)<0)
			return -1;
		raw_desc = &query_desc.arg.raw;
		cap_size = raw_desc->pitch*raw_desc->height;
		sprintf(suffix, "raw");
		printf("%s width %d height %d pitch %d\n", suffix, raw_desc->width, raw_desc->height, raw_desc->pitch);
	}
	else if(yuv_cap_mode){
		query_desc.qid = IAV_DESC_YUV;
		if(get_cap_size(fd_iav,&query_desc)<0)
			return -1;
		yuv_desc = &query_desc.arg.yuv;
		cap_size = yuv_desc->width*yuv_desc->height*3/2;
		sprintf(suffix, "yuv");
		printf("%s width %d height %d pitch %d\n", suffix, yuv_desc->width, yuv_desc->height, yuv_desc->pitch);
	}
	else if(me1_cap_mode) {
		query_desc.qid = IAV_DESC_ME1;
		if(get_cap_size(fd_iav,&query_desc)<0)
			return -1;
		me1_desc = &query_desc.arg.me1;
		cap_size = me1_desc->width*me1_desc->height;
		sprintf(suffix, "me1");
		printf("%s width %d height %d pitch %d\n", suffix, me1_desc->width, me1_desc->height, me1_desc->pitch);
	}

	printf("allocate mem for %d x %d\n", cap_size, cap_count);
	cap_buf = malloc(cap_size* cap_count);
	if(cap_buf == NULL) {
		return -1;
	}

	memset(cap_buf, 0,cap_size* cap_count);
	if(raw_cap_mode || yuv_cap_mode || me1_cap_mode) {
		int i=0;
		u8* wp = cap_buf;
		for(i=0; i<cap_count; i++) {
			if(raw_cap_mode) {
				save_raw2mem(fd_iav, wp);
			}
			else if(yuv_cap_mode){
				save_yuv2mem(fd_iav, wp);
			}
			else if(me1_cap_mode) {
				save_me12mem(fd_iav, wp);
			}
			wp += cap_size;
		}

		wp = cap_buf;
		for(i=0; i<cap_count; i++) {
			save_2file(wp, cap_size, i, file_name, suffix);
			wp += cap_size;
		}

	}

	free(cap_buf);
	return 0;
}

