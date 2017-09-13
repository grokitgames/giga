
#ifndef httprequest_hpp
#define httprequest_hpp

/**
* An HTTP(s) call using the CURL library
*/
class HttpRequest : public ScriptableObject {
public:
	HttpRequest();
	~HttpRequest();

	GIGA_CLASS_NAME("HttpRequest");

	// Create a new request
	void Create(std::string url);

	// Add POST data
	void AddPostData(std::string name, std::string value);

	// Read the HTTP response
	static size_t _ProcessResponse(char *buffer, size_t size, size_t nitems, void *userdata);
	void SaveResponse(char *buffer, unsigned int size);

	// Execute the request
	unsigned char* Execute();

	/**
	* Scripting integration
	*/
	void Initialize(Variant** argv, int argc);

	/**
	* Scripting functions
	*/
	static Variant* Execute(Variant* instance, int argc, Variant** argv);
	static Variant* AddPostData(Variant* instance, int argc, Variant** argv);

protected:
	// Whether the CURL system is initialized
	static bool m_initialized;

	// Store the response
	unsigned char* m_response;
	unsigned int m_bytes;

	// A CURL handle
	CURL* m_handle;

	// Post data
	std::map<std::string, std::string> m_vars;

	// Response HTTP code
	long m_responseCode;

	// The URL we are trying to access
	std::string m_url;
};

#endif