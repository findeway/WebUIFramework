#ifndef CefUtil_h__
#define CefUtil_h__

#include "cef_v8.h"
#include "cef_values.h"

bool ConvertArrayToList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
bool ConvertListToArray(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);

#endif // CefUtil_h__