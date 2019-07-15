/**
 * @file
 * H.265/HEVC codec.
 * @author Michael Niedermayer <michaelni@gmx.at>
 */

#ifndef AVCODEC_SSHEVCDEC_H
#define AVCODEC_SSHEVCDEC_H

#include <stdbool.h>

#include "libavutil/buffer.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/thread.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>


#include "avcodec.h"
#include "bswapdsp.h"
#include "cabac.h"
#include "get_bits.h"
#include "hevcpred.h"
#include "h2645_parse.h"
#include "hevc.h"
#include "hevc_ps.h"
#include "hevc_sei.h"
#include "hevcdsp.h"
#include "internal.h"
#include "thread.h"
#include "videodsp.h"


//sstar sdk lib
#include "mi_vdec.h"
#include "mi_vdec_datatype.h"

#include "mi_common.h"
#include "mi_common_datatype.h"

#include "mi_sys.h"
#include "mi_sys_datatype.h"

//#include "mi_divp.h"
//#include "mi_divp_datatype.h"

#include "mi_hdmi.h"
#include "mi_hdmi_datatype.h"

#include "mi_disp.h"
#include "mi_disp_datatype.h"



typedef struct SsHevcContext {
    AVFrame *frame;
	
	AVCodecContext *avctx;
	
	BswapDSPContext bdsp;
	
	H2645Packet pkt;
	
	HEVCSEI sei;

    HEVCParamSets ps;
	
    int channel;
	int max_ra;
	
    int cropwidth;
    int cropheight;
  
    const uint8_t *data;

	/**
     * Sequence counters for decoded and output frames, so that old
     * frames are output first after a POC reset
     */
    uint16_t seq_decode;
    uint16_t seq_output;
   
    // type of the first VCL NAL of the current frame
    enum HEVCNALUnitType first_nal_type;
	enum HEVCNALUnitType nal_unit_type;

	struct SwsContext *img_ctx;

    uint8_t context_initialized;
	uint8_t eos, last_eos;
    int is_nalff;           ///< this flag is != 0 if bitstream is encapsulated
                            ///< as a format defined in 14496-15
    int apply_defdispwin;

    int nal_length_size;    ///< Number of bytes used for nal length (1, 2 or 4)
	int nuh_layer_id;
} SsHevcContext;




#endif

