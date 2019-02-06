#pragma once

enum PhorgrError
{
	Success = 0,
	ErrorUnknown,
	NetworkSetup,
	NetworkReceive,
	NetworkSend,
	NetworkDNS
};