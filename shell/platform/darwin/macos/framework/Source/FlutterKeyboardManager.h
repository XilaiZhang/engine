// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "flutter/shell/platform/darwin/macos/framework/Headers/FlutterEngine.h"
#import "flutter/shell/platform/darwin/macos/framework/Source/FlutterKeyboardViewDelegate.h"

namespace {
// Someohow this pointer type must be defined as a single type for the compiler
// to compile the function pointer type (due to _Nullable).
typedef NSResponder* _NSResponderPtr;
}

typedef _Nullable _NSResponderPtr (^NextResponderProvider)();

/**
 * Processes keyboard events and cooperate with |TextInputPlugin|.
 *
 * A keyboard event goes through a few sections, each can choose to handled the
 * event, and only unhandled events can move to the next section:
 *
 * - Pre-filtering: Events during IME are sent to the system immediately
 *   (to be implemented).
 * - Keyboard: Dispatch to the embedder responder and the channel responder
 *   simultaneously. After both responders have responded (asynchronously), the
 *   event is considered handled if either responder handles.
 * - Text input: Events are sent to |TextInputPlugin| and are handled
 *   synchronously.
 * - Next responder: Events are sent to the next responder as specified by
 *   |viewDelegate|.
 */
@interface FlutterKeyboardManager : NSObject

/**
 * Create a keyboard manager.
 *
 * The |engine| is a weak reference, used for embedder APIs and channel
 * messages.
 *
 * The |viewDelegate| is a weak reference, typically implemented by
 * |FlutterViewController|.
 */
- (nonnull instancetype)initWithEngine:(nonnull FlutterEngine*)engine
                          viewDelegate:(nonnull id<FlutterKeyboardViewDelegate>)viewDelegate;

/**
 * Processes a key event.
 *
 * Unhandled events will be dispatched to the text input system, and possibly
 * the next responder afterwards.
 */
- (void)handleEvent:(nonnull NSEvent*)event;

@end
