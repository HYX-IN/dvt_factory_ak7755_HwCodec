##
## /S2l/oryx/stream/playback/engine/make.inc
##
## History:
##   2014-10-24 - [ypchang] created file
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

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

PLAYBACK_ENGINE_TARGET    := libamstream-engine-playback.so
PLAYBACK_ENGINE_CONF       = playback-engine.acs
PLAYBACK_ENGINE_VER_FILE   = $(ORYX_DIR)/include/stream/version.h
PLAYBACK_ENGINE_VER_PREFIX = PLAYBACK

LOCAL_TARGET := $(PLAYBACK_ENGINE_TARGET)
LOCAL_SRCS   := $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_CFLAGS := -I$(LOCAL_PATH)                       \
                -I$(ORYX_DIR)/include                 \
                -I$(ORYX_DIR)/include/audio           \
                -I$(ORYX_DIR)/include/audio/codec     \
                -I$(ORYX_DIR)/include/configure       \
                -I$(ORYX_DIR)/include/utility         \
                -I$(ORYX_DIR)/include/stream          \
                -I$(ORYX_DIR)/stream/include/amf      \
                -I$(ORYX_DIR)/stream/include/common   \
                -I$(ORYX_DIR)/stream/include/playback \
                -fvisibility=hidden -std=c++11 -Werror

LOCAL_LIBS := libamutil.so       \
              libamosal.so       \
              libamconfig.so     \
              libamstream-iid-playback.so

PLAYBACK_ENGINE_SO_VER_MAJ := $(shell awk '/define\ $(PLAYBACK_ENGINE_VER_PREFIX)_LIB_MAJOR/{print $$3}' $(PLAYBACK_ENGINE_VER_FILE))
PLAYBACK_ENGINE_SO_VER_MIN := $(shell awk '/define\ $(PLAYBACK_ENGINE_VER_PREFIX)_LIB_MINOR/{print $$3}' $(PLAYBACK_ENGINE_VER_FILE))
PLAYBACK_ENGINE_SO_VER_PAT := $(shell awk '/define\ $(PLAYBACK_ENGINE_VER_PREFIX)_LIB_PATCH/{print $$3}' $(PLAYBACK_ENGINE_VER_FILE))
PLAYBACK_ENGINE_SO_VER_STR := $(PLAYBACK_ENGINE_SO_VER_MAJ).$(PLAYBACK_ENGINE_SO_VER_MIN).$(PLAYBACK_ENGINE_SO_VER_PAT)

PLAYBACK_ENGINE_SO_NAME   := $(patsubst %.so,%.so.$(PLAYBACK_ENGINE_SO_VER_MAJ), $(PLAYBACK_ENGINE_TARGET))
PLAYBACK_ENGINE_SO_TARGET := $(patsubst %.so,%.so.$(PLAYBACK_ENGINE_SO_VER_STR), $(PLAYBACK_ENGINE_TARGET))

LOCAL_SO_NAME := $(PLAYBACK_ENGINE_SO_NAME)

include $(BUILD_APP)

.PHONY: $(LOCAL_TARGET)

$(LOCAL_TARGET): filter-demuxer.so filter-decoder.so filter-player.so
$(LOCAL_TARGET): PLAYBACK_ENGINE_SRC_PATH := $(LOCAL_PATH)
$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(ORYX_LIB_DIR)/
	@mkdir -p $(ORYX_CONF_DIR)/stream/engine/
	@cp -dpRf $(PLAYBACK_ENGINE_SRC_PATH)/$(PLAYBACK_ENGINE_CONF) $(ORYX_CONF_DIR)/stream/engine/$(PLAYBACK_ENGINE_CONF)
	@cp -dpRf $< $(ORYX_LIB_DIR)/$(PLAYBACK_ENGINE_SO_TARGET)
	@ln -sf $(PLAYBACK_ENGINE_SO_TARGET) $(ORYX_LIB_DIR)/$(PLAYBACK_ENGINE_SO_NAME)
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))