#include "GPF.h"

namespace {
	size_t saveToMemory_cb(void *buffer, size_t size, size_t nmemb, void *userdata) {
		auto response = (ofHttpResponse*)userdata;
		response->data.append((const char*)buffer, size * nmemb);
		return size * nmemb;
	}
}

namespace ofxModule {

	string GPF::UnixToIsoTimeStamp(string parameter) {
		char newTime[300];
		int intTime = atoi(parameter.c_str());
		time_t rawTime = intTime;
		struct tm *timeinfo = localtime(&rawTime);
		strftime(newTime, 300, "%y-%m-%dT%H:%M:%S+0000", timeinfo);
		std::string time = newTime;
		return newTime;
	}

	string GPF::timeCodeToTimestring(string code) {
		ofStringReplace(code, "[y]", ofToString(ofGetYear()).substr(2));
		ofStringReplace(code, "[yy]", ofToString(ofGetYear()));
		ofStringReplace(code, "[m]", ofToString(ofGetMonth()));
		ofStringReplace(code, "[mm]", ofGetMonth() < 10 ? "0" + ofToString(ofGetMonth()) : ofToString(ofGetMonth()));
		ofStringReplace(code, "[d]", ofToString(ofGetDay()));
		ofStringReplace(code, "[dd]", ofGetDay() < 10 ? "0" + ofToString(ofGetDay()) : ofToString(ofGetDay()));
		ofStringReplace(code, "[h]", ofToString(ofGetHours()));
		ofStringReplace(code, "[hh]", ofGetHours() < 10 ? "0" + ofToString(ofGetHours()) : ofToString(ofGetHours()));
		ofStringReplace(code, "[M]", ofToString(ofGetMinutes()));
		ofStringReplace(code, "[MM]", ofGetMinutes() < 10 ? "0" + ofToString(ofGetMinutes()) : ofToString(ofGetMinutes()));
		ofStringReplace(code, "[S]", ofToString(ofGetSeconds()));
		ofStringReplace(code, "[SS]", ofGetMinutes() < 10 ? "0" + ofToString(ofGetSeconds()) : ofToString(ofGetSeconds()));
		ofStringReplace(code, "[i]", ofGetTimestampString("%i"));
		ofStringReplace(code, "[s]", ofToString(ofGetHours() * 3600 + ofGetMinutes() * 60 + ofGetSeconds()));

		return code;
	}

	ofColor GPF::colorFromJson(ofJson val)
	{
		switch (val.size())
		{
		case 1:
			// interprete string
			if (val.is_string()) {
				
				if (ofIsStringInString(val.get<string>(), ".")) {
					ofParameter<ofFloatColor> pTemp;
					pTemp.fromString(val);
					return pTemp.get();
				}
				else {
					ofParameter<ofColor> pTemp;
					pTemp.fromString(val);
					return pTemp.get();
				}
				
			}// interprete array
			else
				return ofColor(val[0].get<int>());

			break;
		case 2:
			return ofColor(val[0], val[0], val[0], val[1]);
			break;
		case 3:
			return ofColor(val[0], val[1], val[2]);
			break;
		case 4:
			return ofColor(val[0], val[1], val[2], val[3]);
			break;
		default:
			return ofColor();
			break;
		}

	}

	ofVec2f GPF::vec2fFromJson(ofJson val)
	{
		switch (val.size())
		{
		case 1:
			// interprete string
			if (val.is_string()) {
				ofParameter<ofVec2f> pTemp;
				pTemp.fromString(val);
				return pTemp.get();
			}// interprete array
			else
				return ofVec2f(val[0].get<int>());

			break;
		case 2:
			return ofVec2f(val[0], val[1]);
			break;
		default:
			return ofVec2f();
			break;
		}
	}

	ofVec3f GPF::vec3fFromJson(ofJson val)
	{
		switch (val.size())
		{
		case 1:
			// interprete string
			if (val.is_string()) {
				ofParameter<ofVec3f> pTemp;
				pTemp.fromString(val);
				return pTemp.get();
			}// interprete array
			else
				return ofVec3f(val[0].get<int>());

			break;
		case 2:
			return ofVec3f(val[0], val[1]);
			break;
		case 3:
			return ofVec3f(val[0], val[1], val[2]);
			break;
		default:
			return ofVec2f();
			break;
		}
	}

	ofTexture GPF::loadTexture(string path) {
		ofTexture ret;
		ofStringReplace(path, "\\", "/");
		//ofStringReplace(path, " ", "+");
		if (!ofLoadImage(ret, path)) {
			ofLogError("error loading texture : " + path);
		}
		return ret;
	}

	vector<string> GPF::listFiles(ofJson path)
	{
		vector<string> fileList;
		for (auto& elem:path)
		{
			ofDirectory dir{ elem.get<string>() };
			auto v = listFiles(dir);
			fileList.insert(end(fileList), begin(v), end(v));
		}

		return fileList;
	}

	vector<string> GPF::listFolders(string path)
	{
		vector<string> folderList;
			ofDirectory dir{ path };
			if (dir.isDirectory()) {
				for (auto& elem : dir) {
					if (elem.isDirectory()) {
						string path = elem.getAbsolutePath();
						ofStringReplace(path, "\\", "/");
						if (path.back() == '/') {
							path.resize(path.size() - 1);
						}
						folderList.push_back(path);
					}
				}
			}

		return folderList;
	}

	ofHttpResponse GPF::sendPostRequest(string url, ofJson content, string token)
	{
		PostRequestSettings s;
		s.url = url;
		s.content = content;
		s.token = token;
		return sendPostRequest(s);
	}

	ofHttpResponse GPF::sendPostRequest(PostRequestSettings s)
	{
		ofHttpResponse response;

		CURLcode ret;
		CURL *hnd;
		struct curl_slist *slist1;
		std::string jsonstr = s.content.dump();

		slist1 = NULL;
		slist1 = curl_slist_append(slist1, "Content-Type: application/json");

		s.token = "authorization: " + s.token;
		std::vector<char> cstr(s.token.c_str(), s.token.c_str() + s.token.size() + 1);
		slist1 = curl_slist_append(slist1, &cstr[0]);

		hnd = curl_easy_init();
		curl_easy_setopt(hnd, CURLOPT_URL, s.url.c_str());
		curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, jsonstr.c_str());
		curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
		curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

		//response
		curl_easy_setopt(hnd, CURLOPT_TIMEOUT, s.timeout);
		curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, saveToMemory_cb);
		curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, FALSE);

		ret = curl_easy_perform(hnd);


		if (ret == CURLE_OK) {
			long http_code = 0;
			curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &http_code);
			response.status = http_code;
		}
		else {
			response.error = curl_easy_strerror(ret);
			response.status = -1;
		}

		curl_easy_cleanup(hnd);
		hnd = NULL;
		curl_slist_free_all(slist1);
		slist1 = NULL;

		return response;
	}

	vector<string> GPF::listFiles(ofDirectory dir)
	{
		vector<string> fileList;
		if (dir.isDirectory()) {
			for (auto& elem:dir){
				auto v = listFiles(ofDirectory{ elem });
				fileList.insert(end(fileList), begin(v), end(v));
				}
			}
		else {
			string path = dir.getAbsolutePath();
			ofStringReplace(path, "\\", "/");
			if (path.back() == '/') {
				path.resize(path.size() - 1);
			}
			fileList.push_back(path);
		}
		return fileList;
	}

}