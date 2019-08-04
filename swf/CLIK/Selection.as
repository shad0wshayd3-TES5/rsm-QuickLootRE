//****************************************************************************
// ActionScript Standard Library
// Selection control
//****************************************************************************

intrinsic class Selection
{
	static function addListener(listener:Object):Void;
	static function getBeginIndex():Number;
	static function getCaretIndex():Number;
	static function getEndIndex():Number;
	static function getFocus():String;
	static function removeListener(listener:Object):Boolean;
	static function setFocus(newFocus:Object):Boolean; // newFocus can be string path or Object itself
	static function setSelection(beginIndex:Number, endIndex:Number):Void;
	
	// scaleform extensions
	
	/*
	static var alwaysEnableArrowKeys:Boolean;
	static var alwaysEnableKeyboardPress:Boolean;
	static var disableFocusAutoRelease:Boolean;
	static var disableFocusKeys:Boolean;
	static var disableFocusRolloverEvent:Boolean;
	static var modalClip:MovieClip;
	static var numFocusGroups:Number;
	
	static function captureFocus([doCapture:Boolean, controllerIdx:Number]):Void;
	static function moveFocus(keyToSimmulate:String [, startFromMovie:Object, includeFocusEnabledChars:Boolean = false, controllerIdx:Number]):Object;
	static function findFocus(keyToSimmulate:String [, parentMovie:Object, loop:Boolean, startFromMovie:Object, includeFocusEnabledChars:Boolean, controllerIndex:Number]):Object;
	static function setModalClip(modalClip:Object, controllerIndex:Number):Void;
	static function getModalClip(controllerIndex:Number):Object;
	static function setControllerFocusGroup(controllerIndex:Number, focusGroupIdx:Number):Boolean;
	static function getControllerFocusGroup(controllerIndex:Number):Number;
	static function getFocusArray(mc:Object):Array;
	static function getFocusBitmask(mc:Object):Number;
	static function getControllerMaskByFocusGroup(focusGroupIdx:Number):Number;
	*/
}