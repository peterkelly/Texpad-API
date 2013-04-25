# Texpad API

### Introduction

Texpad-API.framework is a typesetting API that enables any iOS application to typeset LaTeX documents using Texpad's built-in LaTeX typesetter.

Please note that the API server was only added to Texpad 1.5.  This API will not work with earlier versions of Texpad.

### Example project

The "Texpad API Test" folder contains an example project that demonstrates this API.  You will need to alter the bundle id, and provide your own provisioning profile in order to install it on your device.

### Getting started guide

To use this library in your project, you must first add the framework in Xcode.  Having selected the relevant target, choose the "Build Phases" pane, open the "Link Binary with Libraries" leaf and press the "+" button to add a framework.  Next choose "Add other ..." and navigate to Texpad-API.framework.

The framework's include statement is

    #import <TexpadAPI/TexpadAPI.h>


###### Choose a URL Scheme

Next it is necessary to choose a return URL scheme that Texpad will use to pass TeX's PDF document back to your application. This URL scheme **must** be prefixed with `tpapi-` and it should be unique to your application.  Once you have chosen one (for example tpapi-com.yourdomain.yourapp) please add it to your target's Info.plist.

	<key>CFBundleURLTypes</key>
	<array>
		<dict>
			<key>CFBundleURLSchemes</key>
			<array>
				<string>tpapi-testapp</string>
			</array>
		</dict>
	</array>

When the TPAPIManager object is first instantiated it will search through your Application's Info.plist to find this url scheme, hence the requirement that it must be prefixed with `tpapi-`.  It will throw an exception if no URL scheme is found.  

###### Add handler hook to Application Delegate's handleOpenURL:

Once the URL scheme has been chosen and entered into your target's Info.plist, you must add a handler hook to your application delegate's application:handleOpenURL: method

    -(BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
        if ([[TPAPIManager sharedManager] handleOpenURL:url]) {
            return YES;
        }

        // handle other URLs here
        
        return NO;
    }

This will intercept any URL open requests matching Texpad API's naming scheme, unpack them and pass them to `[TPAPIManager sharedManager]`'s delegate.  All other URL schemes will be ignored.

Please note that all access to `TPAPIManager` should be through the singleton object returned by [TPAPIManager sharedManager].  Do not instantiate multiple API managers yourself.

###### Set TPAPIManager's delegate

When a typeset request is returned from Texpad, it will be passed to `[TPAPIManager sharedManager]`'s delegate.

Implement the `TPAPIManagerDelegate` protocol in an object and set this object as the delegate of the shared `TPAPIManager` instance.

    @interface SomeObject : SomeSuperclass <TPAPIManagerDelegate> {

    . . .

    SomeObject *obj = [[SomeObject alloc] …
    [TPAPIManager sharedManager].delgate = obj;

Please note that when the delegate is nil, TPAPIManager will queue any returned requests, so that they are not lost.  They will be delivered when the delegate is set to a non-nil value for the first time.

This is done because although very unlikely, there is no guarantee that once an application goes out of focus, iOS will not decide to terminate it.  Whilst typesetting a document, your application will briefly be out of focus, so in the unlikely case that iOS runs out of memory and terminates your application this structure guarantees that any pending typeset requests will not be lost.

###### Check that Texpad is installed

You can verify whether Texpad is installed or not by calling typesetterPresent on the shared manager object object.

    if ([[TPAPIManager sharedManager] typesetterPresent]) {
        // action if API found
    } else {
        // action if API not found
    }

This allows you to unhide any options in your application that should only be visible when Texpad is present.

###### Submit a request

Once everything is setup, to typeset a document you should create a `TPAPIRequest` object as follows

    // create object
    TPAPIRequest *request = [[TPAPIRequest alloc] init];
    
    // add a single latex file
    NSString *rootFile = @"\\documentclass{article}\n\\begin{document}\nHello world\n\\end{document}\n";
    [request addData:[rootFile dataUsingEncoding:NSUTF8StringEncoding]
              atPath:@"root.tex"];

You may add as many LaTeX source files, image files, or other data files as you need to.  By default the first file added will be considered the root file for typeset purposes, but you can customise this default with the `rootFilePath` property on `TPAPIRequest`.

If your application may be submitting multiple requests at the same time (not recommended at all), or if you want to handle the possibility that iOS may terminate your application when it goes out focus during typeset, then we recommend that you assign an internal ID using the `tag` field of the TPAPIRequest object.

When ready, dispatch the request via the typeset manager

    [[TPAPIManager sharedManager] submitRequest:request];

Your application will go out of focus briefly whilst the request is typeset, and when finished Texpad will pass focus back to your application.  It does not provide the user with any choices, or anything that would cause the focus to fail to return to your application.

The request will be typeset twice to ensure that any references or tables of contents or figures are built correctly.

###### Handle the returned request

You handle the returned request in the delegate's typesetRequestComplete: method.  The status field informs you of the success or failure of the typeset, and if the typeset has failed you can find out why by checking the log.

    -(void)typesetRequestComplete:(TPAPIRequest*)request {
        NSLog(@"Request %ld returned", request.tag);

        if (request.status == TPAPIRequestStatusSuccess) {
            NSLog(@"success!");

            // show log on console
            NSLog(@"%@", request.typesetLog);
        } else if (request.status == TPAPIRequestStatusTypesetFailure) {
            NSLog(@"typeset failed!");

            // show log on console
            NSLog(@"%@", request.typesetLog);
        } else if (request.status == TPAPIRequestStatusFailure) {
            NSLog(@"other failure!");
        }
    }

###### BibTeX

BibTeX will not be run by default, but if you set the request's `shouldBibtex` flag to true, it will be executed.

    reqest.shouldBibtex = YES

The document will be typeset three times with one Bibtex run to ensure that references are correctly resolved (LaTeX, BibTeX, LaTeX, LaTeX).

### Full Documentation

The most detailed, and up to date, documentation is available in the header files of the framework in Doxygen format.

### Contact 

If you have any questions, or trouble integrating this framework into your application, or suggestions for improvement please get in contact with us at <support@vallettaventures.com>.

### Applications that

We'd love to hear about anybody that is using the Texpad API, so that we can maintain a list of client applications on our website.  The list so far is:

* [UX Write](http://uxproductivity.com/)

### License

This code is available under the following BSD license

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