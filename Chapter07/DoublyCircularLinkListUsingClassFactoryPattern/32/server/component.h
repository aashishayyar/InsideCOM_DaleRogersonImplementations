static HMODULE g_hModule = NULL;

static long g_cComponents  = 0;
static long g_cServerLocks = 0;

const char g_szFriendlyName[] = "Aashish N. Ayyar : DCLLusingClassFactory";
const char g_szVerIndProgId[] = "AashishAyyar.InsideCOM07";

const char g_szProgId[]       = "AashishAyyar.InsideCOM07.1";

#define SUCCESS 0
#define FAILURE 1