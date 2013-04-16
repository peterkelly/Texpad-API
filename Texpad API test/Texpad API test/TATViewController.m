//
//  TATViewController.m
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

#import "TATViewController.h"

@implementation TATViewController

@synthesize tvEntry = _tvEntry, outputView = _outputView, typesetButton = _typesetButton;

- (void)viewDidLoad {
    [super viewDidLoad];

}

-(void)typesetRequestComplete:(TPAPIRequest *)request {
    if (request.status == TPAPIRequestStatusSuccess) {
        // load request into pdf view
        NSString *pdfPath = [NSTemporaryDirectory() stringByAppendingPathComponent:@"out.pdf"];
        [request.pdfData writeToFile:pdfPath atomically:NO];
        [_outputView loadRequest:[NSURLRequest requestWithURL:[NSURL fileURLWithPath:pdfPath]]];
        
        // show log on console
        NSLog(@"%@", request.typesetLog);
    } else if (request.status == TPAPIRequestStatusTypesetFailure) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Typeset failed"
                                                        message:@"general latex error"
                                                       delegate:nil
                                              cancelButtonTitle:@"I'm sorry"
                                              otherButtonTitles:nil];
        [alert show];
    } else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Request failed"
                                                        message:@"error transmitting or receiving the request"
                                                       delegate:nil
                                              cancelButtonTitle:@"Ok"
                                              otherButtonTitles:nil];
        [alert show];
    }
    
    _typesetButton.enabled = YES;
}

-(IBAction)pressedTypeset:(id)sender {
    if ([[TPAPIManager sharedManager] typesetterPresent]) {
        [TPAPIManager sharedManager].delegate = self;
        
        _typesetButton.enabled = NO;
        
        NSString *rootFile = _tvEntry.text;
        TPAPIRequest *request = [[TPAPIRequest alloc] init];
        [request addData:[rootFile dataUsingEncoding:NSUTF8StringEncoding]
                  atPath:@"root.tex"];
        
        NSString *includedFile = @"This is contained in included.tex";
        [request addData:[includedFile dataUsingEncoding:NSUTF8StringEncoding]
                  atPath:@"included.tex"];
        
        NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"palmtree" ofType:@"jpg"];
        NSData *imageData = [NSData dataWithContentsOfFile:imagePath];
        [request addData:imageData
                  atPath:@"palmtree.jpg"];
        
        // this isn't actually necessary as it defaults to the first file added
        request.rootFilePath = @"root.tex";
        
        [[TPAPIManager sharedManager] submitRequest:request];
    } else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Texpad not installed"
                                                        message:@"Texpad isn't installed on this device"
                                                       delegate:nil
                                              cancelButtonTitle:@"Ok"
                                              otherButtonTitles:nil];
        [alert show];
    }
}

@end
