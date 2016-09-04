#include "Twit.h"
#include "oauth_info.h"
#include <stdint.h>
#include <exception>

//Twit& Twit::operator=(const Twit& t)
//{
//  this->userName = t.userName;
//  this->passWord = t.passWord;
//  this->twitterObj = t.twitterObj;
//  return *this;
//}

Twit::Twit()
{
    std::string replyMsg = "";
    userName = "";
    passWord = "";
    /* Set twitter username and password */
    twitterObj.setTwitterUsername( userName );
    twitterObj.setTwitterPassword( passWord );

    /* OAuth flow begins */
    /* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
    twitterObj.getOAuth().setConsumerKey( std::string(CONSUMER) );
    twitterObj.getOAuth().setConsumerSecret( std::string(CONSUMERSECRET) );

    /* Step 1: Check if we alredy have OAuth access token from a previous run */
    std::string myOAuthAccessTokenKey("");
    std::string myOAuthAccessTokenSecret("");
    std::ifstream oAuthTokenKeyIn;
    std::ifstream oAuthTokenSecretIn;

    oAuthTokenKeyIn.open( "twitterClient_token_key.txt" );
    oAuthTokenSecretIn.open( "twitterClient_token_secret.txt" );

    memset( tmpBuf, 0, 1024 );
    oAuthTokenKeyIn >> tmpBuf;
    myOAuthAccessTokenKey = tmpBuf;

    memset( tmpBuf, 0, 1024 );
    oAuthTokenSecretIn >> tmpBuf;
    myOAuthAccessTokenSecret = tmpBuf;

    oAuthTokenKeyIn.close();
    oAuthTokenSecretIn.close();

    if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
    {
        /* If we already have these keys, then no need to go through auth again */
        //printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

        twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
    }
    else
    {
        /* Step 2: Get request token key and secret */
        std::string authUrl;
        twitterObj.oAuthRequestToken( authUrl );

        /* Step 3: Get PIN  */
        memset( tmpBuf, 0, 1024 );
        //printf( "\nDo you want to visit twitter.com for PIN (0 for no; 1 for yes): " );
        //gets( tmpBuf );
        tmpStr = tmpBuf;
        if( std::string::npos != tmpStr.find( "1" ) )
        {
            /* Ask user to visit twitter.com auth page and get PIN */
            memset( tmpBuf, 0, 1024 );
            //printf( "\nPlease visit this link in web browser and authorize this application:\n%s", authUrl.c_str() );
            //printf( "\nEnter the PIN provided by twitter: " );
            //gets( tmpBuf );
            tmpStr = tmpBuf;
            twitterObj.getOAuth().setOAuthPin( tmpStr );
        }
        else
        {
            /* Else, pass auth url to twitCurl and get it via twitCurl PIN handling */
            twitterObj.oAuthHandlePIN( authUrl );
        }

        /* Step 4: Exchange request token with access token */
        twitterObj.oAuthAccessToken();

        /* Step 5: Now, save this access token key and secret for future use without PIN */
        twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

        /* Step 6: Save these keys in a file or wherever */
        std::ofstream oAuthTokenKeyOut;
        std::ofstream oAuthTokenSecretOut;

        oAuthTokenKeyOut.open( "twitterClient_token_key.txt" );
        oAuthTokenSecretOut.open( "twitterClient_token_secret.txt" );

        oAuthTokenKeyOut.clear();
        oAuthTokenSecretOut.clear();

        oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
        oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

        oAuthTokenKeyOut.close();
        oAuthTokenSecretOut.close();
    }
    /* OAuth flow ends */
    //printf( "auth end\n" );
}

bool Twit::read_timeline(std::string& resp, int64_t last_id)
{
    int count = 100;
    std::stringstream ss;
    ss << "?count=";
    ss << count;
    ss << "&since_id=";
    ss << last_id;

    std::cout << ss.str() << std::endl;
    if( twitterObj.timelinePublicGet(ss.str())) {
      twitterObj.getLastWebResponse( replyMsg );
      resp = replyMsg;
      return true;
    } else {
      twitterObj.getLastCurlError( replyMsg );
    }

    return false;
}


Twit::~Twit()
{
    printf("destract\n");
}

void Twit::post(const char *text)
{
  printf("post %s\n", text);
  
  /* Post a new status message */
  memset( tmpBuf, 0, 1024 );
  printf( "\nEnter a new status message: " );
  //gets( tmpBuf );
  //tmpStr = tmpBuf;
  //replyMsg = "";
  if( twitterObj.statusUpdate( text ) )
  {
    twitterObj.getLastWebResponse( replyMsg );
    //printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
  }
  else
  {
    twitterObj.getLastCurlError( replyMsg );
    printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
    twitterObj.getLastWebResponse( replyMsg );
  }
}
