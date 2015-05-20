//
//  ViewController.m
//  JiveInterceptingHTTPProtocolDemo
//
//  Created by Heath Borders on 5/19/15.
//  Copyright (c) 2015 Heath Borders. All rights reserved.
//

#import "ViewController.h"
#import <JiveInterceptingHTTPProtocol/JIHPInterceptingHTTPProtocol.h>

@interface ViewController () <JIHPInterceptingHTTPProtocolDelegate, UIWebViewDelegate>

@property (nonatomic, weak) IBOutlet UIWebView *webView;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [JIHPInterceptingHTTPProtocol setDelegate:self];
    [JIHPInterceptingHTTPProtocol start];
    
    [self.webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://www.jivesoftware.com/"]]];
}

#pragma mark - JIHPInterceptingHTTPProtocolDelegate

- (nonnull NSMutableURLRequest *)interceptingHTTPProtocol:(nullable JIHPInterceptingHTTPProtocol *)interceptingHTTPProtocol interceptRequest:(nonnull NSURLRequest *)originalRequest {
    NSMutableURLRequest *mutableRequest = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"http://i1.kym-cdn.com/photos/images/newsfeed/000/096/044/trollface.jpg"]];
    return mutableRequest;
}

- (BOOL)canInterceptURL:(nonnull NSURL *)URL {
    NSString *lowercasePathExtension = [[URL pathExtension] lowercaseString];
    
    BOOL canInterceptURL = ([lowercasePathExtension isEqualToString:@"png"] ||
                            [lowercasePathExtension isEqualToString:@"jpg"] ||
                            [lowercasePathExtension isEqualToString:@"jpeg"] ||
                            [lowercasePathExtension isEqualToString:@"gif"]);
    return canInterceptURL;
}

- (void)interceptingHTTPProtocol:(nullable JIHPInterceptingHTTPProtocol *)interceptingHTTPProtocol logWithFormat:(nonnull NSString *)format arguments:(va_list)arguments {
    NSLog(@"logWithFormat: %@", [[NSString alloc] initWithFormat:format arguments:arguments]);
}

- (void)interceptingHTTPProtocol:(nullable JIHPInterceptingHTTPProtocol *)interceptingHTTPProtocol logMessage:(nonnull NSString *)message {
    NSLog(@"logMessage: %@", message);
}

#pragma mark - UIWebViewDelegate

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    [[[UIAlertView alloc] initWithTitle:@"JIHPDemo"
                                message:error.localizedDescription
                               delegate:nil
                      cancelButtonTitle:@"OK"
                      otherButtonTitles:nil] show];
}

@end
