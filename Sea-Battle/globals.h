#pragma once

#if defined(_DEBUG) || defined(DEVELOPER)
#define DEVELOPER_CODE(x) (x)
#else
#define DEVELOPER_CODE(x)
#endif

namespace globals
{
	
}