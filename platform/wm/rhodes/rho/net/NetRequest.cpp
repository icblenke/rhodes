#include "stdafx.h"

#include "NetRequest.h"
#include "common/AutoPointer.h"
#include "common/RhoFile.h"
#include "NetRequestImpl.h"
#include "common/RhodesApp.h"

namespace rho {
namespace net {
IMPLEMENT_LOGCLASS(CNetRequest,"Net");

INetResponse* CNetRequest::pullData(const String& strUrl, IRhoSession* oSession )
{
    return doRequest("GET",strUrl,String(),oSession,null);
}

INetResponse* CNetRequest::pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    return doRequest("POST",strUrl,strBody,oSession,null);
}

INetResponse* CNetRequest::pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
	INetResponse* pResp = doRequest("POST", strUrl, strBody, oSession, null );
	if ( pResp->isOK() )
        ((CNetResponseImpl*)pResp)->getRawData() = pResp->getCookies();
		
	return pResp;
}

INetResponse* CNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
    CNetRequestImpl oImpl(this, method,strUrl,oSession,pHeaders);
    return oImpl.sendString(strBody);
}


String CNetRequest::resolveUrl(const String& strUrl)
{
    return RHODESAPP().canonicalizeRhoUrl(strUrl);
}

void CNetRequest::cancel()
{
    m_bCancel = true;
    if ( m_pCurNetRequestImpl != null )
        m_pCurNetRequestImpl->cancel();
}

INetResponse* CNetRequest::pushFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
    {
        LOG(ERROR) + "pushFile: cannot find file :" + strFilePath;
        return new CNetResponseImpl();
    }

    int nTry = 0;
    m_bCancel = false;
    CNetResponseImpl* pResp = 0;
    do
    {
        if ( pResp )
            delete pResp;

        CNetRequestImpl oImpl(this, "POST",strUrl,oSession,pHeaders);
        pResp = oImpl.sendStream(oFile.getInputStream());
        nTry++;

    }while( !m_bCancel && !pResp->isResponseRecieved() && nTry < MAX_NETREQUEST_RETRY );

    return pResp;
}

INetResponse* CNetRequest::pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenForWrite) ) 
    {
        LOG(ERROR) + "pullFile: cannot create file :" + strFilePath;
        return new CNetResponseImpl();
    }

    int nTry = 0;
    m_bCancel = false;
    CNetResponseImpl* pResp = 0;
    do
    {
        if ( pResp )
            delete pResp;

        CNetRequestImpl oImpl(this, "GET",strUrl,oSession,pHeaders);
        pResp = oImpl.downloadFile(oFile);
        nTry++;

    }while( !m_bCancel && !pResp->isResponseRecieved() && nTry < MAX_NETREQUEST_RETRY );

    return pResp;
}

}
}
