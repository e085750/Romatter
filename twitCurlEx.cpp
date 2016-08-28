#include "twitCurlEx.h"
#include "twitcurlurls.h"

bool twitCurlEx::timelinePublicGet(const std::string& par)
{
    /* Perform GET */
    return performGet( twitCurlDefaults::TWITCURL_PROTOCOLS[m_eProtocolType] +
                       twitterDefaults::TWITCURL_PUBLIC_TIMELINE_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] + par );
}
