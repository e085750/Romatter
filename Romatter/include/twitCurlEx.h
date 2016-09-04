#include "twitcurl.h"

class twitCurlEx : public twitCurl
{
  public:
    bool timelinePublicGet(const std::string& par);
};
