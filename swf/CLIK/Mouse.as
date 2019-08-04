import flash.geom.Point;
//****************************************************************************
// ActionScript Standard Library
// Mouse object
//****************************************************************************

intrinsic class Mouse
{
	static function addListener(listener:Object):Void;
	static function hide():Number;
	static function removeListener(listener:Object):Boolean;
	static function show():Number;
	
	// scaleform extensions
	/*
	static function getButtonsState(mouseIndex:Number):Number;
	
	static function getTopMostEntity([mouseIndex:Number]):Object;
	static function getTopMostEntity(testAll:Boolean, [mouseIndex:Number]):Object;
	static function getTopMostEntity(x:Number, y:Number):Object;
	static function getTopMostEntity(x:Number, y:Number, testAll:Boolean):Object;
	
	static function getPosition(mouseIndex:Number):Point;
	
	static function setCursorType(cursorType:Number, [mouseIndex:Number]):Void;
	*/
}