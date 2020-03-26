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
PyObject *wigner3j_test(PyObject *self, PyObject *args){
    //cout << a << endl; cout.flush();
    return Py_BuildValue("d", WignerSymbols::wigner3j( 827,  1020,  1570,   590,   902, -1492));
}
PyObject *wigner3j_wrap(PyObject *self, PyObject *args){
    double l1, l2, l3, m1, m2, m3;

    if (!PyArg_ParseTuple(args, "dddddd", &l1, &l2, &l3, &m1, &m2, &m3))
        return NULL;
        
    double result = 0;
    //if (l3 >= l2 and l3 >= l1){
        //result = WignerSymbols::wigner3j(l3, l1, l2, m3, m1, m2);
    //}else if(l2 >= l3 and l2 >= l1){
        //result = WignerSymbols::wigner3j(l2, l3, l1, m2, m3, m1);
    //}else{
        result = WignerSymbols::wigner3j(l1, l2, l3, m1, m2, m3);
    //}
    //if (result == 0){
        //PyErr_Format(PyExc_ValueError, "Either the combination of input quantum numbers (%d, %d, %d, %d, %d, %d) are not physical, or you have hit the region of large quantum numbers where this code shamefully cannot handle.", (int)l1, (int)l2, (int)l3, (int)m1, (int)m2, (int)m3);
        //return NULL;
    //}
    //cout << a << endl; cout.flush();
    return Py_BuildValue("d", result);
}



PyObject *wigner3j_vec_wrap(PyObject *self, PyObject *args){
    PyArrayObject *op = NULL; // output array
    PyObject *rv;
    double l1, l2, m1, m2, m3;

    //parse inputs
    if (!PyArg_ParseTuple(args, "dddd", &l1, &l2, &m1, &m2))
        return NULL;
    m3 = - m1 - m2;

    //C++ calculation
    std::vector<double> result = WignerSymbols::wigner3j(l1, l2, m3, m1, m2);
    if (result[0] == 0){
        PyErr_Format(PyExc_ValueError, "Either the combination of input quantum numbers (%d, %d, %d, %d) are not physical, or you have hit the region of large quantum numbers where this code shamefully cannot handle.", (int)l1, (int)l2, (int)m1, (int)m2);
        return NULL;
    }
    // allocate output op array
    npy_intp dims[1] = {result.size()};
    op = (PyArrayObject *) PyArray_SimpleNew(1, dims, PyArray_DOUBLE);

    CHK_NULL(op);


    //copy to op array
    for (uint n = 0; n < result.size(); n++) {
        ((double *) PyArray_GETPTR1(op,n))[0] = result[n];
    }


    // Make sure to DECREF when using Py_BuildValue() with objects!!
    rv = Py_BuildValue("O", PyArray_Return(op));
    Py_DECREF(op);
    return rv;
}

PyObject *wigner6j_wrap(PyObject *self, PyObject *args){//NOTE: do not take the l and m synbols here literally! They are just wrappers. For actual physical meaning for these 6j symbols, ask Joey Dumont, who's the author of the C++ codes.
    double l1, l2, l3, m1, m2, m3;

    if (!PyArg_ParseTuple(args, "dddddd", &l1, &l2, &l3, &m1, &m2, &m3))
        return NULL;
    double result = WignerSymbols::wigner6j(l1, l2, l3, m1, m2, m3);
    //cout << a << endl; cout.flush();
    return Py_BuildValue("d", result);
}



PyObject *wigner6j_vec_wrap(PyObject *self, PyObject *args){//NOTE: do not take the l and m synbols here literally! They are just wrappers. For actual physical meaning for these 6j symbols, ask Joey Dumont, who's the author of the C++ codes.
    PyArrayObject *op = NULL; // output array
    PyObject *rv;
    double l1, l2, m1, m2, m3;

    //parse inputs
    if (!PyArg_ParseTuple(args, "ddddd", &l1, &l2, &m1, &m2, &m3))
        return NULL;

    //C++ calculation
    std::vector<double> result = WignerSymbols::wigner6j(l1, l2, m1, m2, m3);


    // allocate output op array
    npy_intp dims[1] = {result.size()};
    op = (PyArrayObject *) PyArray_SimpleNew(1, dims, PyArray_DOUBLE);

    CHK_NULL(op);


    //copy to op array
    for (uint n = 0; n < result.size(); n++) {
        ((double *) PyArray_GETPTR1(op,n))[0] = result[n];
    }


    // Make sure to DECREF when using Py_BuildValue() with objects!!
    rv = Py_BuildValue("O", PyArray_Return(op));
    Py_DECREF(op);
    return rv;
}

// Module methods
static PyMethodDef wignerpy_methods[] = {
    {"test", (PyCFunction) wigner3j_test, METH_NOARGS,
        "Return the wigner 3j symbol of j1, j2, j3, m1, m2, m3."},
    {"wigner3j", (PyCFunction) wigner3j_wrap, METH_VARARGS,
        "Return the wigner 3j symbol of j1, j2, j3, m1, m2, m3."},
    {"wigner3jvec", (PyCFunction) wigner3j_vec_wrap, METH_VARARGS,
        "Return the wigner 3j symbol of all non-zero j3 in a numpy array given j1, j2, m1, m2."},
    {"wigner6j", (PyCFunction) wigner6j_wrap, METH_VARARGS,
        "Return the wigner 6j symbol of j1, j2, j3, j4, j5, j6."},
    {"wigner6jvec", (PyCFunction) wigner6j_vec_wrap, METH_VARARGS,
        "Return the wigner 6j symbol of all non-zero j1 in a numpy array given j2, j3, j4, j5, j6."},
    //{"redcal", (PyCFunction)redcal_wrap, METH_VARARGS | METH_KEYWORDS,
        //"redcal(data,calpar,info,additivein,uselogcal=1,removedegen=0,maxiter=20,stepsize=.3,conv=.001)\nRun redundant calibration on data (3D array of complex floats)."},

    {NULL, NULL}  /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3
    #define MOD_ERROR_VAL NULL
    #define MOD_SUCCESS_VAL(val) val
    #define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
    #define MOD_DEF(ob, name, doc, methods) \
        static struct PyModuleDef moduledef = { \
            PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
        ob = PyModule_Create(&moduledef);
#else
    #define MOD_ERROR_VAL
    #define MOD_SUCCESS_VAL(val)
    #define MOD_INIT(name) void init##name(void)
    #define MOD_DEF(ob, name, doc, methods) \
        ob = Py_InitModule3(name, methods, doc);
#endif

MOD_INIT(_wignerpy) {
    PyObject* m;
    MOD_DEF(m, "_wignerpy", "Wrapper for wignerSymbol C++ code (C++ authored by Joey Dumont).", wignerpy_methods);
    if (m == NULL) {
        return MOD_ERROR_VAL;
    }
    import_array();
    return MOD_SUCCESS_VAL(m);
}
