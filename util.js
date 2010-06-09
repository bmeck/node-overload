var overload=require("./overload")
, Watchable = overload.Watchable
, RePrototype = overload.RePrototype

exports.ReadOnly=ReadOnly=function(obj,mask) {
	var $this=Watchable(function(args){
		var v=mask
		&& args.property in mask
			? mask[args.property]
			: obj[args.property];
		if(typeof(v)=="function") {
			return function(){return v.apply(obj,arguments)}
		}
		return v;
	});
	//only objects have prototypes
	if(obj instanceof Object) {
		RePrototype($this,Object.getPrototypeOf(obj))
	}
	return $this;
}
var noop=function(){}
ReadOnly.ArrayMask={
	pop:function(){return this[this.length-1];}
	,push:function(){return this.length}
	,reverse:noop
	,shift:function(){return this[0];}
	,sort:noop
	,splice:function(){return [];}
	,unshift:function(){return this.length}
}
ReadOnly.DateMask={
	setDate:noop
	,setFullYear:noop
	,setHours:noop
	,setMilliseconds:noop
	,setMinutes:noop
	,setMonth:noop
	,setSeconds:noop
	,setTime:noop
	,setUTCDate:noop
	,setUTCFullYear:noop
	,setUTCHours:noop
	,setUTCMilliseconds:noop
	,setUTCMinutes:noop
	,setUTCMonth:noop
	,setUTCSeconds:noop
	,setYear:noop
}
ReadOnly.StringMask={}
delete noop;