#include "StdAfx.h"
#include "BrowserV8Handler.h"

CBrowserV8Handler::CBrowserV8Handler(void)
{
}

CBrowserV8Handler::~CBrowserV8Handler(void)
{
}

bool CBrowserV8Handler::Execute( const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception )
{
	bool handled = false;
	if (name == _T("DoWebUICall"))
	{
		// Send a message to the browser process.
		if (arguments.size() > 0 && arguments[0]->IsString())
		{
			CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
			if(browser.get())
			{
				CefString name = arguments[0]->GetStringValue();
				if (!name.empty())
				{
					CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name);
					if(arguments.size() > 1 && arguments[1]->IsString())
					{
						message->GetArgumentList()->SetString(0,arguments[1]->GetStringValue());
					}
					browser->SendProcessMessage(PID_BROWSER, message);
					handled = true;
				}
			}
		}
	}
	else if (name == _T("setWebUICallback"))
	{
		// Set a message callback.
		if (arguments.size() == 2 && arguments[0]->IsString() && arguments[1]->IsFunction())
		{
			std::wstring name = arguments[0]->GetStringValue();
			CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
			int browser_id = context->GetBrowser()->GetIdentifier();
			AddMessageCallback(name, browser_id, context, arguments[1]);
			handled = true;
		}
	}
	else if (name == _T("removeWebUICallback"))
	{
		// Remove a message callback.
		if (arguments.size() == 1 && arguments[0]->IsString())
		{
			std::wstring name = arguments[0]->GetStringValue();
			CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
			int browser_id = context->GetBrowser()->GetIdentifier();
			bool removed = RemoveMessageCallback(name, browser_id);
			retval = CefV8Value::CreateBool(removed);
			handled = true;
		}
	}
	return handled;
}

bool CBrowserV8Handler::AddMessageCallback( const std::wstring& message_name,int browser_id,CefRefPtr<CefV8Context> context,CefRefPtr<CefV8Value> function )
{
	if(CefCurrentlyOn(TID_RENDERER))
	{
		m_mapMessageCallback.insert(std::make_pair(std::make_pair(message_name, browser_id), std::make_pair(context, function)));
		return true;
	}
	return false;
}

bool CBrowserV8Handler::RemoveMessageCallback( const std::wstring& message_name,int browser_id )
{
	if(CefCurrentlyOn(TID_RENDERER))
	{
		CallbackMap::iterator it = m_mapMessageCallback.find(std::make_pair(message_name, browser_id));
		if (it != m_mapMessageCallback.end()) {
			m_mapMessageCallback.erase(it);
			return true;
		}
	}
	return false;
}

bool CBrowserV8Handler::RemoveMessageCallback( CefRefPtr<CefV8Context> context )
{
	for(CallbackMap::iterator it = m_mapMessageCallback.begin(); it != m_mapMessageCallback.end(); )
	{
		if(it->second.first->IsSame(context))
		{
			m_mapMessageCallback.erase(it++);
			return true;
		}
		else
		{
			it++;
		}
	}
	return false;
}

bool CBrowserV8Handler::FindMessageCallback( const std::wstring& message_name,int browser_id,std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> >& targetCallback )
{
	if(!m_mapMessageCallback.empty())		
	{
		CallbackMap::iterator targetIter = m_mapMessageCallback.find(std::make_pair(message_name,browser_id));
		if(targetIter != m_mapMessageCallback.end())
		{
			targetCallback = targetIter->second;
			return true;
		}
	}
	return false;
}
