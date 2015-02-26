#ifndef BrowserV8Handler_h__
#define BrowserV8Handler_h__

#include "cef_v8.h"

class CBrowserV8Handler :
	public CefV8Handler
{
public:
	CBrowserV8Handler(void);
	~CBrowserV8Handler(void);

	virtual bool Execute( const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception ) OVERRIDE;

	//自定义接口
	bool FindMessageCallback(const std::wstring& message_name,int browser_id,std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> >& targetCallback);
	bool AddMessageCallback(const std::wstring& message_name,int browser_id,CefRefPtr<CefV8Context> context,CefRefPtr<CefV8Value> function);
	bool RemoveMessageCallback(const std::wstring& message_name,int browser_id);
	bool RemoveMessageCallback(CefRefPtr<CefV8Context> context);

private:
	IMPLEMENT_REFCOUNTING(CBrowserV8Handler);

private:
	// 消息回调队列
	typedef std::map<std::pair<std::wstring, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > > CallbackMap;
	CallbackMap								m_mapMessageCallback;
};

#endif // BrowserV8Handler_h__
