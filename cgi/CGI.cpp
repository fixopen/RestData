//
// Created by up duan on 30/4/15.
//

#include "CGI.h"
#include <cstdlib>

std::map<std::string, std::string> Request::GetCookie() {
    std::map<std::string, std::string> result;
    return result;
}

void Request::Response::SetCookie(std::map<std::string, std::string> const& cookie) {
    //
}

void CGI::GetEnv() {
    fServerSoftware 	= std::getenv("SERVER_SOFTWARE");
    fServerName 		= std::getenv("SERVER_NAME");
    fGatewayInterface 	= std::getenv("GATEWAY_INTERFACE");
    fServerProtocol 	= std::getenv("SERVER_PROTOCOL");

    std::string port 	= std::getenv("SERVER_PORT");
    fServerPort 		= (unsigned short)std::atol(port.c_str());

    fRequestMethod 	= std::getenv("REQUEST_METHOD");
    fPathInfo 		= std::getenv("PATH_INFO");
    fPathTranslated 	= std::getenv("PATH_TRANSLATED");
    fScriptName 		= std::getenv("SCRIPT_NAME");
    fQueryString 		= std::getenv("QUERY_STRING");
    fRemoteHost 		= std::getenv("REMOTE_HOST");
    fRemoteAddr 		= std::getenv("REMOTE_ADDR");
    fAuthType 		= std::getenv("AUTH_TYPE");
    fRemoteUser 		= std::getenv("REMOTE_USER");
    fRemoteIdent 		= std::getenv("REMOTE_IDENT");
    fContentType 		= std::getenv("CONTENT_TYPE");

    std::string length 	= std::getenv("CONTENT_LENGTH");
    fContentLength 	= (unsigned long)std::atol(length.c_str());

    fAccept 		= std::getenv("HTTP_ACCEPT");
    fUserAgent 		= std::getenv("HTTP_USER_AGENT");
    fRedirectRequest 	= std::getenv("REDIRECT_REQUEST");
    fRedirectURL 		= std::getenv("REDIRECT_URL");
    fRedirectStatus 	= std::getenv("REDIRECT_STATUS");
    fReferrer 		= std::getenv("HTTP_REFERER");
    fCookie 		= std::getenv("HTTP_COOKIE");
    fAcceptLanguageString = std::getenv("HTTP_ACCEPT_LANGUAGE");

    // Win32 bug fix by Peter Goedtkindt
    std::string https 	= std::getenv("HTTPS");
    if(https == "on")
        fUsingHTTPS = true;
    else
        fUsingHTTPS = false;
}