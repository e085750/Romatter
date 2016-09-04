#include "twitCurlEx.h"
//#include <cstdint>
#include <stdint.h>

class Twit 
{
  public:
    Twit();
    ~Twit();
    //Twit& operator=(const Twit& rhs);
    //twitCurl twitterObj;
    twitCurlEx twitterObj;
    std::string tmpStr, tmpStr2, replyMsg;
    char tmpBuf[1024];
    std::string userName;
    std::string passWord;

  public:
    void post(const char *text);
    bool read_timeline(std::string& resp, int64_t last_id);
};
