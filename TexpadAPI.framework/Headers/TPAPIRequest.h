//
//  TPAPIRequest.h
//  Texpad API test
//
//  Created by Duncan Steele on 05/03/2013.
//  Copyright (c) 2013 Valletta Ventures LLP. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
// * Neither the name of the <organization> nor the
//   names of its contributors may be used to endorse or promote products
//   derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#import <Foundation/Foundation.h>

/**
 * A status code type for a request
 */
typedef enum {
    /**
     * The request is brand new and has not been sent yet
     */
    TPAPIRequestStatusNew,
    
    /**
     * The request has been sent to Texpad and the manager is awaiting a response
     */
    TPAPIRequestStatusInProgress,
    
    /**
     * The request has returned successfully from Texpad.
     * This does not guarantee that there were no typeset errors, just that the communication was successful
     */
    TPAPIRequestStatusSuccess,
    
    /**
     * The request itself failed.
     * Either it timed out, or it was not possible to send it
     */
    TPAPIRequestStatusFailure,
    
    /**
     * The request was fine, but the typeset failed sufficiently badly that a PDF was not created
     */
    TPAPIRequestStatusTypesetFailure,
} TPAPIRequestStatus;

/**
 * A class to encapsulate a single request
 *
 * This class contains all information needed for communcations between the TPAPI and Texpad
 * It can be serialised for transmission
 */
@interface TPAPIRequest : NSObject <NSCoding>

/**
 * Flag to indicate whether Bibtex should be run when typesetting this request
 *
 * This is set to NO by default
 */
@property BOOL shouldBibtex;

/**
 * set this property true to indicate that Texpad should typeset with Texpad-Custom-Packages directory turned on.
 *
 * Files in a user's Texpad-Custom-Packages directory may overwrite the standard versions and cause unexpected behaviour when typesetting
 * When this option is NO you are guaranteed to be using the standard distribution (unless of course the user has jailbroken)
 *
 * Default value is NO
 */
@property BOOL shouldUseCustomPackages;

/**
 * A data object containing the PDF data resulting from the typeset operation
 *
 * This should be saved to a PDF file
 */
@property (nonatomic, retain) NSData *pdfData;

/**
 * The log output by LaTeX
 *
 * If no log is output by latex this will be one of two error messages
 * Texpad: could not decode log
 *  for some reason the log was output in an exotic character encoding that Texpad was not able to interpret
 * Texpad: no log found
 *  no log was found.  This usually indicates a very severe failure to typeset such that TeX exits immediately
 */
@property (nonatomic, copy) NSString *typesetLog;

/**
 * The rootFilePath is the file that Texpad will initiate typeset upon
 * this must be the file containing the standard \documentclass command
 *
 * this property will default to the first path added
 */
@property (nonatomic, copy) NSString *rootFilePath;

/**
 * The URL scheme that should be used to return this request
 *
 * in normal operation this is set by TPAPIManager when dispatching the request
 */
@property (nonatomic, copy) NSString *returnScheme;

/**
 * The Texpad version number string
 *
 * This is set by Texpad when carrying out the typesetting, up until that point it will be nil
 */
@property (nonatomic, copy) NSString *texpadVersion;

/**
 * A unique ID for this request
 *
 * in normal operation this is set by TPAPIManager when dispatching the request
 *
 * Please use the tag if you wish to assign an identifier for your own use
 */
@property (nonatomic, copy) NSString *requestId;

/**
 * Add a file at the given path to the request
 */
-(void)addData:(NSData*)data atPath:(NSString*)path;

/**
 * The current status of this request
 */
@property TPAPIRequestStatus status;

/**
 * An integer tag for this request
 *
 * This is intended as a unique identifier for the application using this API to use internally
 * Unlike requestId it may be set before the request is dispatched as TPAPIManager will not interfere with it
 */
@property NSInteger tag;

/**
 * call this to clear the source files from this request
 *
 * This is used for minimising the size for transmission of the document back to the calling application
 * Do not call it yourself as it will strip all the .tex files out of the request, it is intended for Texpad's use only
 */
-(void)stripRequest;

/**
 * extract all source files in the request to a directory
 *
 * This is intended for Texpad to use when typesetting the document, not for an API client
 *
 * @param basePath the base path for the extraction
 */
-(void)extractToPath:(NSString*)basePath;

/**
 * a convenience method for creating requests
 */
+(TPAPIRequest*)request;

/**
 * All source file paths contained within the request
 *
 * @return set of all paths contained within this request
 */
-(NSSet*)allPaths;

@end
