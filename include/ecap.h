
/* PRU_ECAP register set */
typedef struct{

	/* PRU_ECAP_TSCTR register bit field */
	union {
		volatile uint32_t TSCTR;

		volatile struct {
			unsigned  TSCTR : 32;
		} TSCTR_bit;
	} ;	// 0x0


	/* PRU_ECAP_CTRPHS register bit field */
	union {
		volatile uint32_t CTRPHS;

		volatile struct{
			unsigned CTRPHS : 32;
		} CTRPHS_bit;
	} ;	// 0x4


	/* PRU_ECAP_CAP1 register bit field */
	union {
		volatile uint32_t CAP1;

		volatile struct{
			unsigned CAP1 : 32;	
		} CAP1_bit;
	} ;	// 0x8

        /* PRU_ECAP_CAP2 register bit field */
        union {
                volatile uint32_t CAP2;

                volatile struct{
                        unsigned CAP2 : 32;
                } CAP2_bit;
        } ;     // 0xC

        /* PRU_ECAP_CAP3 register bit field */
        union {
                volatile uint32_t CAP3;

                volatile struct{
                        unsigned CAP3 : 32;
                } CAP3_bit;
        } ;     // 0x10

        /* PRU_ECAP_CAP4 register bit field */
        union {
                volatile uint32_t CAP4;

                volatile struct{
                        unsigned CAP4 : 32;
                } CAP4_bit;
        } ;     // 0x14

        uint32_t rsvd18[4];     // 0x18 - 0x24

	/* PRU_ECAP_ECCTL1 register bit field */
	union {
		volatile uint16_t ECCTL1;

		volatile  struct{
			unsigned CAP1POL : 1;		// 0
			unsigned CTRRST1 : 1;		// 1
			unsigned CAP2POL : 1;		// 2
			unsigned CTRRST2 : 1;		// 3
			unsigned CAP3POL : 1;		// 4
			unsigned CTRRST3 : 1;		// 5
			unsigned CAP4POL : 1;		// 6
			unsigned CTRRST4 : 1;		// 7
			unsigned CAPLDEN : 1;		// 8
			unsigned PRESCALE : 5;		// 13:9
			unsigned FREE_SOFT : 2;		// 15:14
		} ECCTL1_bit;
	} ;	// 0x28


	/* PRU_ECCTL2 register bit field */
	union {
		volatile uint16_t ECCTL2;

		volatile  struct{
			unsigned CONT_ONESHT : 1;	// 0
			unsigned STOP_WRAP : 2;		// 2:1
			unsigned RE_ARM : 1;		// 3
			unsigned TSCTRSTOP : 1;		// 4
			unsigned SYNCI_EN : 1;		// 5
			unsigned SYNCO_SEL : 2;		// 7:6
			unsigned SWSYNC : 1;		// 8
			unsigned CAP_APWM : 1;		// 9
			unsigned APWMPOL : 1;		// 10
			unsigned Reserved : 5;		// 15:11
		} ECCTL2_bit;
	} ;	// 0x2A

	/* PRU_ECAP_ECEINT register bit field */
	union {
		volatile uint16_t ECEINT;

		volatile struct{
			unsigned Reserved : 1;		// 0
			unsigned CEVT1 : 1;		// 1
			unsigned CEVT2 : 1;		// 2
			unsigned CEVT3 : 1; 		// 3
			unsigned CEVT4 : 1;		// 4
			unsigned CNTOVF : 1;		// 5
			unsigned PRDEQ : 1;		// 6
			unsigned CMPEQ : 1;		// 7
			unsigned Reserved1 : 8;		// 15:8
		} ECEINT_bit;
	} ;	// 0x2C

        /* PRU_ECAP_ECFLG register bit field */
        union {
                volatile uint16_t ECFLG;

                volatile struct{
                        unsigned INT : 1;               // 0
                        unsigned CEVT1 : 1;             // 1
                        unsigned CEVT2 : 1;             // 2
                        unsigned CEVT3 : 1;             // 3
                        unsigned CEVT4 : 1;             // 4
                        unsigned CNTOVF : 1;            // 5
                        unsigned PRDEQ : 1;             // 6
                        unsigned CMPEQ : 1;             // 7
                        unsigned Reserved : 8;          // 15:8
                } ECFLG_bit;
        } ;     // 0x2E

	/* PRU_ECAP_ECCLR register bit field */
	union {
		volatile uint16_t ECCLR;

		volatile struct{
			unsigned INT : 1;		// 0
			unsigned CEVT1 : 1;		// 1
			unsigned CEVT2 : 1;		// 2
			unsigned CEVT3 : 1;		// 3
			unsigned CEVT4 : 1;		// 4
			unsigned CNTOVF : 1;		// 5
			unsigned PRDEQ : 1;		// 6
			unsigned CMPEQ : 1;		// 7
			unsigned Reserved : 8;		// 15:8
		} ECCLR_bit;
	} ;	// 0x30

        /* PRU_ECAP_ECFRC register bit field */
        union {
                volatile uint16_t ECFRC;

                volatile struct{
                        unsigned Reserved : 1;          // 0
                        unsigned CEVT1 : 1;             // 1
                        unsigned CEVT2 : 1;             // 2
                        unsigned CEVT3 : 1;             // 3
                        unsigned CEVT4 : 1;             // 4
                        unsigned CNTOVF : 1;            // 5
                        unsigned PRDEQ : 1;             // 6
                        unsigned CMPEQ : 1;             // 7
                        unsigned Reserved8 : 8;         // 15:8
                } ECFRC_bit;
        } ;     // 0x32


	uint32_t rsvd34[10];	// 0x34 - 0x54


	/* PRU_ECAP_REVID register bit field */
	union {
		volatile uint32_t REVID;

		volatile struct{
			unsigned REV : 32;		
		} REVID_bit;
	} ;	// 0x5C
} pruEcap;
