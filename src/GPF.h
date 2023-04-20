#ifndef _GeneralPurposeFunctions
#define _GeneralPurposeFunctions

#include "ofMain.h"
#include <curl/curl.h>

namespace ofxModule {

	struct PostRequestSettings {
		string url;
		string token = "";
		float timeout = 3;
		string user = "";
		map<string, string> header;
	};

	struct PostRequestJsonSettings : PostRequestSettings{
		ofJson content;
	};

	struct PostRequestFileSettings : PostRequestSettings {
		string filename;
	};

	/// \brief as the name indicates, a class with static useful functions
	class GPF {
	public:
		/// \brief converts a UNIX timestampstring to a string of format "%y-%m-%dT%H:%M:%S+0000"
		static string UnixToIsoTimeStamp(string parameter);
		static string timeCodeToTimestring(string code);

		static ofColor colorFromJson(ofJson val);
		static ofVec2f vec2fFromJson(ofJson val);
		static ofVec3f vec3fFromJson(ofJson val);

		static ofTexture loadTexture(string path);
		static vector<string> listFiles(ofJson path);
		static vector<string> listFolders(string path);

		static ofHttpResponse sendPostRequestJson(string url, ofJson content, string token = "");
		static ofHttpResponse sendPostRequestJson(PostRequestJsonSettings s);
		static ofHttpResponse sendPostRequestFile(PostRequestFileSettings s);

	protected:
		static vector<string> listFiles(ofDirectory dir);

	private:


	};

}

#endif
