#include <cassert>
#include<iostream>
#include<list>
#include<map>

#include "MemberTokenParser.h"
#include "Utf8TextICEncoder.h"
#include "Utf8TextICDecoder.h"

using namespace std;

int main(int argc, char* argv[]) {

    string str = "space\xE5\xA5\xBD";
    string testStr = "Key with \n and space好";

    list<string> keys;
    keys.push_back(str);
    keys.push_back(testStr);
    keys.push_back("Key::_?=,");
    keys.push_back( "Key");

    // cout<<keys.size();
    
    Utf8TextICEncoder encoder;
    string encoded = encoder.encodeKeys(keys);
    cout<< encoded << endl;

    list<string> output;

    string encodedInput("serviceCallTraceData=%7B%22caller%22%3A%7B%22container%22%3A%22curli%22%2C%22service%22%3A%22curli%22%2C%22env%22%3A%22dev%22%2C%22instance%22%3A%22dev%22%2C%22machine%22%3A%22vigchand-mn1.linkedin.biz%22%2C%22version%22%3A%220.0.1%22%7D%2C%22treeId%22%3A%22AAYC83VXjoF/RufIEnwjKg%3D%3D%22%2C%22enabled%22%3Atrue%2C%22scale%22%3A1%2C%22context%22%3A%7B%22source%22%3A%22curli%22%2C%22forceTraceEnabled%22%3A%22true%22%2C%22debugEnabled%22%3A%22true%22%2C%22traceGroupingKey%22%3A%22vigchand%22%7D%2C%22rqId%22%3A0%2C%22version%22%3A1%7D,com.linkedin.container.rpc.trace.rpcTrace=%28vigchand-mn1.linkedin.biz%2Ccurli%2C2020/01/01%2001%3A00%3A00.000%29,QD-qd.routingKey=vigchand-mn1.linkedin.biz,mik=AwMPRxx4AhTF5wAAAYn4sX57AAABifjobPtVACWVMZIc5evCcG1CSuOMogRDY1oXdpbEVwdJJHYH_EPaL9XU4QROmcH1Qi5yjs9YtPLpZDvjTAaSk71ikBAnWws");

    map<string, string> keyValuePairs;
    keyValuePairs.insert(pair<string,string>("user-agent-1", str));
    keyValuePairs.insert(pair<string,string>("user-agent-2", testStr));
    keyValuePairs.insert(pair<string,string>("user-agent-3", "Key::_?=,"));
    keyValuePairs.insert(pair<string,string>("user-agent-4::?", "Key"));

    encoded = encoder.encodeKeyValuePairs(keyValuePairs);
    cout<< encoded << endl;

    map<string, string> outputKeyValuePairs;
    Utf8TextICDecoder decoder2(encodedInput);
    decoder2.decodeKeyValuePairs(outputKeyValuePairs);
    auto mapIter = outputKeyValuePairs.begin();
    while(mapIter != outputKeyValuePairs.end()) {
        cout << mapIter-> second << "   " <<  keyValuePairs[mapIter->first] << "   " << mapIter -> first << endl;
        mapIter ++;
    }
    MemberTokenParser memberTokenParser(outputKeyValuePairs["mik"]);
    memberTokenParser.getParsedToken();

    
}
