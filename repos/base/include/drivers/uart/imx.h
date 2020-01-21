/*
 * \brief  Driver for Freescale's i.MX UART
 * \author Norman Feske
 * \author Martin Stein
 * \date   2012-08-30
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__UART__IMX_H_
#define _INCLUDE__DRIVERS__UART__IMX_H_

/* Genode includes */
#include <util/mmio.h>

namespace Genode { class Imx_uart; }


/**
 * Driver base for i.MX UART-module
 */
class Genode::Imx_uart: Mmio
{
	struct Pmr : Register<0x1f8, 32>
	{
		struct Mux_mode_0 : Bitfield<0, 1> { };
		struct Mux_mode_1 : Bitfield<1, 1> { };
		struct Mux_mode_2 : Bitfield<2, 1> { };

		static access_t init_value()
                {
                        return Mux_mode_0::bits(0) |
				Mux_mode_1::bits(0) |
				Mux_mode_2::bits(1);
		}
	};

	struct Pmr0 : Register<0x1fc, 32>
        {
                struct Mux_mode_0 : Bitfield<0, 1> { };
                struct Mux_mode_1 : Bitfield<1, 1> { };
                struct Mux_mode_2 : Bitfield<2, 1> { };

                static access_t init_value()
                {
                        return Mux_mode_0::bits(0) |
                                Mux_mode_1::bits(0) |
                                Mux_mode_2::bits(1);
                }
        };


	/**
	 * Control register 1
	 */
	struct Cr1 : Register<0x110080, 32>
	{
		struct Uart_en    : Bitfield<0, 1> { }; /* enable UART */
		struct Doze       : Bitfield<1, 1> { }; /* disable on doze */
		struct At_dma_en  : Bitfield<2, 1> { }; /* aging DMA
		                                         * timer on */
		struct Tx_dma_en  : Bitfield<3, 1> { }; /* TX ready DMA on */
		struct Snd_brk    : Bitfield<4, 1> { }; /* send breaks */
		struct Rtsd_en    : Bitfield<5, 1> { }; /* RTS delta IRQ on */
		struct Tx_mpty_en : Bitfield<6, 1> { }; /* TX empty IRQ on */
		struct Ir_en      : Bitfield<7, 1> { }; /* enable infrared */
		struct Rx_dma_en  : Bitfield<8, 1> { }; /* RX ready DMA on */
		struct R_rdy_en   : Bitfield<9, 1> { }; /* RX ready IRQ on */

		struct Icd        : Bitfield<10, 2> /* idle IRQ condition */
		{
			enum { IDLE_4_FRAMES = 0 };
		};

		struct Id_en    : Bitfield<12, 1> { }; /* enable idle IRQ */
		struct T_rdy_en : Bitfield<13, 1> { }; /* TX ready IRQ on */
		struct Adbr     : Bitfield<14, 1> { }; /* enable baud-rate
		                                        * auto detect */
		struct Ad_en    : Bitfield<15, 1> { }; /* enable ADBR IRQ */

		/**
		 * Initialization value
		 */
		static access_t init_value()
		{
			return Uart_en::bits(1) |
				   Doze::bits(0) |
				   At_dma_en::bits(0) |
				   Tx_dma_en::bits(0) |
				   Snd_brk::bits(0) |
				   Rtsd_en::bits(1) |
				   Tx_mpty_en::bits(0) |
				   Ir_en::bits(0) |
				   Rx_dma_en::bits(0) |
				   R_rdy_en::bits(1) |
				   Id_en::bits(0) |
				   T_rdy_en::bits(0) |
				   Adbr::bits(0) |
				   Ad_en::bits(0);
		}

	};

	/**
	 * Control register 2
	 */
	struct Cr2 : Register<0x110084, 32>
	{
		struct S_rst : Bitfield<0, 1> /* SW reset bit */
		{
			enum { NO_RESET = 1 };
		};

		struct Rx_en  : Bitfield<1, 1> { }; /* enable receiver */
		struct Tx_en  : Bitfield<2, 1> { }; /* enable transmitter */
		struct At_en  : Bitfield<3, 1> { }; /* enable aging timer */
		struct Rts_en : Bitfield<4, 1> { }; /* send request IRQ on */

		struct Ws : Bitfield<5, 1> /* select word size */
		{
			enum { _8_BITS = 1 };
		};

		struct Stpb : Bitfield<6, 1> /* number of stop bits */
		{
			enum { _1_BIT = 0 };
		};

		struct Pr_en  : Bitfield<8, 1> { };  /* enable parity */
		struct Rtec_any : Bitfield<9, 1> { }; /* Trigger IRQ on any edge */
		struct Esc_en : Bitfield<11, 1> { }; /* escape detection on */

		struct Ctsc   : Bitfield<13, 1>      /* select CTS control */
		{
			enum { BY_RECEIVER = 1 };
		};

		struct Irts : Bitfield<14, 1> { }; /* ignore RTS pin */
		struct Esci : Bitfield<15, 1> { }; /* enable escape IRQ */

		/**
		 * Initialization value
		 */
		static access_t init_value()
		{
			return S_rst::bits(S_rst::NO_RESET) |
				   Rx_en::bits(1) |
				   Tx_en::bits(1) |
				   At_en::bits(0) |
				   Rts_en::bits(0) |
				   Ws::bits(Ws::_8_BITS) |
				   Stpb::bits(Stpb::_1_BIT) |
				   Pr_en::bits(1) |
				   Rtec_any::bits(2) |
				   Esc_en::bits(1) |
				   Ctsc::bits(Ctsc::BY_RECEIVER) |
				   Irts::bits(1) |
				   Esci::bits(1);
		}
	};

	/**
	 * Control register 3
	 */
	struct Cr3 : Register<0x110088, 32>
	{
		struct Rxdmux_sel : Bitfield<2, 1> { }; /* use muxed RXD */
		struct Aci_en     : Bitfield<0, 1> { }; /* autobaud count IRQ on */
		struct Dtrd_en    : Bitfield<3, 1> { }; /* data terminal ready
		                                         * delta IRQ on */
		struct Awak_en    : Bitfield<4, 1> { }; /* wake IRQ on */
		struct Air_int_en : Bitfield<5, 1> { }; /* IR wake IRQ on */
		struct Rx_ds_en   : Bitfield<6, 1> { }; /* RX status IRQ on */
		struct Ad_nimp    : Bitfield<7, 1> { }; /* autobaud detect off */
		struct Ri_en      : Bitfield<8, 1> { }; /* ring indicator IRQ on */
		struct Dcd_en     : Bitfield<9, 1> { }; /* data carrier detect
		                                         * IRQ on */
		struct Dsr        : Bitfield<10,1> { }; /* DSR/DTR output */
		struct Frame_en   : Bitfield<11,1> { }; /* frame error IRQ on */
		struct Parity_en  : Bitfield<12,1> { }; /* parity error IRQ on */
		struct Dtr_en     : Bitfield<13,1> { }; /* data terminal ready
		                                         * IRQ on */
		struct Dpec_ctrl  : Bitfield<14,2> { }; /* DTR/DSR IRQ edge
		                                         * control */

		/**
		 * Initialization value
		 */
		static access_t init_value()
		{
			return Aci_en::bits(1) |
				   Rxdmux_sel::bits(1) |
				   Dtrd_en::bits(1) |
				   Awak_en::bits(0) |
				   Air_int_en::bits(0) |
				   Rx_ds_en::bits(0) |
				   Ad_nimp::bits(1) |
				   Ri_en::bits(0) |
				   Dcd_en::bits(0) |
				   Dsr::bits(0) |
				   Frame_en::bits(0) |
				   Parity_en::bits(0) |
				   Dtr_en::bits(0) |
				   Dpec_ctrl::bits(0);
		}
	};

	/**
	 * Control register 4
	 */
	struct Cr4 : Register<0x11008c, 32>
	{
		struct Dr_en       : Bitfield<0, 1> { }; /* RX data ready IRQ on */
		struct Or_en       : Bitfield<1, 1> { }; /* RX overrun IRQ on */
		struct Bk_en       : Bitfield<2, 1> { }; /* BREAK IRQ on */
		struct Tc_en       : Bitfield<3, 1> { }; /* TX complete IRQ on */
		struct Lp_dis      : Bitfield<4, 1> { }; /* low power off */
		struct IR_sc       : Bitfield<5, 1> { }; /* use UART ref clock
		                                          * for vote logic */
		struct Id_dma_en   : Bitfield<6, 1> { }; /* idle DMA IRQ on */
		struct Wake_en     : Bitfield<7, 1> { }; /* WAKE IRQ on */
		struct IR_en       : Bitfield<8, 1> { }; /* serial IR IRQ on */
		struct Cts_level   : Bitfield<10,6> { }; /* CTS trigger level*/

		/**
		 * Initialization value
		 */
		static access_t init_value()
		{
			return Dr_en::bits(1) |
			       Or_en::bits(1) |
			       Bk_en::bits(1) |
			       Tc_en::bits(1) |
			       Lp_dis::bits(1) |
			       IR_sc::bits(1) |
			       Id_dma_en::bits(1) |
			       Wake_en::bits(1) |
			       IR_en::bits(1) |
			       Cts_level::bits(0);
		}
	};

	/**
	 * Status register 1
	 */
	struct Sr1 : Register<0x110094, 32>
	{
		struct Wake : Bitfield<4, 1> { }; /* Asynchronous WAKE Interrupt Flag */
		struct Tr_ready : Bitfield<13, 1> { }; /* Transmitter Ready */
	};

	/**
	 * Status register 2
	 */
	struct Sr2 : Register<0x110098, 32>
	{
		struct Recv_ready : Bitfield<0, 1> { }; /* Receive Data Ready */
		struct Error : Bitfield<1, 1> { }; /* Overrun Error */
		struct Break : Bitfield<2, 1> { }; /* Break Condition Detected */
		struct Txdc : Bitfield<3, 1> { }; /* transmission complete */
		struct Empty : Bitfield<14, 1> { }; /* Transmit Buffer FIFO Empty */
	};

	/**
	 * Transmitter register
	 */
	struct Txd : Register<0x110040, 32>
	{
		struct Tx_data : Bitfield<0, 8> { }; /* transmit data */
	};

	struct Rxd : Register<0x110000, 32>
	{
		struct Rx_data : Bitfield<0, 8> { };
	};

	/**
	 * Transmit character 'c' without care about its type
	 */
	void _put_char(char c)
	{
		while (!read<Sr2::Txdc>()) { Genode::log("transmition not complete"); };
		write<Txd::Tx_data>(c);
		Genode::log("Done");
	}

	public:

		/**
		 * Constructor
		 *
		 * \param base  device MMIO base
		 */
		Imx_uart(addr_t base, uint32_t, uint32_t) : Mmio(base)
		{
			write<Pmr>(Pmr::init_value());
			write<Cr1>(Cr1::init_value());
			write<Cr2>(Cr2::init_value());
			write<Cr3>(Cr3::init_value());
			write<Cr4>(Cr4::init_value());
		}

		/**
		 * Print character 'c' through the UART
		 */
		void put_char(char c)
		{
			/* transmit character */
			_put_char(c);
		}

		char read_char()
		{
			while(!read<Sr2::Recv_ready>());
			return read<Rxd::Rx_data>();
		}

		bool char_ready()
		{
			return read<Sr2::Recv_ready>();
		}
};

#endif /* _INCLUDE__DRIVERS__UART__IMX_H_ */
