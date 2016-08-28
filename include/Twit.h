#include "twitCurlEx.h"
//#include <cstdint>
#include <stdint.h>

class Twit 
{
  public:
    Twit();
    ~Twit();
    //twitCurl twitterObj;
    twitCurlEx twitterObj;
    std::string tmpStr, tmpStr2, replyMsg;
    char tmpBuf[1024];
    std::string userName;
    std::string passWord;

  public:
    void post(const char *text);
    void read_timeline();
    int64_t last_id;
};
