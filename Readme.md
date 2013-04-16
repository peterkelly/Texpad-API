# Texpad API

### Introduction

This is a typesetting API that enables any iOS application to typeset LaTeX documents using Texpad's on board LaTeX typesetter.

Please note that the API was only added to T

### Getting started guide

To use this library you must build all files in Texpad API folder into your binary (TPAPI.h, TPAPIManager.*, TPAPIRequest.*, NSString+TPAPICoding.*).  There is no need to include the files in the Texpad API test application

It is necessary to choose a return URL scheme. This must be prefixed with tpapi- and the remainder should be unique to your application.  Once you have chosen one (for example tpapi-testapp) please add it to your info.plist.

	<key>CFBundleURLTypes</key>
	<array>
		<dict>
			<key>CFBundleURLSchemes</key>
			<array>
				<string>tpapi-testapp</string>
			</array>
		</dict>
	</array>

You can check whether Texpad is installed or not before using the API

    #import "TPAPI.h"

    ...

    if ([TPAPIManager typesetterPresent]) {
        // API found
    }

Now build a request

    // create object
    TPAPIRequest *request = [[TPAPIRequest alloc] init];
    
    // add a single latex file
    NSString *rootFile = @"\\documentclass{article}\n\\begin{document}\nHello world\n\\end{document}\n";
    [request addData:[rootFile dataUsingEncoding:NSUTF8StringEncoding]
              atPath:@"root.tex"];

    // handle the result
    request.completionHandler = ^(TPAPIRequest *request) {
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
    };

This request should be dispatched via the api manager

    [[TPAPIManager sharedManager] submitRequest:request];
    
For the return request from Texpad to be handled you need to add a hook to the Application delegate's application:handleOpenURL: method

    -(BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
        if ([[TPAPIManager sharedManager] handleOpenURL:url]) {
            return YES;
        }
        
        return NO;
    }
    
### Full Documentation

The most detailed, and up to date, documentation is available in the header files of the framework in Doxygen format.

### Contact 

### License

This code is available under the following license

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