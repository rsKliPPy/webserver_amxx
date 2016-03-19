#if defined _webserver_included
	#endinput
#endif
#define _webserver_included

#pragma reqlib webserver
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib webserver
#endif

/**
 * Maximum length of the client address.
 * It's only 16 as IPv6 is not supported.
 */
#define WS_ADDRESS_LENGTH 16


/**
 * One of these values should be returned from a request callback.
 * Return MHD_NOTFOUND if you returned no response so the 404 page could be displayed.
 * Return MHD_NO if there was an unexpected error.
 * MHD_YES should never be returned really, return a value that WS_QueueResponse() returns.
 */
#define MHD_YES		1
#define MHD_NO 		0
#define MHD_NOTFOUND	(-1)


/**
 * A newly declared WSResponse handle should be initialized to this value.
 * This value is also returned by WS_Create*Response() natives if they fail.
 */
enum WSResponse {
	Invalid_Response = -1
};

/**
 * To be used when querying responses with WS_QueryResponse()
 */
// Information codes
#define WS_StatusCode_Continue 100
#define WS_StatusCode_SwitchingProtocols 101
#define WS_StatusCode_Processing 102
	
// Success codes
#define WS_StatusCode_OK 200
#define WS_StatusCode_Created 201
#define WS_StatusCode_Accepted 202
#define WS_StatusCode_NonAuthorativeInfo 203
#define WS_StatusCode_NoContent 204
#define WS_StatusCode_ResetContent 205
#define WS_StatusCode_PartialContent 206
#define WS_StatusCode_MultiStatus 207
#define WS_StatusCode_AlreadyReported 208
#define WS_StatusCode_IMUsed 226
	
// Redirection codes
#define WS_StatusCode_MultipleChoices 300
#define WS_StatusCode_MovedPermanently 301
#define WS_StatusCode_Found 302
#define WS_StatusCode_SeeOther 303
#define WS_StatusCode_NotModified 304
#define WS_StatusCode_UseProxy 305
#define WS_StatusCode_SwitchProxy 306
#define WS_StatusCode_TemporaryRedirect 307
#define WS_StatusCode_PermanentRedirect 308
	
// Client error codes
#define WS_StatusCode_BadRequest 400
#define WS_StatusCode_Unauthorized 401
#define WS_StatusCode_PaymentRequired 402
#define WS_StatusCode_Forbidden 403
#define WS_StatusCode_NotFound 404
#define WS_StatusCode_MethodNotAllowed 405
#define WS_StatusCode_NotAcceptable 406
#define WS_StatusCode_ProxyAuthRequired 407 // Proxy Authentication Required
#define WS_StatusCode_RequestTimeout 408
#define WS_StatusCode_Conflict 409
#define WS_StatusCode_Gone 410
#define WS_StatusCode_LengthRequired 411
#define WS_StatusCode_PreconditionFailed 412
#define WS_StatusCode_PayloadTooLarge 413
#define WS_StatusCode_URITooLong 414
#define WS_StatusCode_UnsupportedMedia 415 // Unsupported Media Type
#define WS_StatusCode_RangeNotSatisfy 416 // Range Not Satisfiable
#define WS_StatusCode_ExpectationFailed 417
#define WS_StatusCode_ImATeapot 418
#define WS_StatusCode_MisdirectedReq 421 // Misdirected Request
#define WS_StatusCode_UnprocessableEnt 422 // Unprocessable Entity
#define WS_StatusCode_Locked 423
#define WS_StatusCode_FailedDependency 424
#define WS_StatusCode_UpgradeRequired 426
#define WS_StatusCode_PreconditionReq 428 // Precondition Required
#define WS_StatusCode_TooManyRequests 429
#define WS_StatusCode_ReqHeaderTooLarge 431 // Request Header Fields Too Large
#define WS_StatusCode_UnavailableLegal 451 // Unavailable for Legal Reasons
	
// Server error codes
#define WS_StatusCode_InternalError 500 // Internal Server Error
#define WS_StatusCode_NotImplemented 501
#define WS_StatusCode_BadGateway 502
#define WS_StatusCode_ServiceUnavailable 503
#define WS_StatusCode_GatewayTimeout 504
#define WS_StatusCode_HTTPVerNoSupport 505 // HTTP Version Not Supported,
#define WS_StatusCode_VariantAlsoNeg 506 // Variant Also Negotiates
#define WS_StatusCode_InsufficientStore 507 // Insufficient Storage
#define WS_StatusCode_LoopDetected 508
#define WS_StatusCode_NotExtended 510
#define WS_StatusCode_NetworkAuthReq 511  // Network Authentication Required

/**
 * To be used in WS_GetArgument()
 */
enum WSArgumentType {
	WS_ArgumentType_Head = 1,
	WS_ArgumentType_Cookie = 2,
	WS_ArgumentType_Post = 4,
	WS_ArgumentType_Get = 8
};

/**
 * Standard response fields
 */
stock const WSH_AccessControlAllowOrigin[] 	= "Access-Control-Allow-Origin";
stock const WSH_AcceptPatch[]			= "Accept-Patch";
stock const WSH_AcceptRanges[]			= "Accept-Ranges";
stock const WSH_Age[]				= "Age";
stock const WSH_Allow[]				= "Allow";
stock const WSH_CacheControl[]			= "Cache-Control";
stock const WSH_Connection[]			= "Connection";
stock const WSH_ContentDisposition[]		= "Content-Disposition";
stock const WSH_ContentEncoding[]		= "Content-Encoding";
stock const WSH_ContentLanguage[]		= "Content-Language";
stock const WSH_ContentLength[]			= "Content-Length";
stock const WSH_ContentLocation[]		= "Content-Location";
stock const WSH_ContentMD5[]			= "Content-MD5";
stock const WSH_ContentRange[]			= "Content-Range";
stock const WSH_ContentType[]			= "Content-Type";
stock const WSH_Date[]				= "Date";
stock const WSH_ETag[]				= "ETag";
stock const WSH_Expires[]			= "Expires";
stock const WSH_LastModified[]			= "Last-Modified";
stock const WSH_Link[]				= "Link";
stock const WSH_Location[]			= "Location";
stock const WSH_P3P[]				= "P3P";
stock const WSH_Pragma[]			= "Pragma";
stock const WSH_ProxyAuthenticate[]		= "Proxy-Authenticate";
stock const WSH_PublicKeyPins[]			= "Public-Key-Pins";
stock const WSH_Refresh[]			= "Refresh";
stock const WSH_RetryAfter[]			= "Retry-After";
stock const WSH_Server[]			= "Server";
stock const WSH_SetCookie[]			= "Set-Cookie";
stock const WSH_Status[]			= "Status";
stock const WSH_StrictTransportSecurity[]	= "Strict-Transport-Security";
stock const WSH_Trailer[]			= "Trailer";
stock const WSH_TransferEncoding[]		= "Transfer-Encoding";
stock const WSH_TSV[]				= "TSV";
stock const WSH_Upgrade[]			= "Upgrade";
stock const WSH_Vary[]				= "Vary";
stock const WSH_Via[]				= "Via";
stock const WSH_Warning[]			= "Warning";
stock const WSH_WWWAuthenticate[]		= "WWW-Authenticate";
stock const WSH_XFrameOptions[]			= "X-Frame-Options";

/**
 * Common non-standard response fields
 */
stock const WSH_XXSSProtection[]		= "X-XSS-Protection";
stock const WSH_ContentSecurityPolicy[]		= "Content-Security-Policy";  // -
stock const WSH_XContentSecurityPolicy[]	= "X-Content-Security-Policy";// - Aliases
stock const WSH_XWebKitCSP[]			= "X-WebKit-CSP";               // -
stock const WSH_XContentTypeOptions[]		= "X-Content-Type-Options";
stock const WSH_XPoweredBy[]			= "X-Powered-By";
stock const WSH_XUACompatible[]			= "X-UA-Compatible";
stock const WSH_XContentDuration[]		= "X-Content-Duration";
stock const WSH_UpgradeInsecureRequests[]	= "Upgrade-Insecure-Requests";


/**
 * Registers a new request callback for recieveing HTTP requests.
 * The callback will be called only if such request has arrived:
 * 		http://hostname:port/id/other_stuff
 * 		
 * And url parameter in the callback will be equal to "/other_stuff".
 * This is used so each plugin waiting for a request uses another directory, so
 * they don't start clashing.
 * 
 * Callback's prototype should look as follows:
 * 		public function(WSConnection: connection, const method[], const url[])
 * where 
 * 		connection is a connection callback to be used in other natives,
 * 		method is a request method (GET, SET, COOKIE, HEAD),
 * 		url is requested URL.
 * 		
 * @note It's best to return from the request callback with whatever this
 * 		 WS_QueueResponse() returns. If you don't queue any responses
 * 		 return MHD_NOTFOUND to properly display the 404 page.
 * 
 * @param id			Request identificator.
 * @param callback		A function to be called when a request arrives. Must be public.
 * 
 * @return 				1 on success, 0 otherwise.
 */
native WS_RegisterRequestCallback(const id[], const callback[]);

/**
 * Creates a response handle to be used in WS_QueueResponse().
 * 
 * This native creates a page out of a string.
 * 
 * @param content		A string to be made into a response.
 * 
 * @return 				A handle to a response object.
 */
native WSResponse: WS_CreateStringResponse(const content[] = "");

/**
 * Creates a response handle to be used in WS_QueueResponse().
 * 
 * This native creates a response in binary format, meaning that, unlike a string,
 * it can contain null characters. That's why length is required.
 * 
 * @param content		A string to be made into a response.
 * @param length		The number of elements in content array to be read.
 * 
 * @return 				A handle to a response object.
 */
native WSResponse: WS_CreateBinaryResponse(const content[] = "", length = 0);

/**
 * Creates a response handle to be used in WS_QueueResponse().
 * 
 * This native creates a page out of a file. This can be used both for
 * transfering binary files (like images) and HTML documents. If the page size
 * is undeterminate at the moment of creation, instead of creating a string
 * response, which is easily limited in size, write page to a file and then
 * create a file response.
 * 
 * @param path			Path to a file. It is relative to the mod directory,
 * 						i.e cstrike/ for a Counter-Strike server.
 * 						
 * @return 				A handle to a response object.
 */
native WSResponse: WS_CreateFileResponse(const path[]);

/**
 * Destroys a response handle. 
 * 
 * @note You can destroy a response after you've queued it, the response will still
 * 		 be alive internally until it's sent to the client.
 * @note The module will free all handles on the map end, but you shouldn't
 * 		 rely on that. Destroy all handles that you don't need! Don't leave
 * 		 any hanging around.
 * 
 * @param response		A handle to be destroyed.
 * 
 * @noreturn
 */
native WS_DestroyResponse(&WSResponse: response);

/**
 * Queues a response to be sent to a requesting client.
 * 
 * @note It's best to return from the request callback with whatever this
 * 		 native returns.
 * 
 * @param connection	The connection handle from the request callback.
 * @param status		One of the WS_StatusCode_* constants. You would usually
 * 						send WS_StatusCode_OK (200), which means that the resource
 * 						has been found and is being sent.
 * @param response		A response handle.
 * 
 * @param 				1 (MHD_YES) if the response was successfully queued,
 * 						0 (MHD_NO) otherwise.
 */
native WS_QueueResponse(WSConnection: connection, WSStatusCode: status, WSResponse: response);

/**
 * Adds a HTTP header to the response.
 * 
 * @param response		A response handle.
 * @param header		A header to add. You can use one of the predefined
 * 						WSH_* constants, or a custom one for whatever reason.
 * @param content		The value of a header.
 * 
 * @return 				1 on success, 0 if failed (should never fail).
 */
native WS_AddHeader(WSResponse: response, const header[], const content[]);

/**
 * Removes a HTTP header from the response.
 * 
 * @param response		A response handle.
 * @param header		A header to remove. You can use one of the predefined
 * 						WSH_* constants, or a custom one for whatever reason.
 * @param content		The value of a header to remove. If no value is specified,
 * 						all headers with the given name will be removed.
 * 						
 * @return 				1 on success, 0 otherwise.
 */
native WS_RemoveHeader(WSResponse: response, const header[], const content[] = "");

/**
 * Gets the IP address of a requesting client.
 * 
 * @param connection	The connection handle from the request callback.
 * @param ip			String buffer to put the address in. Should have the
 * 						size of WS_ADDRESS_LENGTH elements.
 * @param length		Maximum length of the ip buffer.
 * 
 * @return 				1 if the address was successfully retrieved,
 * 						0 otherwise (should never return 0).
 */
native WS_GetClientAddress(WSConnection: connection, ip[], length);

/**
 * Gets an argument from a request. Arguments are key=>value pairs passed together
 * with the request. For example, GET arguments are stored in the URL itself:
 * 		http://hostname:port/id/page?param1=value&param2=othervalue
 * 
 * @param connection	The connection handle from the request callback.
 * @param type			One of the WS_ArgumentType_* constants. Tells what type
 * 						of argument to retrieve (GET, POST, COOKIE, HEAD).
 * 						Constants can be bitwise ORed to check for multiple types.
 * @param key			The key to search for.
 * @param value			A buffer where the value corresponding to a given key
 * 						will get stored.
 * @param valuelength	The maximum length of the value buffer.
 * 
 * @return 				1 if an argument with a given key exists, 0 if not.
 */
native WS_GetArgument(WSConnection: connection, WSArgumentType: type, const key[], value[], valuelength);





















