#pragma once

#include "include/Jni_Stuff.h"
#include "AxiomRuntime/enc/StrEnc.h"
#include "AxiomRuntime/enc/json.hpp"
#include "AxiomRuntime/enc/md5.h"
#include "curl/curl-android-arm64-v8a/include/curl/curl.h"
static std::string EXP = "";
static std::string storedKey = "";
static char s[256];
JavaVM *VM;
std::string md5(std::string s);
using json = nlohmann::json;
std::string g_Token, g_Auth;
static std::string err;
bool bValid = true;
bool isCheckCompleted = true;
bool isLogin = false;

std::string getClipboard()
{

	std::string result;
	JNIEnv *env;

	VM->AttachCurrentThread(&env, NULL);

	auto looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
	auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"),
												OBFUSCATE("()V"));
	env->CallStaticVoidMethod(looperClass, prepareMethod);

	jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
	jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
	jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass,
															   sCurrentActivityThreadField);

	jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass,
														OBFUSCATE("mInitialApplication"),
														OBFUSCATE("Landroid/app/Application;"));
	jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread,
													  mInitialApplicationField);

	auto contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
	auto getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"),
												   OBFUSCATE(
													   "(Ljava/lang/String;)Ljava/lang/Object;"));

	auto str = env->NewStringUTF(OBFUSCATE("clipboard"));
	auto clipboardManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
	env->DeleteLocalRef(str);

	jclass ClipboardManagerClass = env->FindClass(OBFUSCATE("android/content/ClipboardManager"));
	auto getText = env->GetMethodID(ClipboardManagerClass, OBFUSCATE("getText"),
									OBFUSCATE("()Ljava/lang/CharSequence;"));

	jclass CharSequenceClass = env->FindClass(OBFUSCATE("java/lang/CharSequence"));
	auto toStringMethod = env->GetMethodID(CharSequenceClass, OBFUSCATE("toString"),
										   OBFUSCATE("()Ljava/lang/String;"));

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
	env->DeleteLocalRef(contextClass);
	env->DeleteLocalRef(mInitialApplication);
	env->DeleteLocalRef(activityThreadClass);
	VM->DetachCurrentThread();
	return result.c_str();
}

struct MemoryStruct
{
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	auto *mem = (struct MemoryStruct *)userp;

	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == nullptr)
	{
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

const char *GetAndroidID(JNIEnv *env, jobject context)
{
	jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
	jmethodID getContentResolverMethod = env->GetMethodID(contextClass,
														  OBFUSCATE("getContentResolver"),
														  OBFUSCATE(
															  "()Landroid/content/ContentResolver;"));
	jclass settingSecureClass = env->FindClass(OBFUSCATE("android/provider/Settings$Secure"));
	jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass, OBFUSCATE("getString"),
													   OBFUSCATE(
														   "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;"));
	auto obj = env->CallObjectMethod(context, getContentResolverMethod);
	auto str = (jstring)env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj,
													env->NewStringUTF(OBFUSCATE("android_id")));
	return env->GetStringUTFChars(str, nullptr);
}

const char *GetDeviceModel(JNIEnv *env)
{
	jclass buildClass = env->FindClass(OBFUSCATE("android/os/Build"));
	jfieldID modelId = env->GetStaticFieldID(buildClass, OBFUSCATE("MODEL"),
											 OBFUSCATE("Ljava/lang/String;"));
	auto str = (jstring)env->GetStaticObjectField(buildClass, modelId);
	return env->GetStringUTFChars(str, nullptr);
}

const char *GetDeviceBrand(JNIEnv *env)
{
	jclass buildClass = env->FindClass(OBFUSCATE("android/os/Build"));
	jfieldID modelId = env->GetStaticFieldID(buildClass, OBFUSCATE("BRAND"),
											 OBFUSCATE("Ljava/lang/String;"));
	auto str = (jstring)env->GetStaticObjectField(buildClass, modelId);
	return env->GetStringUTFChars(str, nullptr);
}

const char *GetPackageName(JNIEnv *env, jobject context)
{
	jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
	jmethodID getPackageNameId = env->GetMethodID(contextClass, OBFUSCATE("getPackageName"),
												  OBFUSCATE("()Ljava/lang/String;"));
	auto str = (jstring)env->CallObjectMethod(context, getPackageNameId);
	return env->GetStringUTFChars(str, nullptr);
}

const char *GetDeviceUniqueIdentifier(JNIEnv *env, const char *uuid)
{
	jclass uuidClass = env->FindClass(OBFUSCATE("java/util/UUID"));
	auto len = strlen(uuid);
	jbyteArray myJByteArray = env->NewByteArray(len);
	env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte *)uuid);
	jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass,
															   OBFUSCATE("nameUUIDFromBytes"),
															   OBFUSCATE("([B)Ljava/util/UUID;"));
	jmethodID toStringMethod = env->GetMethodID(uuidClass, OBFUSCATE("toString"),
												OBFUSCATE("()Ljava/lang/String;"));
	auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
	auto str = (jstring)env->CallObjectMethod(obj, toStringMethod);
	return env->GetStringUTFChars(str, nullptr);
}

//========================= MAIN LOGIN ENCRYPTION =======================\\


#include <string>
#include <vector>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdexcept>
#include <openssl/rand.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/hmac.h>

std::string CalcMD5(std::string s)
{
	std::string result;

	unsigned char hash[MD5_DIGEST_LENGTH];
	char tmp[4];

	MD5_CTX md5;
	MD5_Init(&md5);
	MD5_Update(&md5, s.c_str(), s.length());
	MD5_Final(hash, &md5);
	for (unsigned char i : hash)
	{
		sprintf(tmp, "%02x", i);
		result += tmp;
	}
	return result;
}

std::string Base64Decode(const std::string &input)
{
	BIO *bio, *b64;
	int decodeLen = (int)input.length();
	char *buffer = (char *)malloc(decodeLen);
	memset(buffer, 0, decodeLen);

	bio = BIO_new_mem_buf(input.c_str(), -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // ignore newlines
	int length = BIO_read(bio, buffer, decodeLen);
	std::string output(buffer, length);
	BIO_free_all(bio);
	free(buffer);
	return output;
}

namespace Base64
{
	inline std::string Encode(const std::string &in)
	{
		BIO *bio, *b64;
		BUF_MEM *bufferPtr;
		b64 = BIO_new(BIO_f_base64());
		bio = BIO_new(BIO_s_mem());
		bio = BIO_push(b64, bio);
		BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
		BIO_write(bio, in.data(), (int)in.size());
		BIO_flush(bio);
		BIO_get_mem_ptr(bio, &bufferPtr);
		std::string out(bufferPtr->data, bufferPtr->length);
		BIO_free_all(bio);
		return out;
	}

	inline std::string Decode(const std::string &in)
	{
		BIO *bio, *b64;
		int inLen = (int)in.size();
		std::unique_ptr<char[]> buffer(new char[inLen]);
		b64 = BIO_new(BIO_f_base64());
		bio = BIO_new_mem_buf((void *)in.data(), inLen);
		bio = BIO_push(b64, bio);
		BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
		int decodedLen = BIO_read(bio, buffer.get(), inLen);
		if (decodedLen <= 0)
		{
			BIO_free_all(bio);
			throw std::runtime_error("Base64 decode failure");
		}
		std::string out(buffer.get(), decodedLen);
		BIO_free_all(bio);
		return out;
	}
} // namespace Base64

std::string HMAC_SHA256(const std::string &data, const std::string &key)
{
	unsigned char *digest;
	digest = HMAC(
		EVP_sha256(),
		key.c_str(), key.length(),
		(unsigned char *)data.c_str(), data.length(),
		NULL, NULL);

	char mdString[65];
	for (int i = 0; i < 32; i++)
		sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
	mdString[64] = 0;

	return std::string(mdString);
}

std::string AES_KEY_BASE64 = OBFUSCATE("ijIe7lzCGmmunuhiZ6I/f97NNBAVlLmhaEsfDZJe8eU=");
std::string AES_KEY = Base64::Decode(AES_KEY_BASE64); // <-- decode first

std::string AES256GCMDecrypt(
	const std::string &ciphertext,
	const std::string &iv,
	const std::string &tag,
	const std::string &key)
{
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	if (!ctx)
		throw std::runtime_error("Failed to create EVP context");

	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1)
		throw std::runtime_error("EVP_DecryptInit_ex failed");

	if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), NULL) != 1)
		throw std::runtime_error("EVP_CTRL_GCM_SET_IVLEN failed");

	if (EVP_DecryptInit_ex(ctx, NULL, NULL, (const unsigned char *)key.data(), (const unsigned char *)iv.data()) != 1)
		throw std::runtime_error("EVP_DecryptInit_ex key/iv failed");

	std::vector<unsigned char> plaintext(ciphertext.size());
	int len;

	if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, (const unsigned char *)ciphertext.data(), ciphertext.size()) != 1)
		throw std::runtime_error("EVP_DecryptUpdate failed");

	int plaintext_len = len;

	if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tag.size(), (void *)tag.data()) != 1)
		throw std::runtime_error("EVP_CTRL_GCM_SET_TAG failed");

	if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1)
		throw std::runtime_error("EVP_DecryptFinal_ex failed: authentication failed");

	plaintext_len += len;
	EVP_CIPHER_CTX_free(ctx);

	return std::string((char *)plaintext.data(), plaintext_len);
}

//========================= MAIN LOGIN =======================\\

std::string Login(const char *user_key)
{
	JNIEnv *env;
	VM->AttachCurrentThread(&env, nullptr);

	std::string hwid = user_key;
	jobject context = getGlobalContext(env);
	hwid += GetAndroidID(env, context);
	hwid += GetDeviceModel(env);
	hwid += GetDeviceBrand(env);
	std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());

	VM->DetachCurrentThread();

	std::string errMsg;

	struct MemoryStruct chunk{};
	chunk.memory = (char *)malloc(1);
	chunk.size = 0;

	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		std::string srcStr = Tools::RandomString(32);

		time_t now_t = time(0);
		std::string now_s = std::to_string(now_t);

		std::string api_key = oxorany("https://whitecrowns.xyz/public/connect");
		curl_easy_setopt(curl, CURLOPT_URL, api_key.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, /*POST*/ StrEnc(OBFUSCATE("SZcB"), OBFUSCATE("\x3\x15\x30\x16"), 4).c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, /*Content-Type: application/x-www-form-urlencoded*/ StrEnc(OBFUSCATE("ro3lzAG#P nZ=`\"A2312IayX&_Abzep0a-=''oGGo-WODQY"), OBFUSCATE("\x31\x0\x5d\x18\x1f\x2f\x33\xe\x4\x59\x1e\x3f\x7\x40\x43\x31\x42\x5f\x58\x51\x28\x15\x10\x37\x48\x70\x39\x4f\xd\x12\x7\x1d\x7\x42\x4f\x4a\xa\x1a\x35\x2b\xa\x43\x34\x20\x20\x34\x3d"), 47).c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		std::string message = std::string(oxorany("game=CFL")) +
							  oxorany("&auth=") + srcStr +
							  oxorany("&user_key=") + user_key +
							  oxorany("&serial=") + UUID +
							  oxorany("&t=") + now_s;

		std::string sig = HMAC_SHA256(message, AES_KEY);

		char data[512];
		snprintf(data, sizeof(data),
				 oxorany("game=CFL&auth=%s&user_key=%s&serial=%s&t=%s&sig=%s"),
				 srcStr.c_str(), user_key, UUID.c_str(), now_s.c_str(), sig.c_str());

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		CURLcode res = curl_easy_perform(curl);
		if (res == CURLE_OK)
		{
			try
			{
				json result = json::parse(chunk.memory);

				if (result.contains(oxorany("enc")) && result[oxorany("enc")].get<bool>() == oxorany(true))
				{
					std::string payloadB64 = result[oxorany("payload")].get<std::string>();
					std::string receivedHMAC = result[oxorany("hmac")].get<std::string>();

					std::string raw = Base64Decode(payloadB64);
					if (raw.size() < 28)
						throw std::runtime_error(oxorany("Invalid payload length"));

					std::string iv = raw.substr(0, 12);
					std::string tag = raw.substr(12, 16);
					std::string ciphertext = raw.substr(28);

					std::string computedHMAC = HMAC_SHA256(payloadB64, AES_KEY);
					if (computedHMAC != receivedHMAC)
					{
						throw std::runtime_error(oxorany("HMAC mismatch - payload tampered"));
					}

					std::string decryptedJson = AES256GCMDecrypt(ciphertext, iv, tag, AES_KEY);
					json data = json::parse(decryptedJson);

					auto key_status = OBFUSCATE("status");

					if (data.contains(oxorany("status")) && data[key_status].is_number_integer() && data[key_status].get<int>() == oxorany(945734))
					{
						auto key_data = OBFUSCATE("data");
						auto key_token = OBFUSCATE("token");
						auto key_rng = OBFUSCATE("rng");
						auto key_exp = OBFUSCATE("expiredDate");
						auto CHECKED = OBFUSCATE("checked");
						std::string token = data[key_data][key_token].get<std::string>();
						time_t rng = data[key_data][key_rng].get<time_t>();
						EXP = data[key_data][key_exp].get<std::string>();
						std::string checked = data[key_data][CHECKED].get<std::string>();

						auto respbyme = md5(user_key);
						if (!strcmp(respbyme.c_str(), token.c_str()))
						{

							if (rng + 30 > time(0))
							{

								std::string auth = oxorany("CFL");
								auth += oxorany("-");
								auth += user_key;
								auth += oxorany("-");
								auth += UUID;
								auth += oxorany("-");
								auth += srcStr;
								auth += oxorany("-");
								auth += now_s;
								auth += oxorany("-");
								auth += /*Nh3Dv2WJ9jxfsbEzqWjRlA4KgFY9VQ8H*/ StrEnc(OBFUSCATE("{eDN3!oS<8:o1YWfb2^7uSYSt`AM+y@J"), OBFUSCATE("\x35\xd\x77\xa\x45\x13\x38\x19\x5\x52\x42\x9\x42\x3b\x12\x1c\x13\x65\x34\x65\x19\x12\x6d\x18\x13\x26\x18\x74\x7d\x28\x78\x2"), 32).c_str();

								std::string outputAuth = CalcMD5(auth);

								g_Token = checked;
								g_Auth = outputAuth;
								bValid = (g_Token == g_Auth);

								if (bValid)
								{
									isCheckCompleted = oxorany(true);
								}
								else
								{
									isCheckCompleted = oxorany(false);
									errMsg = oxorany("Verification failed! Noob ??");
								}
							}
							else
							{
								errMsg = oxorany("Timestamp expired");
							}
						}
						else
						{
							errMsg = oxorany("Token mismatch!");
						}
					}
					else
					{
						errMsg = data.contains(oxorany("reason")) ? data[oxorany("reason")].get<std::string>() : oxorany("Rejected");
					}
				}
				else
				{
					errMsg = result.contains(oxorany("reason")) ? result[oxorany("reason")].get<std::string>() : oxorany("Invalid server response");
				}
			}
			catch (std::exception &e)
			{
				errMsg = e.what();
			}
		}
		else
		{
			errMsg = curl_easy_strerror(res);
		}

		curl_easy_cleanup(curl);
	}

	return bValid ? OBFUSCATE("OK") : errMsg;
}
