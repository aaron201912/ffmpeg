/*
* XXX.c - Sigmastar
*
* Copyright (c) [2019~2020] SigmaStar Technology.
*
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License version 2 for more details.
*
*/

#include "st_uac_host.h"
#include <stdint.h>

static void audio_set_config(Audio_Handle_t *audio_handle, int mode)
{
    if(mode & AS_IN_MODE)
    {
        audio_handle->audio_info[0].flags = PCM_IN;
        audio_handle->audio_info[0].pcm_config.channels= 2;
        audio_handle->audio_info[0].pcm_config.rate= 48000;
        audio_handle->audio_info[0].pcm_config.period_size= 1024;
        audio_handle->audio_info[0].pcm_config.period_count= 4;
        audio_handle->audio_info[0].pcm_config.format= PCM_FORMAT_S16_LE;
        audio_handle->audio_info[0].pcm_config.start_threshold= 0;
        audio_handle->audio_info[0].pcm_config.stop_threshold= 0;
        audio_handle->audio_info[0].pcm_config.silence_threshold= 0;
    }

    if(mode & AS_OUT_MODE)
    {
        audio_handle->audio_info[1].flags = PCM_OUT;
        audio_handle->audio_info[1].pcm_config.channels= 2;
        audio_handle->audio_info[1].pcm_config.rate= 48000;
        audio_handle->audio_info[1].pcm_config.period_size= 1024;
        audio_handle->audio_info[1].pcm_config.period_count= 4;
        audio_handle->audio_info[1].pcm_config.format= PCM_FORMAT_S16_LE;
        audio_handle->audio_info[1].pcm_config.start_threshold= 1024 * 4;
        audio_handle->audio_info[1].pcm_config.stop_threshold= 1024 * 4 * 4;
        audio_handle->audio_info[1].pcm_config.silence_threshold= 1024 * 4;
    }
}

int audio_init(Audio_Handle_t *audio_handle, int mode)
{
    audio_set_config(audio_handle, mode);

    if(mode & AS_IN_MODE)
    {
        audio_handle->pcm[0] = pcm_open(audio_handle->card,
                             audio_handle->device,
                             audio_handle->audio_info[0].flags,
                             &audio_handle->audio_info[0].pcm_config);

        if(!audio_handle->pcm[0] || !pcm_is_ready(audio_handle->pcm[0]))
        {
            DEMO_ERR(audio_handle, "pcm_open: %s.\n", pcm_get_error(audio_handle->pcm[0]));
            return -1;
        }
    }

    if(mode & AS_OUT_MODE)
    {
        audio_handle->pcm[1] = pcm_open(audio_handle->card,
                             audio_handle->device,
                             audio_handle->audio_info[1].flags,
                             &audio_handle->audio_info[1].pcm_config);

        if(!audio_handle->pcm[1] || !pcm_is_ready(audio_handle->pcm[1]))
        {
            DEMO_ERR(audio_handle, "pcm_open: %s.\n", pcm_get_error(audio_handle->pcm[1]));
            return -1;
        }
    }

    if(mode & AC_MODE)
    {
        audio_handle->mixer = mixer_open(audio_handle->card);
        if(!audio_handle->mixer)
        {
            DEMO_ERR(audio_handle, "mixer_open: %s.\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}

int audio_deinit(Audio_Handle_t *audio_handle, int mode)
{
    if(mode & AS_IN_MODE && pcm_is_ready(audio_handle->pcm[0]))
        pcm_close(audio_handle->pcm[0]);

    if(mode & AS_OUT_MODE && pcm_is_ready(audio_handle->pcm[1]))
        pcm_close(audio_handle->pcm[1]);

    if(mode & AC_MODE)
        mixer_close(audio_handle->mixer);

    return 0;
}

int audio_get_buf(Audio_Handle_t *audio_handle, Audio_Buffer_t *audio_buf, int mode)
{
    int ret;

    if(mode == AS_IN_MODE)
    {
        audio_buf->buf = calloc(1, pcm_get_buffer_size(audio_handle->pcm[0]));
        if(!audio_buf->buf)
        {
            DEMO_ERR(audio_handle, "Error allocating a packet.\n");
            return -1;
        }

        ret = pcm_readi(audio_handle->pcm[0], audio_buf->buf,
                pcm_bytes_to_frames(audio_handle->pcm[0], pcm_get_buffer_size(audio_handle->pcm[0])));
        if (ret < 0)
        {
            DEMO_ERR(audio_handle, "pcm_readi: %s.\n", pcm_get_error(audio_handle->pcm[0]));
            return -1;
        }

        audio_buf->length = pcm_frames_to_bytes(audio_handle->pcm[0], ret);
    }
    else if(mode == AS_OUT_MODE)
    {
    	//printf("audio_get_buf here size: %d\n",pcm_get_buffer_size(audio_handle->pcm[1]));
        audio_buf->buf = calloc(1, pcm_get_buffer_size(audio_handle->pcm[1]));
		
        audio_buf->length = pcm_get_buffer_size(audio_handle->pcm[1]) ;
        if(!audio_buf->buf)
        {
            DEMO_ERR(audio_handle, "Error allocating a packet.\n");
            return -1;
        }
    }

    return 0;
}

int audio_put_buf(Audio_Handle_t *audio_handle, Audio_Buffer_t *audio_buf, int mode)
{
    int ret;

    if(mode == AS_IN_MODE)
    {
        audio_buf->length = 0;
        if(audio_buf->buf)
            free(audio_buf->buf);
    }
    else if(mode == AS_OUT_MODE)
    {
        //printf("AS_OUT_MODE audio_put_buf len:%d\n",audio_buf->length);
		/*
									{
									     int i = 0;
										 for (i=0; i< 20; i++)
										 	printf("0x%x ",((uint8_t *)(audio_buf->buf)));
										 printf("\n");
									}	
		*/
        ret = pcm_writei(audio_handle->pcm[1], audio_buf->buf,
               pcm_bytes_to_frames(audio_handle->pcm[1], audio_buf->length));
        if (ret < 0)
        {
			//printf("AS_OUT_MODE audio_put_buf error \n");
            DEMO_ERR(audio_handle, "pcm_writei: %s.\n", pcm_get_error(audio_handle->pcm[1]));
            return -1;
        }
        //printf("AS_OUT_MODE audio_put_buf end \n");

        audio_buf->length = 0;
        if(audio_buf->buf)
            free(audio_buf->buf);
    }

    return 0;
}

int audio_enum_standard_control(Audio_Handle_t *audio_handle)
{
    struct mixer_ctl *ctl;
    const char *name, *type;
    unsigned int num_ctls, num_values;
    int min, max, value, i;

    num_ctls = mixer_get_num_ctls(audio_handle->mixer);

    printf("====================Support These Controls====================\n");

    for (i = 0; i < num_ctls; i++)
    {
        ctl = mixer_get_ctl(audio_handle->mixer, i);

        name = mixer_ctl_get_name(ctl);
        type = mixer_ctl_get_type_string(ctl);
        num_values = mixer_ctl_get_num_values(ctl);
        value = mixer_ctl_get_value(ctl, 0);
        min = mixer_ctl_get_range_min(ctl);
        max = mixer_ctl_get_range_max(ctl);

        printf("Id: %d    Type: %s    Num: %d    Name: %s    Value: %d (range %d->%d)\n", i, type, num_values, name, value, min, max);
    }

    return 0;
}

int audio_send_standard_control(Audio_Handle_t *audio_handle, unsigned int id, int *value, int dir)
{
    int ret;
    struct mixer_ctl *ctl = mixer_get_ctl(audio_handle->mixer, id);

    switch(dir)
    {
        case CONTROL_GET:
            ret = mixer_ctl_get_value(ctl, 0);
            if(ret == -EINVAL)
            {
                DEMO_ERR(audio_handle, "mixer_ctl_get_value: %d.\n", ret);
                return ret;
            }
            *value = ret;
            break;

        case CONTROL_SET:
            ret = mixer_ctl_set_value(ctl, 0, *value);
            if(ret != 0)
            {
                DEMO_ERR(audio_handle, "mixer_ctl_set_value: %d.\n", ret);
                return ret;
            }
            break;

        default:
            DEMO_ERR(audio_handle, "Unknown direction.\n");
            return -1;
    }

    return 0;
}

int audio_dump_buf(Audio_Handle_t *audio_handle, Audio_Buffer_t *audio_buf, char *path, int mode)
{
    int fd;

    if(mode == AS_IN_MODE)
    {
        fd = open(path, O_APPEND | O_RDWR | O_CREAT, 0777);
        if(fd < 0)
        {
            DEMO_WRN(audio_handle, "open %s: %s.\n", path, strerror(errno));
            goto fail;
        }

        if(write(fd, audio_buf->buf, audio_buf->length) != audio_buf->length)
            DEMO_WRN(audio_handle, "write %s: %s.\n", path, strerror(errno));

        close(fd);
    }
    else if(mode == AS_OUT_MODE)
    {
        static int pos = 0;
        int ret;

        fd = open(path, O_RDWR, 0777);
        if(fd < 0)
        {
            DEMO_WRN(audio_handle, "open %s: %s.\n", path, strerror(errno));
            goto fail;
        }

        audio_buf->length = pcm_get_buffer_size(audio_handle->pcm[1]);

        ret = lseek(fd, pos * audio_buf->length, SEEK_SET);
        if(ret == -1)
            DEMO_WRN(audio_handle, "lseek %s: %s.\n", path, strerror(errno));

        ret = read(fd, audio_buf->buf, audio_buf->length);
        if(ret < 0)
            DEMO_WRN(audio_handle, "read %s: %s.\n", path, strerror(errno));
        else if(ret < audio_buf->length)
        {
            pos = 0;
            DEMO_INFO(audio_handle, "read %s: rewind.\n", path);
        }
        else
            pos++;

        close(fd);
    }


    return 0;

fail:
    return -1;
}

