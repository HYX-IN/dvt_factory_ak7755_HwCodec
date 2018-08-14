 /*
 * History:
 *	2016/12/30 - [JianTang] Created file
 *
 * Copyright (c) 2016 Ambarella, Inc.
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

/*
 * getopt.c
 *
 * a minimal implementation of the getopt() function, written so that
 * test applications that use that function can run on non-POSIX
 * platforms
 *
 */
/*
 *
 * Copyright (c) 2001-2006 Cisco Systems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials provided
 *   with the distribution.
 *
 *   Neither the name of the Cisco Systems, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdlib.h>  /* for NULL */

#ifdef __cplusplus
extern "C" {
#endif

int optind_s = 0;

char *optarg_s;

#define GETOPT_FOUND_WITHOUT_ARGUMENT    2
#define GETOPT_FOUND_WITH_ARGUMENT       1
#define GETOPT_NOT_FOUND                 0

static int
getopt_check_character(char c, const char *string) {
  unsigned int max_string_len = 128;

  while (*string != 0) {
    if (max_string_len == 0) {
      return '?';
    }
    if (*string++ == c) {
      if (*string == ':') {
	return GETOPT_FOUND_WITH_ARGUMENT;
      } else {
	return GETOPT_FOUND_WITHOUT_ARGUMENT;
      }
    }
  }
  return GETOPT_NOT_FOUND;
}

int
getopt_s(int argc,
       char * const argv[],
       const char *optstring) {


  while (optind_s + 1 < argc) {
    char *string;

    /* move 'string' on to next argument */
    optind_s++;
    string = argv[optind_s];

    if (string == NULL)
      return '?'; /* NULL argument string */

    if (string[0] != '-')
      return -1; /* found an unexpected character */

    switch(getopt_check_character(string[1], optstring)) {
    case GETOPT_FOUND_WITH_ARGUMENT:
      if (optind_s + 1 < argc) {
	optind_s++;
	optarg_s = argv[optind_s];
	return string[1];
      } else {
	return '?';  /* argument missing */
      }
    case GETOPT_FOUND_WITHOUT_ARGUMENT:
      return string[1];
    case GETOPT_NOT_FOUND:
    default:
      return '?'; /* didn't find expected character */
      break;
    }
  }

  return -1;
}
#ifdef __cplusplus
}
#endif
