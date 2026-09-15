#pragma once

#ifdef ECL_EDITOR
	#define check_asset_return(Var) if(!Var.IsValid()) { return; }
	#define check_return(Var) if(!Var) { return; }
	#define check(Var) if(!Var) { DebugBreak(); }
#else
	#define check_asset_return(Var)
	#define check_return(Var)
	#define check(Var)
#endif