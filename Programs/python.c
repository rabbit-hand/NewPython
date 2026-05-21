/* =============================================================================
 * NewPython Core Main Entry Point (Zero-Friction Edition)
 * Stripped away all obsolete legacy initializations and rigid bloat.
 * ============================================================================= */

#include "Python.h"

#ifdef MS_WINDOWS
int wmain(int argc, wchar_t **argv)
{
    /* Frictionless: Direct Windows execution without legacy overhead */
    return Py_Main(argc, argv);
}
#else
int main(int argc, char **argv)
{
    /* Frictionless: Minimalist UNIX boot flow, pure performance */
    return Py_BytesMain(argc, argv);
}
#endif
