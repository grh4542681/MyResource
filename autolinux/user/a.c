#include<Python.h>
#include<stdio.h>
#include"a.h"

void PrintHello()
{
	printf("hello world!\n");
}

static PyObject* _p_PrintHello(PyObject *self)
{
    PrintHello();
    Py_RETURN_NONE;
}

static PyMethodDef ExtestMethods[] = {
    {"PrintHello", (PyCFunction)_p_PrintHello, METH_NOARGS,NULL}, 
    {NULL, NULL,0,NULL},
};

void initliba() {
    Py_InitModule("liba", ExtestMethods);
}
