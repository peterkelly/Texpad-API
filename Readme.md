# Texpad API

### Introduction

This is a typesetting API that enables any iOS application to typeset LaTeX documents using Texpad's on board LaTeX typesetter.

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
    
### 
