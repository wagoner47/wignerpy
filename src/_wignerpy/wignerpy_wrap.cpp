#include "include/wignerpy_wrap.h"

#define QUOTE(a) # a
#define uint unsigned int
#define CHK_NULL(a) \
    if (a == NULL) { \
        PyErr_Format(PyExc_MemoryError, "Failed to allocate %s", QUOTE(a)); \
        return NULL; }




/*_  __           _       _                       _   _               _
|  \/  | ___   __| |_   _| | ___   _ __ ___   ___| |_| |__   ___   __| |___
| |\/| |/ _ \ / _` | | | | |/ _ \ | '_ ` _ \ / _ \ __| '_ \ / _ \ / _` / __|
| |  | | (_) | (_| | |_| | |  __/ | | | | | |  __/ |_| | | | (_) | (_| \__ \
|_|  |_|\___/ \__,_|\__,_|_|\___| |_| |_| |_|\___|\__|_| |_|\___/ \__,_|___/ */

PyObject *wigner3j_wrap(PyObject *self, PyObject *args){
    double l1, l2, l3, m1, m2, m3;

    if (!PyArg_ParseTuple(args, "dddddd", &l1, &l2, &l3, &m1, &m2, &m3))
        return NULL;
    double result = WignerSymbols::wigner3j(l1, l2, l3, m1, m2, m3);
    //cout << a << endl; cout.flush();
    return Py_BuildValue("d", result);
}




// Module methods
static PyMethodDef omnical_methods[] = {
    {"wigner3j", (PyCFunction) wigner3j_wrap, METH_VARARGS,
        "Return the wigner 3j symbol of j1, j2, j3, m1, m2, m3."},

    //{"redcal", (PyCFunction)redcal_wrap, METH_VARARGS | METH_KEYWORDS,
        //"redcal(data,calpar,info,additivein,uselogcal=1,removedegen=0,maxiter=20,stepsize=.3,conv=.001)\nRun redundant calibration on data (3D array of complex floats)."},

    {NULL, NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC init_wignerpy(void) {
    PyObject* m;
    m = Py_InitModule3("_wignerpy", omnical_methods,
    "Wrapper for wignerSymbol C++ code (C++ authored by Joey Dumont).");

    //import_array();
}
