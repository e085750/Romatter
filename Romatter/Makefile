CXXFLAGS += -g -Wall -I./ -I./include/
WXFLAGS = `wx-config --cxxflags --libs`
defalt: tltest

tltest: tltest.o Twit.o twitCurlEx.o MyFrame.o MyThread.o
	$(CXX) $(CXXFLAGS) -o tltest $^ $(WXFLAGS) -ltwitcurl 

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c $< $(WXFLAGS)

Twit.o: Twit.cpp
	$(CXX) $(CXXFLAGS) -c $<

tltest.o: tltest.cpp
	$(CXX) $(CXXFLAGS) -c $< $(WXFLAGS)

MyFrame.o: MyFrame.cpp
	$(CXX) $(CXXFLAGS) -c $< $(WXFLAGS)

MyThread.o: MyThread.cpp
	$(CXX) $(CXXFLAGS) -c $< $(WXFLAGS)

twitCurlEx.o: twitCurlEx.cpp
	$(CXX) $(CXXFLAGS) -c $< -I./include -I../twitcurl/libtwitcurl

clean:
	rm -rf  tltest test.o twitterClient.o tltest.o Twit.o twitCurlEx.o MyFrame.o MyThread.o
