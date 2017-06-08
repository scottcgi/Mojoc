/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 * 
 * Since  : 2017-3-13
 * Author : scott.cgi
 * Version: 0.1
 */

import GLKit
import OpenGLES

class GameViewController: GLKViewController
{
    var context: EAGLContext? = nil
    
    deinit
    {
        EAGLContext.setCurrent(nil)
    }
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        self.context = EAGLContext(api: .openGLES3)
        
        if (self.context == nil)
        {
            print("Failed to create ES context")
        }
        EAGLContext.setCurrent(self.context)

//----------------------------------------------------------------------------------------------
        
        let view                         = self.view as! GLKView
        view.context                     = self.context!
        view.drawableDepthFormat         = .format24
        view.drawableColorFormat         = .RGBA8888
        self.preferredFramesPerSecond    = 60
        self.view.isMultipleTouchEnabled = true

//----------------------------------------------------------------------------------------------
        
        AApplication.GLReady(Int32(view.bounds.size.width * view.contentScaleFactor), Int32(view.bounds.size.height * view.contentScaleFactor));
    }
    
    override func didReceiveMemoryWarning()
    {
        super.didReceiveMemoryWarning()
        
        if self.isViewLoaded && (self.view.window != nil)
        {
            self.view    = nil
            self.context = nil
            EAGLContext.setCurrent(nil)
        }
    }
    
    func update()
    {
        // application main loop
        AApplication.Loop();
    }
    
    
//----------------------------------------------------------------------------------------------
    
    static let fingerCount  = 10
    var        fingerIds    = [Bool](repeating: false, count: fingerCount)
    // k-v: UITouch - fingerId
    var        touchDict    = [UITouch : Int]()
    var        inputTouches = [UnsafeMutablePointer<InputTouch>]()
    
    func touch()
    {
        var array = Mojoc.Array(data: UnsafeMutableRawPointer.init(mutating: inputTouches), length: Int32(inputTouches.count))
        
        AApplication.Touch(&array);
        
        // clear for next use
        inputTouches.removeAll()
    }

    
    func touchReleased(_ touches: Set<UITouch>, _ inputTouchType: InputTouchType)
    {
        for touch in touches
        {
            let fingerId = touchDict[touch]
            let tp       = touch.location (in: self.view)
            let it       = AInput.SetTouch(Int32(fingerId!), Float(tp.x * self.view.contentScaleFactor), Float(tp.y * self.view.contentScaleFactor), inputTouchType);
                
            // collect InputTouch pointer
            inputTouches.append(it!)
                    
            // remove UITouch by finger id
            touchDict.removeValue(forKey: touch)
            
            // reset finger id slot
            fingerIds[fingerId!] = false
        }
        
        self.touch()
    }
    
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?)
    {
        var index = 0
        
        for touch in touches
        {
            for fingerId in index...fingerIds.count
            {
                // find unused finger id
                if fingerIds[fingerId] == false
                {
                    let tp = touch.location (in: self.view)
                    let it = AInput.SetTouch(Int32(fingerId), Float(tp.x * self.view.contentScaleFactor), Float(tp.y * self.view.contentScaleFactor), InputTouchType_Down);
                    
                    // collect InputTouch pointer
                    inputTouches.append(it!)
                    
                    // record fingerId by UITouch
                    touchDict[touch]    = fingerId
                    
                    // flag finger id slot
                    fingerIds[fingerId] = true
                    
                    // next search pos
                    index               = fingerId + 1
                    
                    break;
                }
            }
        }
        
        self.touch()
    }
    
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?)
    {
        for touch in touches
        {
            let fingerId = touchDict[touch]
            let tp       = touch.location (in: self.view)
            let it       = AInput.SetTouch(Int32(fingerId!), Float(tp.x * self.view.contentScaleFactor), Float(tp.y * self.view.contentScaleFactor), InputTouchType_Move);
                
            // collect InputTouch pointer
            inputTouches.append(it!)
        }
        
        self.touch()
    }
    
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?)
    {
        self.touchReleased(touches, InputTouchType_Up)
    }
    
    
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?)
    {
        self.touchReleased(touches, InputTouchType_Cancel)
    }
}


