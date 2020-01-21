/*
 * \brief  Test for UART driver
 * \author Christian Helmuth
 * \date   2011-05-30
 */

/*
 * Copyright (C) 2011-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/snprintf.h>
#include <base/component.h>
#include <timer_session/connection.h>
#include <uart_session/connection.h>

using namespace Genode;


struct Main
{
	Timer::Connection timer;
	Uart::Connection  uart;
	char              buf[100]={106,0000,0000};

	Main(Env &env) : timer(env), uart(env)
	{
		log("--- UART test started ---");
		while(1)
		{
			log("Send ",buf[0]);
			char _buf[100];
			uart.read(_buf,7);
			log("Recv ",_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
			//buf[0]='2';
			//buf[1]='1';
			uart.write(buf, 3);
			timer.msleep(2000);
		}
	}
};

void Component::construct(Env &env) { static Main main(env); }
