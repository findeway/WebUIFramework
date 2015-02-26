#include "stdafx.h"
#include "CefUtil.h"

void SetListValue(CefRefPtr<CefV8Value> list, int index,
                  CefRefPtr<CefListValue> value)
{
    CefRefPtr<CefV8Value> new_value;

    CefValueType type = value->GetType(index);
    switch (type)
    {
        case VTYPE_LIST:
        {
            CefRefPtr<CefListValue> list = value->GetList(index);
            new_value = CefV8Value::CreateArray(static_cast<int>(list->GetSize()));
            ConvertListToArray(list, new_value);
        }
        break;
        case VTYPE_BOOL:
            new_value = CefV8Value::CreateBool(value->GetBool(index));
            break;
        case VTYPE_DOUBLE:
            new_value = CefV8Value::CreateDouble(value->GetDouble(index));
            break;
        case VTYPE_INT:
            new_value = CefV8Value::CreateInt(value->GetInt(index));
            break;
        case VTYPE_STRING:
            new_value = CefV8Value::CreateString(value->GetString(index));
            break;
        default:
            break;
    }

    if (new_value.get())
    {
        list->SetValue(index, new_value);
    }
    else
    {
        list->SetValue(index, CefV8Value::CreateNull());
    }
}

void SetListValue(CefRefPtr<CefListValue> list, int index, CefRefPtr<CefV8Value> value)
{
    if (value->IsArray())
    {
        CefRefPtr<CefListValue> new_list = CefListValue::Create();
        ConvertArrayToList(value, new_list);
        list->SetList(index, new_list);
    }
    else if (value->IsString())
    {
        list->SetString(index, value->GetStringValue());
    }
    else if (value->IsBool())
    {
        list->SetBool(index, value->GetBoolValue());
    }
    else if (value->IsInt())
    {
        list->SetInt(index, value->GetIntValue());
    }
    else if (value->IsDouble())
    {
        list->SetDouble(index, value->GetDoubleValue());
    }
}

bool ConvertArrayToList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target)
{
    if (source->IsArray())
    {
        int arg_length = source->GetArrayLength();
        if (arg_length != 0)
        {
            // Start with null types in all spaces.
            target->SetSize(arg_length);
            for (int i = 0; i < arg_length; ++i)
                SetListValue(target, i, source->GetValue(i));
            return true;
        }
    }
    return false;
}

bool ConvertListToArray(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target)
{
    if (target.get() && target->IsArray())
    {
        int arg_length = static_cast<int>(source->GetSize());
        if (arg_length != 0)
        {
            for (int i = 0; i < arg_length; ++i)
                SetListValue(target, i, source);
            return true;
        }
    }
    return false;
}
