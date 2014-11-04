/*
 * Copyright (c) 2014, Texas Instruments, Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE. */

#include <stdint.h>
#include <cfg.h>

volatile register uint32_t __R30;
volatile register uint32_t __R31;

/* 1D Transfer Parameters */
typedef struct{
	uint32_t src;
	uint32_t dst;
	uint32_t chan;
} hostBuffer;

/* EDMA PARAM registers */
typedef struct{
	uint32_t sam		: 1;
	uint32_t dam		: 1;
	uint32_t syncdim	: 1;
	uint32_t static_set	: 1;
	uint32_t		: 4;
	uint32_t fwid		: 3;
	uint32_t tccmode	: 1;
	uint32_t tcc		: 6;
	uint32_t		: 2;
	uint32_t tcinten	: 1;
	uint32_t itcinten	: 1;
	uint32_t tcchen		: 1;
	uint32_t itcchen	: 1;
	uint32_t privid		: 4;
	uint32_t		: 3;
	uint32_t priv		: 1;
} edmaParamOpt;

/*typedef struct{
	uint32_t src;
} edmaParamSrc;*/

typedef struct{
	uint32_t acnt		: 16;
	uint32_t bcnt		: 16;
} edmaParamABcnt;

/*typedef struct{
	uint32_t dst;
} edmaParamDst;*/

typedef struct{
	uint32_t srcbidx	: 16;
	uint32_t dstbidx	: 16;
} edmaParamBidx;

typedef struct{
	uint32_t link		: 16;
	uint32_t bcntrld	: 16;
} edmaParamLnkRld;

typedef struct{
	uint32_t srccidx	: 16;
	uint32_t dstcidx	: 16;
} edmaParamCidx;

typedef struct{
	uint32_t ccnt		: 16;
	uint32_t		: 16;
} edmaParamCcnt;

typedef struct{
	edmaParamOpt	opt;
	/*edmaParamSrc*/ uint32_t	src;
	edmaParamABcnt	abcnt;
	/*edmaParamDst*/ uint32_t	dst;
	edmaParamBidx	bidx;
	edmaParamLnkRld	lnkrld;
	edmaParamCidx	cidx;
	edmaParamCcnt	ccnt;
} edmaParam;

/* Mapping Constant table register to variable */
volatile pruCfg CT_CFG __attribute__((cregister("CFG", near), peripheral));

/* This is a compiler hack so that the PRU knows where the parameters are stored
 * This is effectively a pointer to DRAM
 */
#pragma LOCATION(buf, 0)
hostBuffer buf;

/* PRU-to-ARM interrupt */
#define PRU0_ARM_INTERRUPT ((19-16)+32)

/* Addresses for Constant Table pointer registers
 * CTBIR_0 -> C24 (PRU0 DRAM)
 * CTBIR_1 -> C25 (PRU1 DRAM)
 */
#define CTBIR_0         (*(volatile uint32_t *)(0x22020))
#define CTBIR_1         (*(volatile uint32_t *)(0x22024))

/* EDMA Channel Registers */
#define EDMA0_CC_BASE	((volatile uint32_t *)(0x49000000))
#define DMAQNUM0	0x0240
#define QUEPRI		0x0284
#define EMR		0x0300
#define EMCR		0x0308
#define EMCRH		0x030C
#define QEMCR		0x0314
#define CCERRCLR	0x031C
#define DRAE0		0x0340
#define DRAE1		0x0348
#define DRAE2		0x0350
#define DRAE3		0x0358
#define QWMTHRA		0x0620
#define GLOBAL_ESR	0x1010
#define GLOBAL_ESRH	0x1014
#define GLOBAL_EECR	0x1028
#define GLOBAL_EECRH	0x102C
#define GLOBAL_SECR	0x1040
#define GLOBAL_SECRH	0x1044
#define GLOBAL_IESR	0x1060
#define GLOBAL_IESRH	0x1064
#define GLOBAL_ICR	0x1070
#define GLOBAL_ICRH	0x1074

/* EDMA Shadow Region 1 */
#define ESR		0x2210
#define ESRH		0x2214
#define EECR		0x2228
#define EECRH		0x222C
#define SECR		0x2240
#define SECRH		0x2244
#define IPR		0x2268
#define IPRH		0x226C
#define ICR		0x2270
#define ICRH		0x2274
#define IESR		0x2260
#define IESRH		0x2264
#define IEVAL		0x2278
#define IECR		0x2258
#define IECRH		0x225C

/* EDMA PARAM registers */
#define PARAM_OFFSET	0x4000
#define OPT		0x00
#define SRC		0x04
#define A_B_CNT		0x08
#define DST		0x0C
#define SRC_DST_BIDX	0x10
#define LINK_BCNTRLD	0x14
#define SRC_DST_CIDX	0x18
#define CCNT		0x1C


#define COPY_LENGTH	32

void main(){
	hostBuffer hostData;
	uint32_t channelMask;
	uint16_t paramOffset;
	edmaParam params;
	volatile uint32_t *ptr;
	volatile edmaParam *pParams;

	ptr = EDMA0_CC_BASE;

	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

	/* Load channel parameters from DRAM - loaded by host */
	hostData.src = buf.src;
	hostData.dst = buf.dst;
	hostData.chan = buf.chan;

	/* Setup EDMA region access for Shadow Region 1 */
	channelMask = (1 << hostData.chan);
	ptr[DRAE1/4] |= channelMask;

	/* Clear channel event from EDMA event registers */
	ptr[GLOBAL_ESR/4] |= channelMask;
	ptr[GLOBAL_SECR/4] |= channelMask;
	ptr[GLOBAL_ICR/4] |= channelMask;

	/* Enable channel interrupt */
	ptr[GLOBAL_IESR/4] |= channelMask;

	/* Clear event missed register */
	ptr[EMCR/4] |= channelMask;

	/* Setup channel to submit to EDMA TC0 */
	ptr[DMAQNUM0/4] &= 0xFF00FFFF;

	/* Setup and store PaRAM set for transfer */
	paramOffset = PARAM_OFFSET;
	paramOffset += hostData.chan << 5; // channel * 0x20

	params.lnkrld.link = 0xFFFF;
	params.lnkrld.bcntrld = 0x0000;
	params.opt.tcc = hostData.chan;
	params.opt.tcinten = 1;
	params.opt.itcchen = 1;

	params.ccnt.ccnt = 1;
	params.abcnt.acnt = 0x100;
	params.abcnt.bcnt = 0x1;
	params.bidx.srcbidx = 0x1;
	params.bidx.dstbidx = 0x1;
	params.src = hostData.src;
	params.dst = hostData.dst;

	pParams = (volatile edmaParam *)ptr + paramOffset;
	*pParams = params;

	/* Trigger transfer */
	/*ptr[IEVAL/4] = channelMask;*/ // TH: I think this is useless  - IEVAL only uses bit 0
	ptr[ESRH/4] = channelMask;

	/* Wait for transfer completion */
	while (ptr[IPR/4] != hostData.chan){
	}

	/* Halt PRU core */
	__halt();
}

