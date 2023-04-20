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

	ofHttpResponse GPF::sendPostRequestJson(string url, ofJson content, string token)
	{
		PostRequestJsonSettings s;
		s.url = url;
		s.content = content;
		s.token = token;
		return sendPostRequestJson(s);
	}

	ofHttpResponse GPF::sendPostRequestJson(PostRequestJsonSettings s)
	{
		ofHttpResponse response;

		CURLcode ret;
		CURL *Curl;
		struct curl_slist * headerlist;
		std::string jsonstr = s.content.dump();

		headerlist = NULL;
		headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

		if (s.token != "") {
			ofLogNotice("token variable is deprecated. please use header[\"authorization\"]");
			s.token = "authorization: " + s.token;
			std::vector<char> cstr(s.token.c_str(), s.token.c_str() + s.token.size() + 1);
			headerlist = curl_slist_append(headerlist, &cstr[0]);
		}

		for (auto& value : s.header) {
			string headerCmd = value.first + ": " + value.second;
			std::vector<char> cstr(headerCmd.c_str(), headerCmd.c_str() + headerCmd.size() + 1);
			headerlist = curl_slist_append(headerlist, &cstr[0]);
		}


		Curl = curl_easy_init();
		curl_easy_setopt(Curl, CURLOPT_URL, s.url.c_str());
		curl_easy_setopt(Curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
		curl_easy_setopt(Curl, CURLOPT_USERAGENT, "curl/7.38.0");
		curl_easy_setopt(Curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(Curl, CURLOPT_MAXREDIRS, 50L);
		curl_easy_setopt(Curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
		curl_easy_setopt(Curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(Curl, CURLOPT_TCP_KEEPALIVE, 1L);
		
		if (s.user != "") {
			curl_easy_setopt(Curl, CURLOPT_USERPWD, s.user.c_str());

		}

		//response
		curl_easy_setopt(Curl, CURLOPT_TIMEOUT, s.timeout);
		curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, saveToMemory_cb);
		curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYPEER, FALSE);

		ret = curl_easy_perform(Curl);


		if (ret == CURLE_OK) {
			long http_code = 0;
			curl_easy_getinfo(Curl, CURLINFO_RESPONSE_CODE, &http_code);
			response.status = http_code;
			//cout << response.data << endl;
		}
		else {
			response.error = curl_easy_strerror(ret);
			response.status = -1;
		}

		curl_easy_cleanup(Curl);
		Curl = NULL;
		curl_slist_free_all(headerlist);
		headerlist = NULL;

		return response;
	}

	ofHttpResponse GPF::sendPostRequestFile(PostRequestFileSettings s)
	{
		s.filename = ofToDataPath(s.filename, true);
		ofHttpResponse response;

		curl_global_init(CURL_GLOBAL_ALL);

		CURL* Curl = curl_easy_init();

		curl_easy_setopt(Curl, CURLOPT_FAILONERROR, 0);
		curl_easy_setopt(Curl, CURLOPT_VERBOSE, 1L);

		curl_httppost* formpost = NULL;
		curl_httppost* lastptr = NULL;

		curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "file",
			CURLFORM_FILE, s.filename.c_str(),
			CURLFORM_CONTENTTYPE, "image/jpeg",
			CURLFORM_END);

		struct curl_slist* headerlist = curl_slist_append(NULL, "Expect:");


		if (s.token != "") {
			ofLogNotice("token variable is deprecated. please use header[\"authorization\"]");
			s.token = "authorization: " + s.token;
			std::vector<char> cstr(s.token.c_str(), s.token.c_str() + s.token.size() + 1);
			headerlist = curl_slist_append(headerlist, &cstr[0]);
		}

		for (auto& value : s.header) {
			string headerCmd = value.first + ": " + value.second;
			std::vector<char> cstr(headerCmd.c_str(), headerCmd.c_str() + headerCmd.size() + 1);
			headerlist = curl_slist_append(headerlist, &cstr[0]);
		}

		if (s.user != "") {
			curl_easy_setopt(Curl, CURLOPT_USERPWD, s.user.c_str());

		}

		string Url = s.url;
		curl_easy_setopt(Curl, CURLOPT_URL, Url.c_str());

		curl_easy_setopt(Curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(Curl, CURLOPT_HTTPPOST, formpost);

		string Reponse;
		curl_easy_setopt(Curl, CURLOPT_TIMEOUT, s.timeout);
		curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, saveToMemory_cb);
		curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYPEER, FALSE);

		CURLcode ret = curl_easy_perform(Curl);

		if (ret == CURLE_OK) {
			long http_code = 0;
			curl_easy_getinfo(Curl, CURLINFO_RESPONSE_CODE, &http_code);
			response.status = http_code;
			cout << response.data << endl;
		}
		else {
			response.error = curl_easy_strerror(ret);
			response.status = -1;
		}

		curl_easy_cleanup(Curl);
		curl_formfree(formpost);
		curl_slist_free_all(headerlist);

		curl_global_cleanup();
		Curl = NULL;
		headerlist = NULL;
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