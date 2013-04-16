//
//  TPAPIManager.h
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

#import <UIKit/UIKit.h>

@class TPAPIRequest;
@protocol TPAPIManagerDelegate;

/**
 * A Singleton API manager instance.
 * Always use the instance created by the sharedManager class method 
 *
 * When TPAPIManager is initialised for the first time it will look for a texpad url scheme in your bundle's info.plist
 * TPAPIManager will throw an exception if an appropriate URL scheme is not found
 *
 * Your URL scheme must be prefixed with tpapi- and the remainder should be unique to your application.
 * Once you have chosen one (for example tpapi-testapp) please add it to your info.plist as follows:
 *
 * <key>CFBundleURLTypes</key>
 * <array>
 * <dict>
 * <key>CFBundleURLSchemes</key>
 * <array>
 * <string>tpapi-testapp</string>
 * </array>
 * </dict>
 * </array>
 *
 * Do not alloc this class, used the shared object created by sharedManager method
 */
@interface TPAPIManager : NSObject

#pragma mark - static methods

/**
 * this returns a singleton instance of the manager
 *
 * ALWAYS interact with this object, do not create one yourself
 */
+(TPAPIManager*)sharedManager;

/**
 * Test for the presence of a suitable typesetter on this system
 *
 * @return  YES if a typesetter was found, NO otherwise
 */
-(BOOL)typesetterPresent;

/**
 * Find the url scheme used by this manager
 *
 * @return the url scheme as a string
 */
-(NSString*)urlScheme;

/**
 * Return the pasteboard type for data transfer
 *
 * @return a pasteboard type for data transfer
 */
-(NSString*)pasteboardType;

#pragma mark - instance methods

/**
 * submit a request to the typesetter
 *
 * you will be notified of success or failure via the request's completion handler
 * this may be called immediately if there are queued requests as requests will not be sent until the delegate has been set
 */
-(void)submitRequest:(TPAPIRequest*)request;

/**
 * Call this with a URL to allow TPAPIManager to handle a URL
 *
 * A call to this MUST be inserted into uiapplicationdelegate's application:handleURL: method
 *
 * @param   the URL to be handled
 * @return  a boolean indicating whether the manager handled the URL or not
 */
-(BOOL)handleOpenURL:(NSURL*)url;

/**
 * The manager delegate is the object that will receive all requests upon completion
 */
@property (nonatomic, assign) id<TPAPIManagerDelegate> delegate;

@end
