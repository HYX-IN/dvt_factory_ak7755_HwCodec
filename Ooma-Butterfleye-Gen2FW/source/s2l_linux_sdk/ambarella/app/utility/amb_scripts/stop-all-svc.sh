#!/bin/sh
## History:
##
## 2016/12/30 - [JianTang] Created file
##
## Copyright (c) 2016 Ambarella, Inc.
##
## This file and its contents ("Software") are protected by intellectual
## property rights including, without limitation, U.S. and/or foreign
## copyrights. This Software is also the confidential and proprietary
## information of Ambarella, Inc. and its licensors. You may not use, reproduce,
## disclose, distribute, modify, or otherwise prepare derivative works of this
## Software or any portion thereof except pursuant to a signed license agreement
## or nondisclosure agreement with Ambarella, Inc. or its authorized affiliates.
## In the absence of such an agreement, you agree to promptly notify and return
## this Software to Ambarella, Inc.
##
## THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
## INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
## MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
## IN NO EVENT SHALL AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
## INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
## (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
## LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
## INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
## CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.
##
#
# stop all services
#

timeout()
{
	second=10
	command=$*
	$command &
	commandpid=$!
	( sleep $second ; kill -9 "$commandpid" > /dev/null 2>&1 ) &
	wait "$commandpid" > /dev/null 2>&1
}

stop_all()
{
	echo "stop all services"
	app_pid=$(pgrep apps_launcher)
	audio_svc=$(pgrep audio_svc)
	video_svc=$(pgrep video_svc)
	media_svc=$(pgrep media_svc)
	event_svc=$(pgrep event_svc)
	img_svc=$(pgrep img_svc)
	sys_svc=$(pgrep sys_svc)
	if [ "$app_pid" ]; then
		kill -2 "$app_pid"
		kill -15 "$app_pid"
	fi

	echo "apps_launcher or services are"
	while [ "$app_pid" -o "$audio_svc" -o "$video_svc" -o "$media_svc" -o "$event_svc" -o "$img_svc" -o "$sys_svc" ]
	do
		if [ "$app_pid" ]; then
			echo "apps_launcher is still running"
		else
			echo "stopped"
			return 0
		fi
		app_pid=$(pgrep apps_launcher)
		audio_svc=$(pgrep audio_svc)
		video_svc=$(pgrep video_svc)
		media_svc=$(pgrep media_svc)
		event_svc=$(pgrep event_svc)
		img_svc=$(pgrep img_svc)
		sys_svc=$(pgrep sys_svc)
		sleep 1
	done
	echo "done"
}

check_svc_status()
{
	DATE=$(date '+%Y-%m-%d-%H:%M:%S')
	LOG_FILE=/var/log/stop_services_$DATE.log
	app_pid=$(pgrep apps_launcher)
	audio_svc=$(pgrep audio_svc)
	video_svc=$(pgrep video_svc)
	media_svc=$(pgrep media_svc)
	event_svc=$(pgrep event_svc)
	img_svc=$(pgrep img_svc)
	sys_svc=$(pgrep img_svc)

	if [ "$app_pid" ]; then
		echo "apps_launcher stopped failed" >> "$LOG_FILE"
		echo "below service still running:" >> "$LOG_FILE"
	else
		echo "apps_launcher stop successfully" >> "$LOG_FILE"
		return 0;
	fi
	if [ "$audio_svc" ]; then
		echo "audio_svc " >> "$LOG_FILE"
	fi
	if [ "$video_svc" ]; then
		echo "video_svc " >> "$LOG_FILE"
	fi
	if [ "$media_svc" ]; then
		echo "media_svc " >> "$LOG_FILE"
	fi
	if [ "$event_svc" ]; then
		echo "event_svc " >> "$LOG_FILE"
	fi
	if [ "$img_svc" ]; then
		echo "img_svc " >> "$LOG_FILE"
	fi
	if [ "$sys_svc" ]; then
		echo "sys_svc " >> "$LOG_FILE"
	fi
	echo "check all services status done"
}

shutdown()
{
	timeout stop_all
	check_svc_status
	#remove pid files to avoid restart services failed
	rm /tmp/*.pid
}


case "$1" in
	shutdown)
		echo "stop services before shutdown"
		shutdown
		;;
	*)
		echo "stop services"
		timeout stop_all
		check_svc_status
		;;
esac
\00\00
