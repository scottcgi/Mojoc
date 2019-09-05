/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2017-3-13
 * Update   : 2019-5-8
 * Author   : scott.cgi
 */


import GLKit
import OpenGLES


class GameViewController: GLKViewController {
    var context: EAGLContext? = nil

    deinit {
        EAGLContext.setCurrent(nil)
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        self.context = EAGLContext(api: .openGLES3)
        
        if (self.context == nil) {
            print("Failed to create ES context.")
        }

        EAGLContext.setCurrent(self.context)

        let view                      = self.view as! GLKView
        view.context                  = self.context!
        view.drawableDepthFormat      = .format24
        view.drawableColorFormat      = .RGBA8888
        view.isMultipleTouchEnabled   = true
        self.preferredFramesPerSecond = 60

        AApplication.Ready(
            Int32(view.bounds.size.width  * view.contentScaleFactor),
            Int32(view.bounds.size.height * view.contentScaleFactor)
        )
    }

    // delay system edge gestures to let app touches can be processed first
    override var preferredScreenEdgesDeferringSystemGestures: UIRectEdge {
        return .all;
    }


    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        if self.isViewLoaded && (self.view.window != nil) {
            self.view    = nil
            self.context = nil
            EAGLContext.setCurrent(nil)
        }
    }


    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        // application main loop
        AApplication.Loop()
    }


//----------------------------------------------------------------------------------------------------------------------


    static let fingerCount   = 10
    // the fingerId is UITouch index 
    var        fingerTouches = [UITouch?](repeating: nil, count: Int(fingerCount))
    var        fingerIds     = UnsafeMutablePointer<Int32>.allocate(capacity: fingerCount)
    var        pixelXs       = UnsafeMutablePointer<Float>.allocate(capacity: fingerCount)
    var        pixelYs       = UnsafeMutablePointer<Float>.allocate(capacity: fingerCount)

    func touchesHandle(_ touches: Set<UITouch>, _ inputTouchType: InputTouchType) {
        var count         = 0;
        var fingerId:Int? = nil

        for touch in touches {
            if (inputTouchType == InputTouchType_Down) {
                fingerId = fingerTouches.firstIndex(of: nil)
                // record touch with fingerId
                fingerTouches[fingerId!] = touch
            } else {
                // get the touch's fingerId
                fingerId = fingerTouches.firstIndex(of: touch)

                if (inputTouchType != InputTouchType_Move) {
                    // remove touch with fingerId when InputTouchType_Up or InputTouchType_Cancel
                    fingerTouches[fingerId!] = nil
                }
            }

            let pos          = touch.location(in: self.view)
            fingerIds[count] = Int32(fingerId!)
            pixelXs  [count] = Float(pos.x * self.view.contentScaleFactor)
            pixelYs  [count] = Float(pos.y * self.view.contentScaleFactor)

            count += 1
        }

        AApplication.Touches(
            fingerIds,
            pixelXs,
            pixelYs,
            Int32(count),
            inputTouchType
        )
    }


    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        self.touchesHandle(touches, InputTouchType_Down)
    }
    
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        self.touchesHandle(touches, InputTouchType_Move)
    }
    
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        self.touchesHandle(touches, InputTouchType_Up)
    }
    
    
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        self.touchesHandle(touches, InputTouchType_Cancel)
    }
}


