#include <curl/curl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "Includes/StrEnc.h"
#include "xorstr.hpp"

static std::string EXP;
static std::string err;
static std::string msgErr = "Message...";
std::string onlineusers;
static std::string input;
std::string Date;
std::string Expiry;
string authtext = "";
string registInfo = "";
ImVec4 authtextcolor = { 1.0f,1.0f,1.0f,1.0f };
std::string mainkey = "";
bool isrequestcompleted = true;
bool iskeyauthinited = false;
bool isrequestcompleted1 = true;
bool iskeyauthinited1 = false;
bool showChat = false;
static std::string username;
static std::string password;
std::string OnlineUsers;
static bool isRegister = false;
std::string getClipboardText()
{
	if (!g_App)
		return "";
	auto activity = g_App->activity;
	if (!activity)
		return "";
	auto vm = activity->vm;
	if (!vm)
		return "";
	auto object = activity->clazz;
	if (!object)
		return "";
	std::string result;
	JNIEnv *env;
	vm->AttachCurrentThread(&env, 0);
	{
		auto ContextClass = env->FindClass("android/content/Context");
		auto getSystemServiceMethod = env->GetMethodID(ContextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
		auto str = env->NewStringUTF("clipboard");
		auto clipboardManager = env->CallObjectMethod(object, getSystemServiceMethod, str);
		env->DeleteLocalRef(str);
		auto ClipboardManagerClass = env->FindClass("android/content/ClipboardManager");
		auto getText = env->GetMethodID(ClipboardManagerClass, "getText", "()Ljava/lang/CharSequence;");
		auto CharSequenceClass = env->FindClass("java/lang/CharSequence");
		auto toStringMethod = env->GetMethodID(CharSequenceClass, "toString", "()Ljava/lang/String;");
		auto text = env->CallObjectMethod(clipboardManager, getText);
		if (text)
		{
			str = (jstring)env->CallObjectMethod(text, toStringMethod);
			result = env->GetStringUTFChars(str, 0);
			env->DeleteLocalRef(str);
			env->DeleteLocalRef(text);
		}
		env->DeleteLocalRef(CharSequenceClass);
		env->DeleteLocalRef(ClipboardManagerClass);
		env->DeleteLocalRef(clipboardManager);
		env->DeleteLocalRef(ContextClass);
	}
	vm->DetachCurrentThread();
	return result;
}

int OpenURL(const char* url)
{
    JavaVM* java_vm = g_App->activity->vm;
    JNIEnv* java_env = NULL;

    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR)
        return -1;

    jni_return = java_vm->AttachCurrentThread(&java_env, NULL);
    if (jni_return != JNI_OK)
        return -2;

    jclass native_activity_clazz = java_env->GetObjectClass(g_App->activity->clazz);
    if (native_activity_clazz == NULL)
        return -3;

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "AndroidThunkJava_LaunchURL", "(Ljava/lang/String;)V");
    if (method_id == NULL)
        return -4;
        
    jstring retStr = java_env->NewStringUTF(url);
    java_env->CallVoidMethod(g_App->activity->clazz, method_id, retStr);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK)
        return -5;

    return 0;
}
int RestartGame()
{
    JavaVM* java_vm = g_App->activity->vm;
    JNIEnv* java_env = NULL;

    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR)
        return -1;

    jni_return = java_vm->AttachCurrentThread(&java_env, NULL);
    if (jni_return != JNI_OK)
        return -2;

    jclass native_activity_clazz = java_env->GetObjectClass(g_App->activity->clazz);
    if (native_activity_clazz == NULL)
        return -3;

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "AndroidThunkJava_RestartGame", "()V");
    if (method_id == NULL)
        return -4;

    java_env->CallVoidMethod(g_App->activity->clazz, method_id);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK)
        return -5;

    return 0;
}

// ======================================================================== //
#include <chrono>
#include <sstream>
#include <iomanip>
std::chrono::seconds GetTimeRemaining(const std::string& expiration)
{
    // Parse the expiration date and time string into a std::tm structure
    std::tm expirationTime = {};
    std::istringstream ss(expiration);
    ss >> std::get_time(&expirationTime, "%Y-%m-%d : %H:%M");

    // Convert the expiration time to a std::chrono::system_clock::time_point
    auto expirationPoint = std::chrono::system_clock::from_time_t(std::mktime(&expirationTime));

    // Calculate the time remaining until the expiration
    auto now = std::chrono::system_clock::now();
    if (now >= expirationPoint) {
        // The expiration time has already passed, so return a zero duration.
        return std::chrono::seconds{0};
    } else {
        return std::chrono::duration_cast<std::chrono::seconds>(expirationPoint - now);
    }
}

// This function takes a duration in seconds and returns a string representation
// of the duration in the format "D days, H hours, M minutes, S seconds".
std::string FormatDuration(std::chrono::seconds duration)
{
    std::ostringstream ss;
    int days = duration.count() / (24 * 60 * 60);
    int hours = (duration.count() % (24 * 60 * 60)) / (60 * 60);
    int minutes = (duration.count() % (60 * 60)) / 60;
    int seconds = duration.count() % 60;

    if (days > 0) {
        ss << days << " days, ";
    }
    if (hours > 0) {
        ss << hours << " hours, ";
    }
    if (minutes > 0) {
        ss << minutes << " minutes ";
    }
  //  ss << seconds << " seconds";

    return ss.str();
}

const char *GetAndroidID(JNIEnv *env, jobject context)
{
    jclass contextClass = env->FindClass(/*android/content/Context*/ StrEnc("`L+&0^[S+-:J^$,r9q92(as", "\x01\x22\x4F\x54\x5F\x37\x3F\x7C\x48\x42\x54\x3E\x3B\x4A\x58\x5D\x7A\x1E\x57\x46\x4D\x19\x07", 23).c_str());
    jmethodID getContentResolverMethod = env->GetMethodID(contextClass, /*getContentResolver*/ StrEnc("E8X\\7r7ys_Q%JS+L+~", "\x22\x5D\x2C\x1F\x58\x1C\x43\x1C\x1D\x2B\x03\x40\x39\x3C\x47\x3A\x4E\x0C", 18).c_str(), /*()Landroid/content/ContentResolver;*/ StrEnc("8^QKmj< }5D:9q7f.BXkef]A*GYLNg}B!/L", "\x10\x77\x1D\x2A\x03\x0E\x4E\x4F\x14\x51\x6B\x59\x56\x1F\x43\x03\x40\x36\x77\x28\x0A\x08\x29\x24\x44\x33\x0B\x29\x3D\x08\x11\x34\x44\x5D\x77", 35).c_str());
    jclass settingSecureClass = env->FindClass(/*android/provider/Settings$Secure*/ StrEnc("T1yw^BCF^af&dB_@Raf}\\FS,zT~L(3Z\"", "\x35\x5F\x1D\x05\x31\x2B\x27\x69\x2E\x13\x09\x50\x0D\x26\x3A\x32\x7D\x32\x03\x09\x28\x2F\x3D\x4B\x09\x70\x2D\x29\x4B\x46\x28\x47", 32).c_str());
    jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass, /*getString*/ StrEnc("e<F*J5c0Y", "\x02\x59\x32\x79\x3E\x47\x0A\x5E\x3E", 9).c_str(), /*(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;*/ StrEnc("$6*%R*!XO\"m18o,0S!*`uI$IW)l_/_knSdlRiO1T`2sH|Ouy__^}%Y)JsQ:-\"(2_^-$i{?H", "\x0C\x7A\x4B\x4B\x36\x58\x4E\x31\x2B\x0D\x0E\x5E\x56\x1B\x49\x5E\x27\x0E\x69\x0F\x1B\x3D\x41\x27\x23\x7B\x09\x2C\x40\x33\x1D\x0B\x21\x5F\x20\x38\x08\x39\x50\x7B\x0C\x53\x1D\x2F\x53\x1C\x01\x0B\x36\x31\x39\x46\x0C\x15\x43\x2B\x05\x30\x15\x41\x43\x46\x55\x70\x0D\x59\x56\x00\x15\x58\x73", 71).c_str());

    auto obj = env->CallObjectMethod(context, getContentResolverMethod);
    auto str = (jstring)env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj, env->NewStringUTF(/*android_id*/ StrEnc("ujHO)8OfOE", "\x14\x04\x2C\x3D\x46\x51\x2B\x39\x26\x21", 10).c_str()));
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceModel(JNIEnv *env)
{
    jclass buildClass = env->FindClass(/*android/os/Build*/ StrEnc("m5I{GKGWBP-VOxkA", "\x0C\x5B\x2D\x09\x28\x22\x23\x78\x2D\x23\x02\x14\x3A\x11\x07\x25", 16).c_str());
    jfieldID modelId = env->GetStaticFieldID(buildClass, /*MODEL*/ StrEnc("|}[q:", "\x31\x32\x1F\x34\x76", 5).c_str(), /*Ljava/lang/String;*/ StrEnc(".D:C:ETZ1O-Ib&^h.Y", "\x62\x2E\x5B\x35\x5B\x6A\x38\x3B\x5F\x28\x02\x1A\x16\x54\x37\x06\x49\x62", 18).c_str());

    auto str = (jstring)env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceBrand(JNIEnv *env)
{
    jclass buildClass = env->FindClass(/*android/os/Build*/ StrEnc("0iW=2^>0zTRB!B90", "\x51\x07\x33\x4F\x5D\x37\x5A\x1F\x15\x27\x7D\x00\x54\x2B\x55\x54", 16).c_str());
    jfieldID modelId = env->GetStaticFieldID(buildClass, /*BRAND*/ StrEnc("@{[FP", "\x02\x29\x1A\x08\x14", 5).c_str(), /*Ljava/lang/String;*/ StrEnc(".D:C:ETZ1O-Ib&^h.Y", "\x62\x2E\x5B\x35\x5B\x6A\x38\x3B\x5F\x28\x02\x1A\x16\x54\x37\x06\x49\x62", 18).c_str());

    auto str = (jstring)env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetPackageName(JNIEnv *env, jobject context)
{
    jclass contextClass = env->FindClass(/*android/content/Context*/ StrEnc("`L+&0^[S+-:J^$,r9q92(as", "\x01\x22\x4F\x54\x5F\x37\x3F\x7C\x48\x42\x54\x3E\x3B\x4A\x58\x5D\x7A\x1E\x57\x46\x4D\x19\x07", 23).c_str());
    jmethodID getPackageNameId = env->GetMethodID(contextClass, /*getPackageName*/ StrEnc("YN4DaP)!{wRGN}", "\x3E\x2B\x40\x14\x00\x33\x42\x40\x1C\x12\x1C\x26\x23\x18", 14).c_str(), /*()Ljava/lang/String;*/ StrEnc("VnpibEspM(b]<s#[9cQD", "\x7E\x47\x3C\x03\x03\x33\x12\x5F\x21\x49\x0C\x3A\x13\x20\x57\x29\x50\x0D\x36\x7F", 20).c_str());

    auto str = (jstring)env->CallObjectMethod(context, getPackageNameId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceUniqueIdentifier(JNIEnv *env, const char *uuid)
{
    jclass uuidClass = env->FindClass(/*java/util/UUID*/ StrEnc("B/TxJ=3BZ_]SFx", "\x28\x4E\x22\x19\x65\x48\x47\x2B\x36\x70\x08\x06\x0F\x3C", 14).c_str());

    auto len = strlen(uuid);

    jbyteArray myJByteArray = env->NewByteArray(len);
    env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte *)uuid);

    jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass, /*nameUUIDFromBytes*/ StrEnc("P6LV|'0#A+zQmoat,", "\x3E\x57\x21\x33\x29\x72\x79\x67\x07\x59\x15\x3C\x2F\x16\x15\x11\x5F", 17).c_str(), /*([B)Ljava/util/UUID;*/ StrEnc("sW[\"Q[W3,7@H.vT0) xB", "\x5B\x0C\x19\x0B\x1D\x31\x36\x45\x4D\x18\x35\x3C\x47\x1A\x7B\x65\x7C\x69\x3C\x79", 20).c_str());
    jmethodID toStringMethod = env->GetMethodID(uuidClass, /*toString*/ StrEnc("2~5292eW", "\x46\x11\x66\x46\x4B\x5B\x0B\x30", 8).c_str(), /*()Ljava/lang/String;*/ StrEnc("P$BMc' #j?<:myTh_*h0", "\x78\x0D\x0E\x27\x02\x51\x41\x0C\x06\x5E\x52\x5D\x42\x2A\x20\x1A\x36\x44\x0F\x0B", 20).c_str());

    auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
    auto str = (jstring)env->CallObjectMethod(obj, toStringMethod);
    return env->GetStringUTFChars(str, 0);
}

struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL)
    {
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
//std::string vip_encode;
std::string vipr_encode;
std::string vip_encode;

char userKey[128] = "";




#include <iostream>
#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <iomanip>
#include <ctime>

// For simplicity, using nlohmann/json for JSON parsing
using json = nlohmann::json;

// Callback function to capture the response from the server
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append(static_cast<char*>(contents), newLength);
    } catch (std::bad_alloc& e) {
        // Handle memory allocation errors
        return 0;
    }
    return newLength;
}

struct channel_struct
{
	std::string author;
	std::string message;
	std::string timestamp;
};

namespace KeyAuth2 {
	class api {
	public:

		

		class subscriptions_class {
		public:
			std::string name;
			std::string expiry;
		};

		class userdata {
		public:

			// user data
			std::string username;
			std::string ip;
			std::string hwid;
			std::string createdate;
			std::string lastlogin;
			std::vector<channel_struct> channeldata;
			bool success{};
			std::string message;
			std::vector<subscriptions_class> subscriptions;
		};

		class appdata {
		public:
			// app data
			std::string numUsers;
			std::string numOnlineUsers;
			std::string numKeys;
			std::string version;
			std::string customerPanelLink;
		};

		class responsedata {
		public:
			// response data

			bool success;
			std::string message;
		};

		userdata user_data;
		appdata app_data;
		responsedata response;
	public:
		std::string sessionid, enckey;

		
		void load_user_data(nlohmann::json data) {
			api::user_data.username = data["username"];
			api::user_data.ip = data["ip"];
			if (data["hwid"].is_null()) {
				api::user_data.hwid = "none";
			}
			else {
				api::user_data.hwid = data["hwid"];
			}
			api::user_data.createdate = data["createdate"];
			api::user_data.lastlogin = data["lastlogin"];

			for (int i = 0; i < data["subscriptions"].size(); i++) { // Prompto#7895 & stars#2297 was here
				subscriptions_class subscriptions;
				subscriptions.name = data["subscriptions"][i]["subscription"];
				subscriptions.expiry = data["subscriptions"][i]["expiry"];
				api::user_data.subscriptions.emplace_back(subscriptions);
			}
		}
		
		void load_app_data(nlohmann::json data) {
			api::app_data.numUsers = data["numUsers"];
			api::app_data.numOnlineUsers = data["numOnlineUsers"];
			api::app_data.numKeys = data["numKeys"];
			api::app_data.version = data["version"];
			api::app_data.customerPanelLink = data["customerPanelLink"];
		}
		
		void load_response_data(nlohmann::json data) {
			api::user_data.success = data["success"];
			api::user_data.message = data["message"];
		}

		void load_channel_data(nlohmann::json data) {
			api::user_data.success = data["success"];
			api::user_data.message = data["message"];
			for (auto sub : data["messages"])
			{
				std::string authoroutput = sub[("author")];
				std::string messageoutput = sub[("message")];
				std::string timestampoutput = sub[("timestamp")];
				authoroutput.erase(remove(authoroutput.begin(), authoroutput.end(), '"'), authoroutput.end());
				messageoutput.erase(remove(messageoutput.begin(), messageoutput.end(), '"'), messageoutput.end());
				timestampoutput.erase(remove(timestampoutput.begin(), timestampoutput.end(), '"'), timestampoutput.end());
				channel_struct output = { authoroutput , messageoutput, timestampoutput };
				api::user_data.channeldata.push_back(output);
			}
		}

		nlohmann::json response_decoder;

	};
}
KeyAuth2::api myApi;
class KeyAuth {
    std::string appname;
    std::string ownerid;
    std::string version;
    std::string url;
    std::string sellerkey;
    std::string sessionid;
    bool initialized;

public:
    KeyAuth(const std::string& appname, const std::string& ownerid, const std::string& version, const std::string& url, const std::string& sellerkey)
        : appname(appname), ownerid(ownerid), version(version), url(url), sellerkey(sellerkey), initialized(false) {}

    void init() {
        CURL* curl = curl_easy_init();
        std::string response_string;
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            std::string postFields = "type=init&ver=" + version + "&name=" + appname + "&ownerid=" + ownerid;
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable peer verification
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // Disable host verification
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            } else {
                json response_json;
                try {
                    response_json = json::parse(response_string);
                    if (response_json["success"].get<bool>()) {
                        sessionid = response_json["sessionid"].get<std::string>();
						myApi.load_app_data(response_json[(XorStr("appinfo"))]);
                        initialized = true;
                    } else {
                        std::cout << "Initialization failed: " << response_json["message"].get<std::string>() << std::endl;
                    }
                } catch (const json::exception& e) {
                    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                }
            }
            curl_easy_cleanup(curl);
        }
    }
std::vector<channel_struct> chatget(std::string channelname)
{
	 
    if (!initialized) {
        std::cerr << "Please initialize first." << std::endl;
    }

    CURL* curl = curl_easy_init();
    std::string response_string;
    bool loginSuccess = false;
    if (curl) {
        auto data =
		XorStr("type=chatget") +
		XorStr("&channel=") + channelname +
		XorStr("&sessionid=") + sessionid +
		XorStr("&name=") + appname +
		XorStr("&ownerid=") + ownerid;
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            json response_json;
            try {
                response_json = json::parse(response_string);
                if (!response_json["success"].get<bool>()) {
                    std::cerr << "Login failed: " << response_json["message"].get<std::string>() << std::endl;
                   } else {
                     myApi.load_channel_data(response_json);
					 std::vector<channel_struct> buff;
				     for (const auto message : response_json[("messages")]) {
					buff.push_back(channel_struct{ message["message"],message["author"] ,message["timestamp"] });
				   }
				return buff;
                }
            } catch (const json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }
        curl_easy_cleanup(curl);
    }
}

bool chatsend(std::string channelname, std::string message)
{
	 
    if (!initialized) {
        std::cerr << "Please initialize first." << std::endl;
        return false;
    }

    CURL* curl = curl_easy_init();
    std::string response_string;
    json response_json;
    if (curl) {
        auto data =
		XorStr("type=chatsend") +
		XorStr("&message=") + message +
		XorStr("&channel=") + channelname +
		XorStr("&sessionid=") + sessionid +
		XorStr("&name=") + appname +
		XorStr("&ownerid=") + ownerid;
		
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            
            try {
                response_json = json::parse(response_string);
                if (!response_json["success"].get<bool>()) {
                    msgErr =response_json["message"].get<std::string>();
                } else {
                     myApi.load_response_data(response_json);
                }
            } catch (const json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }
    return response_json[("success")];
}

int license(const std::string license) {
    if (!initialized) {
        err = "Please initialize first.";
        return 10;
    }
	if (!g_App)
        return 4453;

    auto activity = g_App->activity;
    if (!activity)
        return 4453;

    auto vm = activity->vm;
    if (!vm)
        return 4453;

    auto object = activity->clazz;
    if (!object)
        return 4453;

    JNIEnv *env;
    vm->AttachCurrentThread(&env, 0);

    std::string hwid = license;
    hwid += GetAndroidID(env, object);
    hwid += GetDeviceModel(env);
    hwid += GetDeviceBrand(env);

    std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());

    vm->DetachCurrentThread();
    CURL* curl = curl_easy_init();
    std::string response_string;
    
    if (curl) {
        std::string postFields = "type=license&key=" + license + "&hwid=" + UUID + "&sessionid=" + sessionid + "&name=" + appname + "&ownerid=" + ownerid;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable peer verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // Disable host verification
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
           err = curl_easy_strerror(res);
        } else {
            json json;
            try {
                json = json::parse(response_string);
				if (json[("success")])
			{
				myApi.load_user_data(json[("info")]);
				return 0;
			}
			else
			{
				if (std::string(json[("message")]) == "The user is banned")
				{
					return 1;
				}
				else if (std::string(json[("message")]) == "HWID Doesn't match. Ask for key reset.")
				{
					return 2;
				}
				else if (std::string(json[("message")]) == "Key Not Found.")
				{
					return 3;
				}
				else if (std::string(json[("message")]) == "No active subscriptions found.")
				{
					return 4;
				}
			}
            } catch (const json::exception& e) {
                err = "Error parsing JSON: %s" , e.what();
            }
        }

        curl_easy_cleanup(curl);
    }
    return 4452;
}



void regist(const std::string licenser) {
    if (!initialized) {
        std::cerr << "Please initialize first." << std::endl;
        return;
    }

    CURL* curl = curl_easy_init();
    std::string response_string;
    if (curl) {
           std::string lic = "https://licenseauth.host/api/seller/?sellerkey=" + sellerkey + "&type=add&format=JSON&expiry=7&mask=" + licenser + "&amount=1";
           curl_easy_setopt(curl, CURLOPT_URL, lic.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable peer verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // Disable host verification
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            json response_json;
            try {
                response_json = json::parse(response_string);
                if (!response_json["success"].get<bool>()) {
                } else {
				}
            } catch (const json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }
        curl_easy_cleanup(curl);
    }
}
   int licenseCheck(const std::string license) {
    if (!initialized) {
        std::cerr << "Please initialize first." << std::endl;
        return 0;
    }

    CURL* curl = curl_easy_init();
    std::string response_string;
    if (curl) {

        std::string lic = "https://licenseauth.host/api/seller/?sellerkey=" + sellerkey + "&type=verify&key=" + license;
        curl_easy_setopt(curl, CURLOPT_URL, lic.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable peer verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // Disable host verification
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            json response_json;
            try {
                response_json = json::parse(response_string);
                if (!response_json["success"].get<bool>()) {
                  return 1;
                } else {
			      return 0;
				}
            } catch (const json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }
        curl_easy_cleanup(curl);
    }
    return 63838;
}

};

#include <iostream>
#include <ctime>

std::string tm_to_readable_time(std::tm ctx) {
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%m/%d %H:%M", &ctx); // Format for year-month-day hour:minute:second
    return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
    auto cv = strtol(timestamp.c_str(), NULL, 10); // long
    return static_cast<time_t>(cv);
}

static std::tm timet_to_tm(time_t timestamp) {
    std::tm context;
    localtime_r(&timestamp, &context); // Use localtime_r for thread safety
    return context;
}

