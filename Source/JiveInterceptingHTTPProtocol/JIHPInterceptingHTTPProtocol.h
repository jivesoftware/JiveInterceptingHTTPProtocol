/*
 File: JIHPInterceptingHTTPProtocol.h
 Abstract: An NSURLProtocol subclass that overrides the built-in HTTP/HTTPS protocol.
 Version: 1.1
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2014 Apple Inc. All Rights Reserved.
 
 */

@import Foundation;

@protocol JIHPInterceptingHTTPProtocolDelegate;

/*! An NSURLProtocol subclass that overrides the built-in HTTP/HTTPS protocol to intercept
 *  authentication challenges for subsystems, ilke UIWebView, that don't otherwise allow it.
 *  To use this class you should set up your delegate (+setDelegate:) and then call +start.
 *  If you don't call +start the class is completely benign.
 *
 *  The really tricky stuff here is related to the authentication challenge delegate
 *  callbacks; see the docs for JIHPInterceptingHTTPProtocolDelegate for the details.
 */

@interface JIHPInterceptingHTTPProtocol : NSURLProtocol

/*! Call this to start the module.  Prior to this the module is just dormant, and
 *  all HTTP requests proceed as normal.  After this all HTTP and HTTPS requests
 *  go through this module.
 */

+ (void)start;

/*! Call this to stop the module.  After this no HTTP and HTTPS requests will
 *  go through this module.
 */
+ (void)stop;

/*! Sets the delegate for the class.
 *  \details Note that there's one delegate for the entire class, not one per
 *  instance of the class as is more normal.  The delegate is weakly referenced in general,
 *  but is retained for the duration of any given call.  Once you set the delegate to nil
 *  you can be assured that it won't be called unretained (that is, by the time that
 *  -setDelegate: returns, we've already done all possible retains on the delegate).
 *  \param newValue The new delegate to use; may be nil.
 */

+ (void)setDelegate:(nullable id<JIHPInterceptingHTTPProtocolDelegate>)newValue;

/*! Returns the class delegate.
 */

+ (nullable id<JIHPInterceptingHTTPProtocolDelegate>)delegate;

@end

/*! The delegate for the JIHPInterceptingHTTPProtocol class (not its instances).
 *  \details The delegate handles two different types of callbacks:
 *
 *  - interception
 *
 *  - logging
 */

@protocol JIHPInterceptingHTTPProtocolDelegate <NSObject>

#pragma mark - intercept

/*! Called by the JIHPInterceptingHTTPProtocol class to test interception of a whole NSURLRequest.
 *  This is only called if the NSURLRequest's URL is nonnull.
 *  JIHPInterceptingHTTPProtocol applies a recursive property to intercepted requests so that
 *  clients don't need guard against recursive interception.
 *  Can be called on any thread.
 *  \param interceptingHTTPProtocol The protocol instance itself
 *  \param originalRequest The original NSURLRequest that 
 *  \returns true if the request can be intercepted. false otherwise.
 */

- (nonnull NSMutableURLRequest *)interceptingHTTPProtocol:(nonnull JIHPInterceptingHTTPProtocol *)interceptingHTTPProtocol interceptRequest:(nonnull NSURLRequest *)originalRequest;

@optional

#pragma mark - canIntercept

/*! Called by the JIHPInterceptingHTTPProtocol class to test interception of a whole NSURLRequest.
 *  This is only called if the NSURLRequest's URL is nonnull and its scheme is http or https.
 *  JIHPInterceptingHTTPProtocol applies a recursive property to intercepted requests so that
 *  clients don't need guard against recursive interception.
 *  Can be called on any thread.
 *  \param request the NSURLRequest that could be intercepted
 *  \returns true if the request can be intercepted. false otherwise.
 */

- (BOOL)canInterceptRequest:(nonnull NSURLRequest *)request;

/*! Called by the JIHPInterceptingHTTPProtocol class to test interception of a given NSURL.
 *  This is only called if -canInterceptRequest: is not implemented,
 *  the NSURLRequest's URL is nonnull, and the NSURLRequest's URL scheme is http or https.
 *  JIHPInterceptingHTTPProtocol applies a recursive property to intercepted requests so that
 *  clients don't need guard against recursive interception.
 *  Can be called on any thread.
 *  \param request the NSURL of the NSURLRequest that could be intercepted
 *  \returns true if the request can be intercepted. false otherwise.
 */
- (BOOL)canInterceptURL:(nonnull NSURL *)URL;

#pragma mark - log

/*! Called by the JIHPInterceptingHTTPProtocol to log various bits of information.
 *  Can be called on any thread.
 *  \param interceptingHTTPProtocol The protocol instance itself; nil to indicate log messages from the class itself.
 *  \param format A standard NSString-style format string; will not be nil.
 *  \param arguments Arguments for that format string.
 */

- (void)interceptingHTTPProtocol:(nullable JIHPInterceptingHTTPProtocol *)interceptingHTTPProtocol logWithFormat:(nonnull NSString *)format
// clang's static analyzer doesn't know that a va_list can't have an nullability annotation.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
                       arguments:(va_list)arguments;
#pragma clang diagnostic pop

/*! Called by the JAHPAuthenticatingHTTPProtocol to log various bits of information. Use this if implementing in Swift. Swift doesn't like
 * -authenticatingHTTPProtocol:logWithFormat:arguments: because
 * `Method cannot be marked @objc because the type of the parameter 3 cannot be represented in Objective-C`
 *  I assume this is a problem with Swift not understanding that CVAListPointer should become va_list.
 *  Can be called on any thread.
 *  \param interceptingHTTPProtocol The protocol instance itself; nil to indicate log messages from the class itself.
 *  \param message A message to log
 */

- (void)interceptingHTTPProtocol:(nullable JIHPInterceptingHTTPProtocol *)interceptingHTTPProtocol logMessage:(nonnull NSString *)message;

@end
