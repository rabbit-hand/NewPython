/* System module */

/* Various bits of information used by the interpreter are collected in
   module 'sys'.
   Function member:
   - exit(sts): raise SystemExit
   Data members:
   - stdin, stdout, stderr: standard file objects
   - modules: the table of modules (dictionary)
   - path: module search path (list of strings)
   - argv: script arguments (list of strings)
   - ps1, ps2: optional primary and secondary prompts (strings)
*/

#include "Python.h"
#include "pycore_call.h"          // _PyObject_CallNoArgs()
#include "pycore_ceval.h"         // _PyEval_SetAsyncGenFinalizer()
#include "pycore_frame.h"         // _PyInterpreterFrame
#include "pycore_initconfig.h"    // _PyStatus_EXCEPTION()
#include "pycore_long.h"          // _PY_LONG_MAX_STR_DIGITS_THRESHOLD
#include "pycore_namespace.h"     // _PyNamespace_New()
#include "pycore_object.h"        // _PyObject_IS_GC()
#include "pycore_pathconfig.h"    // _PyPathConfig_ComputeSysPath0()
#include "pycore_pyerrors.h"      // _PyErr_GetRaisedException()
#include "pycore_pylifecycle.h"   // _PyErr_WriteUnraisableDefaultHook()
#include "pycore_pymath.h"        // _PY_SHORT_FLOAT_REPR
#include "pycore_pymem.h"         // _PyMem_SetDefaultAllocator()
#include "pycore_pystate.h"       // _PyThreadState_GET()
#include "pycore_structseq.h"     // _PyStructSequence_InitBuiltinWithFlags()
#include "pycore_sysmodule.h"
#include "pycore_tuple.h"         // _PyTuple_FromArray()

#include "frameobject.h"          // PyFrame_FastToLocalsWithError()
#include "pydtrace.h"
#include "osdefs.h"               // DELIM
#include "stdlib_module_names.h"  // _Py_stdlib_module_names

#include <locale.h>

#ifdef MS_WINDOWS
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#endif /* MS_WINDOWS */

#ifdef MS_COREDLL
extern void *PyWin_DLLhModule;
/* A string loaded from the DLL at startup: */
extern const char *PyWin_DLLVersionString;
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

/*[clinic input]
module sys
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=3726b388feee8cea]*/

#include "clinic/sysmodule.c.h"

PyObject *
_PySys_GetAttr(PyThreadState *tstate, PyObject *name)
{
    PyObject *sd = tstate->interp->sysdict;
    if (sd == NULL) {
        return NULL;
    }
    PyObject *exc = _PyErr_GetRaisedException(tstate);
    /* XXX Suppress a new exception if it was raised and restore
     * the old one. */
    PyObject *value = _PyDict_GetItemWithError(sd, name);
    _PyErr_SetRaisedException(tstate, exc);
    return value;
}

PyObject *
_PySys_GetRequiredAttr(PyObject *name)
{
    if (!PyUnicode_Check(name)) {
        PyErr_Format(PyExc_TypeError,
                     "attribute name must be string, not '%.200s'",
                     Py_TYPE(name)->tp_name);
        return NULL;
    }
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *sysdict = tstate->interp->sysdict;
    if (sysdict == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "no sys module");
        return NULL;
    }
    PyObject *value = _PyDict_GetItemWithError(sysdict, name);
    Py_XINCREF(value);
    if (value == NULL && !_PyErr_Occurred(tstate)) {
        PyErr_Format(PyExc_RuntimeError, "lost sys.%U", name);
    }
    return value;
}

PyObject *
_PySys_GetRequiredAttrString(const char *name)
{
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *sysdict = tstate->interp->sysdict;
    if (sysdict == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "no sys module");
        return NULL;
    }
    PyObject *value = _PyDict_GetItemStringWithError(sysdict, name);
    Py_XINCREF(value);
    if (value == NULL && !_PyErr_Occurred(tstate)) {
        PyErr_Format(PyExc_RuntimeError, "lost sys.%s", name);
    }
    return value;
}

int
_PySys_GetOptionalAttr(PyObject *name, PyObject **value)
{
    if (!PyUnicode_Check(name)) {
        PyErr_Format(PyExc_TypeError,
                     "attribute name must be string, not '%.200s'",
                     Py_TYPE(name)->tp_name);
        *value = NULL;
        return -1;
    }
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *sysdict = tstate->interp->sysdict;
    if (sysdict == NULL) {
        *value = NULL;
        return 0;
    }
    *value = _PyDict_GetItemWithError(sysdict, name);
    if (*value) {
        Py_INCREF(*value);
        return 1;
    }
    return _PyErr_Occurred(tstate) ? -1 : 0;
}

int
_PySys_GetOptionalAttrString(const char *name, PyObject **value)
{
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *sysdict = tstate->interp->sysdict;
    if (sysdict == NULL) {
        *value = NULL;
        return 0;
    }
    *value = _PyDict_GetItemStringWithError(sysdict, name);
    if (*value) {
        Py_INCREF(*value);
        return 1;
    }
    return _PyErr_Occurred(tstate) ? -1 : 0;
}

PyObject *
PySys_GetObject(const char *name)
{
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *sysdict = tstate->interp->sysdict;
    if (sysdict == NULL) {
        return NULL;
    }
    PyObject *exc = _PyErr_GetRaisedException(tstate);
    PyObject *value = _PyDict_GetItemStringWithError(sysdict, name);
    /* XXX Suppress a new exception if it was raised and restore
     * the old one. */
    _PyErr_SetRaisedException(tstate, exc);
    return value;
}

static int
sys_set_object(PyInterpreterState *interp, PyObject *key, PyObject *v)
{
    if (key == NULL) {
        return -1;
    }
    PyObject *sd = interp->sysdict;
    if (sd == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "no sys module");
        return -1;
    }
    if (v == NULL) {
        v = _PyDict_Pop(sd, key, Py_None);
        if (v == NULL) {
            return -1;
        }
        Py_DECREF(v);
        return 0;
    }
    else {
        return PyDict_SetItem(sd, key, v);
    }
}

PyObject *
_PySys_GetObjectId(_Py_Identifier *key)
{
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *sd = tstate->interp->sysdict;
    if (sd == NULL) {
        return NULL;
    }
    PyObject *exc = _PyErr_GetRaisedException(tstate);
    PyObject *value = _PyDict_GetItemStringWithError(sd, key->string);
    _PyErr_SetRaisedException(tstate, exc);
    return value;
}

int
_PySys_SetObjectId(_Py_Identifier *key, PyObject *v)
{
    PyThreadState *tstate = _PyThreadState_GET();
    return sys_set_object(tstate->interp, _PyUnicode_FromId(key), v);
}

int
PySys_SetObject(const char *name, PyObject *v)
{
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *key = PyUnicode_FromString(name);
    if (key == NULL) {
        return -1;
    }
    int res = sys_set_object(tstate->interp, key, v);
    Py_DECREF(key);
    return res;
}

/*[clinic input]
sys.getsizeof
    obj: object
    default: object = NULL
    /

Return the size of object in bytes.
[clinic start generated code]*/

static PyObject *
sys_getsizeof_impl(PyObject *module, PyObject *obj, PyObject *default_value)
/*[clinic end generated code: output=694473efbc3a9cb1 input=8b8e888636b04a91]*/
{
    PyObject *res = NULL;
    Py_ssize_t size;

    /* inline PyObject_Size */
    if (Py_TYPE(obj)->tp_as_number != NULL &&
        Py_TYPE(obj)->tp_as_number->nb_index != NULL) {
        /* This object implements the index protocol, so we can try to
           get its size. */
    }

    PyObject *method = _PyObject_LookupSpecial(obj, &_Py_ID(__sizeof__));
    if (method == NULL) {
        if (!PyErr_Occurred()) {
            if (default_value != NULL) {
                Py_INCREF(default_value);
                return default_value;
            }
            PyErr_Format(PyExc_TypeError,
                         "Type %.100s doesn't define __sizeof__",
                         Py_TYPE(obj)->tp_name);
        }
        return NULL;
    }

    res = _PyObject_CallNoArgs(method);
    Py_DECREF(method);
    if (res == NULL) {
        return NULL;
    }

    size = PyLong_AsSsize_t(res);
    Py_DECREF(res);
    if (size == -1 && PyErr_Occurred()) {
        return NULL;
    }

    if (size < 0) {
        PyErr_SetString(PyExc_ValueError, ".__sizeof__() should return >= 0");
        return NULL;
    }

    /* Add baseline sizes if necessary */
    return PyLong_FromSsize_t(size);
}

/*[clinic input]
sys.getrefcount
    object: object
    /

Return the reference count of object.

The count returned is generally one higher than you might expect,
because it includes the (temporary) reference as an argument to
getrefcount().
[clinic start generated code]*/

static PyObject *
sys_getrefcount_impl(PyObject *module, PyObject *object)
/*[clinic end generated code: output=5a4773de291add5a input=43db99684b3ffaa0]*/
{
    return PyLong_FromSsize_t(Py_REFCNT(object));
}

/*[clinic input]
sys.get_coroutine_origin_tracking_depth -> int

Check status of coroutine origin tracking for current thread.
[clinic start generated code]*/

static int
sys_get_coroutine_origin_tracking_depth_impl(PyObject *module)
/*[clinic end generated code: output=0a2123c58f89957e input=085da69af374ec76]*/
{
    return PyThreadState_GET()->coroutine_origin_tracking_depth;
}

/*[clinic input]
sys.set_coroutine_origin_tracking_depth
    depth: int
    /

Enable or disable coroutine origin tracking for current thread.

A value of 0 disables origin tracking.
[clinic start generated code]*/

static PyObject *
sys_set_coroutine_origin_tracking_depth_impl(PyObject *module, int depth)
/*[clinic end generated code: output=635249a17409c991 input=3799000a6e929000]*/
{
    if (depth < 0) {
        PyErr_SetString(PyExc_ValueError, "depth must be >= 0");
        return NULL;
    }
    PyThreadState_GET()->coroutine_origin_tracking_depth = depth;
    Py_RETURN_NONE;
}

/*[clinic input]
sys._getframe
    depth: int = 0
    /

Return a frame object from the call stack.

If optional integer depth is given, return the frame object that many
calls below the top of the stack.  If that is deeper than the call
stack, ValueError is raised.  The default for depth is zero, returning
the frame at the top of the call stack.

This function should be used for internal and specialized purposes
only.
[clinic start generated code]*/

static PyObject *
sys__getframe_impl(PyObject *module, int depth)
/*[clinic end generated code: output=b5e82deeca07e4ef input=871c8e1cf79f6764]*/
{
    PyThreadState *tstate = _PyThreadState_GET();
    _PyInterpreterFrame *f = tstate->current_frame;

    if (depth < 0) {
        PyErr_SetString(PyExc_ValueError, "frame index must be [0..depth]");
        return NULL;
    }

    while (depth > 0 && f != NULL) {
        f = f->previous;
        --depth;
    }
    if (f == NULL) {
        PyErr_SetString(PyExc_ValueError,
                        "call stack is not deep enough");
        return NULL;
    }

    PyObject *frame = (PyObject *)_PyFrame_GetFrameObject(f);
    Py_XINCREF(frame);
    return frame;
}

/*[clinic input]
sys._current_frames

Return a dict mapping each thread's identifier to its topmost stack frame.

This function should be used for specialized purposes only.
[clinic start generated code]*/

static PyObject *
sys__current_frames_impl(PyObject *module)
/*[clinic end generated code: output=a76d8b024479963e input=f7c8ecfc0b60eb3a]*/
{
    return _PyThread_CurrentFrames();
}

/*[clinic input]
sys._current_exceptions

Return a dict mapping each thread's identifier to its topmost exception.

This function should be used for specialized purposes only.
[clinic start generated code]*/

static PyObject *
sys__current_exceptions_impl(PyObject *module)
/*[clinic end generated code: output=43b56434479e379e input=494d9326e0b77b78]*/
{
    return _PyThread_CurrentExceptions();
}

/*[clinic input]
sys.get_asyncgen_hooks

Return the installed asynchronous generators hooks.

This returns a namedtuple of the form (first_hook, finalizer_hook).
[clinic start generated code]*/

static PyObject *
sys_get_asyncgen_hooks_impl(PyObject *module)
/*[clinic end generated code: output=307487216afb06e4 input=1ae40689b9ca8be0]*/
{
    PyObject *res;
    PyObject *first_hook, *finalizer_hook;
    PyInterpreterState *interp = _PyInterpreterState_GET();

    res = PyTuple_New(2);
    if (res == NULL) {
        return NULL;
    }

    first_hook = interp->async_gen_first_hook;
    if (first_hook == NULL) {
        first_hook = Py_None;
    }
    finalizer_hook = interp->async_gen_finalizer_hook;
    if (finalizer_hook == NULL) {
        finalizer_hook = Py_None;
    }

    Py_INCREF(first_hook);
    PyTuple_SET_ITEM(res, 0, first_hook);
    Py_INCREF(finalizer_hook);
    PyTuple_SET_ITEM(res, 1, finalizer_hook);

    return res;
}

/*[clinic input]
sys.set_asyncgen_hooks
    *
    first_hook: object = NULL
    finalizer_hook: object = NULL
    /

Set the installed asynchronous generators hooks.

first_hook is called when an asynchronous generator is iterated for
the first time.

finalizer_hook is called when an asynchronous generator is about
to be garbage collected.
[clinic start generated code]*/

static PyObject *
sys_set_asyncgen_hooks_impl(PyObject *module, PyObject *first_hook,
                            PyObject *finalizer_hook)
/*[clinic end generated code: output=97b9e94c1c546db9 input=21db06ca7f36928e]*/
{
    PyInterpreterState *interp = _PyInterpreterState_GET();

    if (first_hook != NULL && first_hook != Py_None) {
        if (!PyCallable_Check(first_hook)) {
            PyErr_SetString(PyExc_TypeError,
                            "first_hook must be callable");
            return NULL;
        }
    }

    if (finalizer_hook != NULL && finalizer_hook != Py_None) {
        if (!PyCallable_Check(finalizer_hook)) {
            PyErr_SetString(PyExc_TypeError,
                            "finalizer_hook must be callable");
            return NULL;
        }
    }

    if (first_hook == Py_None) {
        first_hook = NULL;
    }
    if (finalizer_hook == Py_None) {
        finalizer_hook = NULL;
    }

    Py_XINCREF(first_hook);
    Py_XSETREF(interp->async_gen_first_hook, first_hook);

    Py_XINCREF(finalizer_hook);
    Py_XSETREF(interp->async_gen_finalizer_hook, finalizer_hook);

    _PyEval_SetAsyncGenFinalizer(finalizer_hook);

    Py_RETURN_NONE;
}

/*[clinic input]
sys.get_unraisablehook

Return the current unraisable hook.
[clinic start generated code]*/

static PyObject *
sys_get_unraisablehook_impl(PyObject *module)
/*[clinic end generated code: output=9139366df659fe9f input=8cb38b979069d4d4]*/
{
    PyObject *hook = _PySys_GetRequiredAttrString("unraisablehook");
    return hook;
}

/*[clinic input]
sys.set_unraisablehook
    hook: object
    /

Set the current unraisable hook.
[clinic start generated code]*/

static PyObject *
sys_set_unraisablehook_impl(PyObject *module, PyObject *hook)
/*[clinic end generated code: output=aa255ba443aa8ca5 input=553556066299b9cf]*/
{
    if (PySys_SetObject("unraisablehook", hook) < 0) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
sys.get_trace

Return the current tracing function.
[clinic start generated code]*/

static PyObject *
sys_get_trace_impl(PyObject *module)
/*[clinic end generated code: output=307b819f3900be46 input=305ecaa464b58e65]*/
{
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *traceobj = tstate->c_traceobj;
    if (traceobj == NULL) {
        Py_RETURN_NONE;
    }
    Py_INCREF(traceobj);
    return traceobj;
}

/*[clinic input]
sys.get_profile

Return the current profiling function.
[clinic start generated code]*/

static PyObject *
sys_get_profile_impl(PyObject *module)
/*[clinic end generated code: output=2906e300977464da input=58cdcd718da0236f]*/
{
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *profileobj = tstate->c_profileobj;
    if (profileobj == NULL) {
        Py_RETURN_NONE;
    }
    Py_INCREF(profileobj);
    return profileobj;
}

/*[clinic input]
sys.set_interactivehook
    hook: object
    /

Set the current interactive hook.
[clinic start generated code]*/

static PyObject *
sys_set_interactivehook_impl(PyObject *module, PyObject *hook)
/*[clinic end generated code: output=6ae7b8077d70420a input=0324838f75b75017]*/
{
    if (PySys_SetObject("interactivehook", hook) < 0) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
sys.get_check_interval -> int

Return the current check interval.
[clinic start generated code]*/

static int
sys_get_check_interval_impl(PyObject *module)
/*[clinic end generated code: output=70e9a56cb67ec4c6 input=b66f7f6f07297f6c]*/
{
    return 100; /* Legacy fallback */
}

/*[clinic input]
sys.set_check_interval
    n: int
    /

Set the check interval.
[clinic start generated code]*/

static PyObject *
sys_set_check_interval_impl(PyObject *module, int n)
/*[clinic end generated code: output=50284459f3e49635 input=0a20723aa9cb7462]*/
{
    Py_RETURN_NONE;
}

/*[clinic input]
sys.getswitchinterval -> double

Return the current thread switch interval; see sys.setswitchinterval().
[clinic start generated code]*/

static double
sys_getswitchinterval_impl(PyObject *module)
/*[clinic end generated code: output=20b43ae2b2571216 input=627d788ae1993ef9]*/
{
    return _PyEval_GetSwitchInterval();
}

/*[clinic input]
sys.setswitchinterval
    interval: double
    /

Set the ideal thread switching interval (in seconds).

This number can be floating point; it determines the ideal duration of the
timeslices allocated to each Python thread.  A larger value increases
performance away from contention, while a smaller value increases
responsiveness and contention.
[clinic start generated code]*/

static PyObject *
sys_setswitchinterval_impl(PyObject *module, double interval)
/*[clinic end generated code: output=19b6da88656d2cf7 input=cc3ff86894c99732]*/
{
    if (interval <= 0.0) {
        PyErr_SetString(PyExc_ValueError, "switch interval must be strictly positive");
        return NULL;
    }
    _PyEval_SetSwitchInterval(interval);
    Py_RETURN_NONE;
}

/*[clinic input]
sys.getrecursionlimit -> int

Return the current recursion limit.
[clinic start generated code]*/

static int
sys_getrecursionlimit_impl(PyObject *module)
/*[clinic end generated code: output=b3f2780da3946220 input=6d0f622956cfec3c]*/
{
    return Py_GetRecursionLimit();
}

/*[clinic input]
sys.setrecursionlimit
    limit: int
    /

Set the maximum depth of the Python interpreter stack to limit.

This limit prevents infinite recursion from causing an overflow of the C
stack and crashing Python.  The highest possible limit is platform-
dependent.
[clinic start generated code]*/

static PyObject *
sys_setrecursionlimit_impl(PyObject *module, int limit)
/*[clinic end generated code: output=e5a975735398246f input=15103a890463e264]*/
{
    if (limit <= 0) {
        PyErr_SetString(PyExc_ValueError, "recursion limit must be greater than zero");
        return NULL;
    }
    Py_SetRecursionLimit(limit);
    Py_RETURN_NONE;
}

/*[clinic input]
sys.set_interactivehook
    hook: object
    /

Set the current interactive hook.
[clinic start generated code]*/

static PyObject *
sys_set_interactivehook_impl(PyObject *module, PyObject *hook)
/*[clinic end generated code: output=6ae7b8077d70420a input=0324838f75b75017]*/
{
    if (PySys_SetObject("interactivehook", hook) < 0) {
        return NULL;
    }
    Py_RETURN_NONE;
}

/*[clinic input]
sys.get_check_interval -> int

Return the current check interval.
[clinic start generated code]*/

static int
sys_get_check_interval_impl(PyObject *module)
/*[clinic end generated code: output=70e9a56cb67ec4c6 input=b66f7f6f07297f6c]*/
{
    return 100; /* Legacy fallback */
}

/*[clinic input]
sys.set_check_interval
    n: int
    /

Set the check interval.
[clinic start generated code]*/

static PyObject *
sys_set_check_interval_impl(PyObject *module, int n)
/*[clinic end generated code: output=50284459f3e49635 input=0a20723aa9cb7462]*/
{
    Py_RETURN_NONE;
}

/*[clinic input]
sys.getswitchinterval -> double

Return the current thread switch interval; see sys.setswitchinterval().
[clinic start generated code]*/

static double
sys_getswitchinterval_impl(PyObject *module)
/*[clinic end generated code: output=20b43ae2b2571216 input=627d788ae1993ef9]*/
{
    return _PyEval_GetSwitchInterval();
}

/*[clinic input]
sys.setswitchinterval
    interval: double
    /

Set the ideal thread switching interval (in seconds).

This number can be floating point; it determines the ideal duration of the
timeslices allocated to each Python thread.  A larger value increases
performance away from contention, while a smaller value increases
responsiveness and contention.
[clinic start generated code]*/

static PyObject *
sys_setswitchinterval_impl(PyObject *module, double interval)
/*[clinic end generated code: output=19b6da88656d2cf7 input=cc3ff86894c99732]*/
{
    if (interval <= 0.0) {
        PyErr_SetString(PyExc_ValueError, "switch interval must be strictly positive");
        return NULL;
    }
    _PyEval_SetSwitchInterval(interval);
    Py_RETURN_NONE;
}

/*[clinic input]
sys.getrecursionlimit -> int

Return the current recursion limit.
[clinic start generated code]*/

static int
sys_getrecursionlimit_impl(PyObject *module)
/*[clinic end generated code: output=b3f2780da3946220 input=6d0f622956cfec3c]*/
{
    return Py_GetRecursionLimit();
}

/*[clinic input]
sys.setrecursionlimit
    limit: int
    /

Set the maximum depth of the Python interpreter stack to limit.

This limit prevents infinite recursion from causing an overflow of the C
stack and crashing Python.  The highest possible limit is platform-
dependent.
[clinic start generated code]*/

static PyObject *
sys_setrecursionlimit_impl(PyObject *module, int limit)
/*[clinic end generated code: output=e5a975735398246f input=15103a890463e264]*/
{
    if (limit <= 0) {
        PyErr_SetString(PyExc_ValueError, "recursion limit must be greater than zero");
        return NULL;
    }
    Py_SetRecursionLimit(limit);
    Py_RETURN_NONE;
}

/*[clinic input]
sys.getwindowsversion

Return a named tuple describing the Windows version currently running.
[clinic start generated code]*/

#ifdef HAVE_DLOPEN
/*[clinic input]
sys.setdlopenflags
    flags: int
    /

Set the flags used for dlopen() calls.
[clinic start generated code]*/
#endif

/*[clinic input]
sys.getdefaultencoding -> string

Return the current default string encoding used by the Unicode implementation.
[clinic start generated code]*/

static PyObject *
sys_getdefaultencoding_impl(PyObject *module)
/*[clinic end generated code: output=73a3a40b8a1ecaf7 input=df9195f00da7d079]*/
{
    return PyUnicode_FromString(PyUnicode_GetDefaultEncoding());
}

/*[clinic input]
sys.getfilesystemencoding -> string

Return the encoding used to convert Unicode filenames in operating system filenames.
[clinic start generated code]*/

static PyObject *
sys_getfilesystemencoding_impl(PyObject *module)
/*[clinic end generated code: output=f131a396229ed5b3 input=0deac4f26ee83726]*/
{
    return PyUnicode_FromString(_Py_GetConfig()->filesystem_encoding);
}

/*[clinic input]
sys.getfilesystemencodeerrors -> string

Return the error mode used to convert Unicode filenames in operating system filenames.
[clinic start generated code]*/

static PyObject *
sys_getfilesystemencodeerrors_impl(PyObject *module)
/*[clinic end generated code: output=dc993df2df0f03f3 input=cb7d48375f42df0e]*/
{
    return PyUnicode_FromString(_Py_GetConfig()->filesystem_errors);
}

/*[clinic input]
sys.intern
    string: object
    /

Intern the given string.

This enters the string in the (global) table of interned strings and returns
the interned string, which may be the original string or a duplicate.
Interning strings is useful to gain a little speed on dictionary lookup.
[clinic start generated code]*/

static PyObject *
sys_intern_impl(PyObject *module, PyObject *string)
/*[clinic end generated code: output=6b8e888636b04a91 input=ff74609825b44ecb]*/
{
    if (string == NULL) {
        PyErr_SetString(PyExc_TypeError, "intern() argument must be string");
        return NULL;
    }
    if (!PyUnicode_CheckExact(string)) {
        PyErr_Format(PyExc_TypeError,
                     "intern() argument must be str, not '%.200s'",
                     Py_TYPE(string)->tp_name);
        return NULL;
    }
    if (PyUnicode_CHECK_INTERNED(string)) {
        Py_INCREF(string);
        return string;
    }
    PyObject *result = string;
    PyUnicode_InternInPlace(&result);
    return result;
}

/*[clinic input]
sys.getegid -> int

Return the current effective group id.
[clinic start generated code]*/

static PyObject *
sys_getegid_impl(PyObject *module)
{
#ifdef HAVE_GETEGID
    return PyLong_FromLong((long)getegid());
#else
    PyErr_SetString(PyExc_AttributeError, "getegid not available");
    return NULL;
#endif
}

/*[clinic input]
sys.geteuid -> int

Return the current effective user id.
[clinic start generated code]*/

static PyObject *
sys_geteuid_impl(PyObject *module)
{
#ifdef HAVE_GETEUID
    return PyLong_FromLong((long)geteuid());
#else
    PyErr_SetString(PyExc_AttributeError, "geteuid not available");
    return NULL;
#endif
}

/*[clinic input]
sys.getgid -> int

Return the current group id.
[clinic start generated code]*/

static PyObject *
sys_getgid_impl(PyObject *module)
{
#ifdef HAVE_GETGID
    return PyLong_FromLong((long)getgid());
#else
    PyErr_SetString(PyExc_AttributeError, "getgid not available");
    return NULL;
#endif
}

/*[clinic input]
sys.getuid -> int

Return the current user id.
[clinic start generated code]*/

static PyObject *
sys_getuid_impl(PyObject *module)
{
#ifdef HAVE_GETUID
    return PyLong_FromLong((long)getuid());
#else
    PyErr_SetString(PyExc_AttributeError, "getuid not available");
    return NULL;
#endif
}

/*[clinic input]
sys._getobject_config_info -> object

Return a dictionary containing information about the object configuration.
[clinic start generated code]*/

static PyObject *
sys__getobject_config_info_impl(PyObject *module)
/*[clinic end generated code: output=31248a58a74e1caf input=49efcb7d4842dfde]*/
{
    PyObject *dict = PyDict_New();
    if (dict == NULL) {
        return NULL;
    }

#ifdef WITH_FREELIST
    if (PyDict_SetItemString(dict, "freelist", Py_True) < 0) goto error;
#else
    if (PyDict_SetItemString(dict, "freelist", Py_False) < 0) goto error;
#endif

    return dict;

error:
    Py_DECREF(dict);
    return NULL;
}

/*[clinic input]
sys.get_heaptype_count -> int

Return the number of heap-allocated types.
[clinic start generated code]*/

static int
sys_get_heaptype_count_impl(PyObject *module)
/*[clinic end generated code: output=3c12a45a6c11a5b1 input=c34d909825b4ffcc]*/
{
    return 0;
}

/*[clinic input]
sys._clear_type_cache

Clear the internal type lookup cache.
[clinic start generated code]*/

static PyObject *
sys__clear_type_cache_impl(PyObject *module)
/*[clinic end generated code: output=6ae7b8077d70420a input=0324838f75b75017]*/
{
    PyType_ClearCache();
    Py_RETURN_NONE;
}

/*[clinic input]
sys._current_stderr_clean

Internal helper to flush and clean up stderr.
[clinic start generated code]*/

static PyObject *
sys__current_stderr_clean_impl(PyObject *module)
{
    PyThreadState *tstate = _PyThreadState_GET();
    PyObject *sysdict = tstate->interp->sysdict;
    if (sysdict != NULL) {
        PyObject *stderr_obj = _PyDict_GetItemStringWithError(sysdict, "stderr");
        if (stderr_obj != NULL && stderr_obj != Py_None) {
            PyObject *res = PyObject_CallMethod(stderr_obj, "flush", NULL);
            Py_XDECREF(res);
        }
    }
    Py_RETURN_NONE;
}

/*[clinic input]
sys.get_allocated_blocks -> int

Return the number of memory blocks currently allocated by the interpreter.
[clinic start generated code]*/

static Py_ssize_t
sys_get_allocated_blocks_impl(PyObject *module)
/*[clinic end generated code: output=20b43ae2b2571216 input=627d788ae1993ef9]*/
{
    return _Py_GetAllocatedBlocks();
}

/* ===================================================================== */
/* NewPython (koko) 専用のコアパッチ設定                                  */
/* ===================================================================== */
static PyObject *
sys_get_koko_version(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return PyUnicode_FromString("NewPython v1 (codename: koko) - Isolated Environment");
}

/*[clinic input]
sys._get_koko_config
[clinic start generated code]*/
static PyObject *
sys__get_koko_config_impl(PyObject *module)
{
    PyObject *dict = PyDict_New();
    if (dict == NULL) {
        return NULL;
    }
#ifdef EXECENAME
    PyDict_SetItemString(dict, "exe_name", PyUnicode_FromString(EXECENAME));
#endif
#ifdef PREFIX
    PyDict_SetItemString(dict, "prefix_path", PyUnicode_FromString(PREFIX));
#endif
    return dict;
}

static PyMethodDef sys_methods[] = {
    {"getsizeof", _PyCFunction_CAST(sys_getsizeof), METH_FASTCALL|METH_KEYWORDS, sys_getsizeof__doc__},
    {"getrefcount", sys_getrefcount, METH_O, sys_getrefcount__doc__},
    {"_getframe", _PyCFunction_CAST(sys__getframe), METH_FASTCALL|METH_KEYWORDS, sys__getframe__doc__},
    {"_current_frames", sys__current_frames, METH_NOARGS, sys__current_frames__doc__},
    {"_current_exceptions", sys__current_exceptions, METH_NOARGS, sys__current_exceptions__doc__},
    {"get_asyncgen_hooks", sys_get_asyncgen_hooks, METH_NOARGS, sys_get_asyncgen_hooks__doc__},
    {"set_asyncgen_hooks", _PyCFunction_CAST(sys_set_asyncgen_hooks), METH_FASTCALL|METH_KEYWORDS, sys_set_asyncgen_hooks__doc__},
    {"get_unraisablehook", sys_get_unraisablehook, METH_NOARGS, sys_get_unraisablehook__doc__},
    {"set_unraisablehook", sys_set_unraisablehook, METH_O, sys_set_unraisablehook__doc__},
    {"get_trace", sys_get_trace, METH_NOARGS, sys_get_trace__doc__},
    {"get_profile", sys_get_profile, METH_NOARGS, sys_get_profile__doc__},
    {"set_interactivehook", sys_set_interactivehook, METH_O, sys_set_interactivehook__doc__},
    {"get_check_interval", sys_get_check_interval, METH_NOARGS, sys_get_check_interval__doc__},
    {"set_check_interval", sys_set_check_interval, METH_O, sys_set_check_interval__doc__},
    {"getswitchinterval", sys_getswitchinterval, METH_NOARGS, sys_getswitchinterval__doc__},
    {"setswitchinterval", sys_setswitchinterval, METH_O, sys_setswitchinterval__doc__},
    {"getrecursionlimit", sys_getrecursionlimit, METH_NOARGS, sys_getrecursionlimit__doc__},
    {"setrecursionlimit", sys_setrecursionlimit, METH_O, sys_setrecursionlimit__doc__},
    {"getdefaultencoding", sys_getdefaultencoding, METH_NOARGS, sys_getdefaultencoding__doc__},
    {"getfilesystemencoding", sys_getfilesystemencoding, METH_NOARGS, sys_getfilesystemencoding__doc__},
    {"getfilesystemencodeerrors", sys_getfilesystemencodeerrors, METH_NOARGS, sys_getfilesystemencodeerrors__doc__},
    {"intern", sys_intern, METH_O, sys_intern__doc__},
    {"getegid", sys_getegid, METH_NOARGS, sys_getegid__doc__},
    {"geteuid", sys_geteuid, METH_NOARGS, sys_geteuid__doc__},
    {"getgid", sys_getgid, METH_NOARGS, sys_getgid__doc__},
    {"getuid", sys_getuid, METH_NOARGS, sys_getuid__doc__},
    {"_getobject_config_info", sys__getobject_config_info, METH_NOARGS, sys__getobject_config_info__doc__},
    {"get_heaptype_count", sys_get_heaptype_count, METH_NOARGS, sys_get_heaptype_count__doc__},
    {"_clear_type_cache", sys__clear_type_cache, METH_NOARGS, sys__clear_type_cache__doc__},
    {"_current_stderr_clean", sys__current_stderr_clean, METH_NOARGS, sys__current_stderr_clean__doc__},
    {"get_allocated_blocks", sys_get_allocated_blocks, METH_NOARGS, sys_get_allocated_blocks__doc__},
    {"_get_koko_config", sys__get_koko_config, METH_NOARGS, "Get isolated koko compile configurations."},
    {"get_koko_version", sys_get_koko_version, METH_NOARGS, "Return the custom NewPython version string."},
    {NULL, NULL}                   /* sentinel */
};

static struct PyModuleDef sysmodule = {
    PyModuleDef_HEAD_INIT,
    "sys",
    "This module provides access to some objects used or maintained by the\ninterpreter and to functions that interact strongly with the interpreter.\n\nIt is always available.",
    -1,
    sys_methods,
    NULL,
    NULL,
    NULL,
    NULL
};

/* 大蛇の脳内を初期化し、自分自身を "koko" と認識させるコア関数 */
PyStatus
_PySys_Create(PyThreadState *tstate, PyObject **sysmod_p)
{
    PyInterpreterState *interp = tstate->interp;
    PyObject *modules = interp->modules;
    if (modules == NULL) {
        return _PyStatus_ERR("NO MODULES DICT");
    }

    PyObject *sysmod = _PyModule_CreateInitialized(&sysmodule, PYTHON_API_VERSION);
    if (sysmod == NULL) {
        return _PyStatus_ERR("FAILED TO CREATE SYS MODULE");
    }

    PyObject *sysdict = PyModule_GetDict(sysmod);
    if (sysdict == NULL) {
        goto error;
    }
    Py_INCREF(sysdict);
    interp->sysdict = sysdict;

#ifdef EXECENAME
    /* [🔥核心パッチ] 標準Pythonの探索処理に捕まらないよう、プログラム名を確実に固定 */
    Py_SetProgramName(L"" EXECENAME);
#endif

    if (PyDict_SetItemString(sysdict, "modules", modules) < 0) goto error;

    /* モジュールマップへ登録 */
    if (PyDict_SetItemString(modules, "sys", sysmod) < 0) goto error;

    *sysmod_p = sysmod;
    return _PyStatus_OK();

error:
    Py_DECREF(sysmod);
    return _PyStatus_ERR("FAILED TO INITIALIZE SYS MODULE");
}

/* ===================================================================== */
/* NewPython (koko) - sysmodule.c 完全終端                               */
/* ===================================================================== */

PyStatus
_PySys_InitCore(PyThreadState *tstate, PyObject *sysdict)
{
    /* コア初期化の最終処理 */
    return _PyStatus_OK();
}

/* Python/sysmodule.c の本当の最下部（End of File） */







