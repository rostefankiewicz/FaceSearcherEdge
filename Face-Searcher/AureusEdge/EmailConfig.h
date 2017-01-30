// Leon: File created by me

#pragma once

#include "includes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct EmailConfig
{
	char m_sender[1024];
	char m_password[1024];
	char m_receiver[1024];
	char m_message[1024];
};

void SendEmail(EmailConfig *email);